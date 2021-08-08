#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DFRobot_XianNing.h"

#define BEDROOD_LIGHT  D2

/*配置WIFI名和密码*/
const char * WIFI_SSID     = "lwx_2.4G";
const char * WIFI_PASSWORD = "4001xlwx";

/*配置域名和端口号*/
String XN_SERVER = "122.189.56.75";
uint16_t PORT = 1883;
/*配置设备证书信息*/

/********************************************************************
*********************************************************************
  ************* 设备连接咸宁市工业物联网平台 信息参数配置 *************
        1、ProductId  产品的secureId   平台预设定
        2、ProductKey 产品的secureKey  平台预设定
        3、DeviceId   设备的secureId   平台预设定
        4、DeviceKey  设备的secureKey  平台预设定
        5、DeviceName 设备的名称   必须设定 作为连接时的标识之一
        6、ClientId   客户端名称 即实际连接设备的名称 可自己设定

    注：目前平台MQTT服务器不需要username 和 passwd  所以1、2、3、4项可以不设定
*********************************************************************
*********************************************************************
*/
String ProductId   = "";  
String ProductKey  = "";
String DeviceId    = "";
String DeviceKey   = "";
String DeviceName  = "testDevice01";
String ClientId    = "eps32";
/*需要上报和订阅的两个TOPIC*/
const char * subTopic = "/raspberry/testDevice01/properties/read";//****set
const char * pubTopic = "/raspberry/testDevice01/properties/report";//******post

DFRobot_XianNing myMQTT;
WiFiClient espClient;

PubSubClient client(espClient);

static void openLight(){
  digitalWrite(BEDROOD_LIGHT, HIGH);
}

static void closeLight(){
  digitalWrite(BEDROOD_LIGHT, LOW);
}

void connectWiFi(){
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP Adderss: ");
  Serial.println(WiFi.localIP());
}

void callback(char * topic, byte * payload, unsigned int len){
  Serial.print("Recevice [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < len; i++){
    Serial.print((char)payload[i]);
  }

  Serial.println();
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject((const char *)payload);
  if(!root.success()){
    Serial.println("parseObject() failed");
    return;
  }

  const uint16_t LightStatus = root["properties"]["LEDesp32_LED3"];
  if(LightStatus == 1){
    openLight();
  }else{
    closeLight();
  }
  String tempMseg = "{\"properties\":{\"temperature\":20.5}}";
  char sendMseg[tempMseg.length()];
  strcpy(sendMseg,tempMseg.c_str());
  client.publish(pubTopic,sendMseg);
}

void ConnectIot(){

  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    /*根据自动计算的用户名和密码连接到咸宁市工业物联网平台的设备，不需要更改*/
    if(client.connect(myMQTT.client_id,myMQTT.username,myMQTT.password)){
      Serial.println("connected");
      client.subscribe(subTopic);
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup(){
  Serial.begin(115200);
  pinMode(BEDROOD_LIGHT,OUTPUT);
  
  /*连接WIFI*/
  connectWiFi();
  
  /*初始化Alinyun的配置，可自动计算用户名和密码*/
  myMQTT.init(XN_SERVER,ProductId,ProductKey,ClientId,DeviceName,DeviceId,DeviceKey);
  
  client.setServer(myMQTT.mqtt_server,PORT);
  
  /*设置回调函数，当收到订阅信息时会执行回调函数*/
  client.setCallback(callback);
  
  /*连接到物联网平台*/
  ConnectIot();
  
  /*开机先关灯*/
  closeLight();
  
  /*上报关灯信息*/
  //client.publish(pubTopic,("{\"properties\":{\"temperature\":20.5}}"));
}

void loop(){
  if(!client.connected()){
    ConnectIot();
  }
  client.loop();
  client.publish(pubTopic,("{\"properties\":{\"temperature\":20.5}}"));
}
