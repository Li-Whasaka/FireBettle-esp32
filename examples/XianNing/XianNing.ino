#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DFRobot_XianNing.h"
#include <dht11.h>//引用dht11库文件，使得下面可以调用相关参数                
#define DHT11PIN D5   //定义温湿度针脚号为2号引脚   
#define PERSON D6
#define FIRE   D7
int c = 0;   
dht11 DHT11; 
int flag = 0;   
/*配置WIFI名和密码*/
const char * WIFI_SSID     = "";
const char * WIFI_PASSWORD = "";

StaticJsonBuffer<1024> jsonBuffer;
StaticJsonBuffer<1024> js;
JsonObject& root = jsonBuffer.createObject();
JsonObject& rt = js.createObject();
JsonObject& properties = root.createNestedObject("properties");
JsonObject& pt = rt.createNestedObject("properties");
char buf[248];
char alarmBuf[248];
char pBuf[4];
char fBuf[4];

/*配置域名和端口号*/

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

String XN_SERVER = "";
uint16_t PORT =  ;
String ProductId   = "";  
String ProductKey  = "";
String DeviceId    = "";
String DeviceKey   = "";
String DeviceName  = "testDevice01";
String ClientId    = "testDevice01";
/*需要上报和订阅的两个TOPIC*/
const char * subTopic = "/raspberry/testDevice01/properties/read";//****set
const char * pubTopic = "/raspberry/testDevice01/properties/report";//******post

DFRobot_XianNing myMQTT;
WiFiClient espClient;
PubSubClient client(espClient);

/*需要操作的产品标识符*/
String Identifier = "";


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

void checkPerson(){
  static int per = 0;
  int val = digitalRead(PERSON);
  if (val == 1){
    delay(50);
    if (val == 1){
      c = 1;                            //反转c值
      strcpy(pBuf,"ON");
      Serial.println("have people");  
    }
 }
  else if (c==1 && val==0){
    c = 0;
    strcpy(pBuf,"OFF");
    Serial.println("people left");      //当人离开的时候打印
  }
   else if(c==0 &&val == 0){
    strcpy(pBuf,"OFF");
    Serial.println("No people"); 
  }

  if(val != per)
    flag = 1;

  per = val;
}
void checkFire(){

  static int per = 0;
  //delay(200);
  int val = digitalRead(FIRE);
  if (val == 0){                    
      strcpy(fBuf,"ON");
      Serial.println("Have fire");  
  }
  else if (val == 1){
    strcpy(fBuf,"OFF");
    Serial.println("NO fire");      
  }
  if(val != per){
    flag = 1;
    Serial.println("fired");
  }
  per = val;
}

void callback(char * topic, byte * payload, unsigned int len){
  Serial.println("esp32 messge callback");
}

void ConnectXnIot(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    /*根据自动计算的用户名和密码连接到Alinyun的设备，不需要更改*/
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

  pinMode(DHT11PIN,INPUT);  
  pinMode(PERSON,INPUT);
  pinMode(FIRE,INPUT);
  Serial.flush();  
  /*连接WIFI*/
  connectWiFi();
  /*初始化MQTT的配置，可自动计算用户名和密码*/
  myMQTT.init(XN_SERVER,ProductId,ProductKey,ClientId,DeviceName,DeviceId,DeviceKey);
  client.setServer(myMQTT.mqtt_server,PORT);
  
  /*设置回调函数，当收到订阅信息时会执行回调函数*/
  client.setCallback(callback);
  /*连接到咸宁市工业物联网*/
  ConnectXnIot();
}

void loop(){
  if(!client.connected()){
    ConnectXnIot();
  }
  flag = 0;
  //将湿度值赋给hum
  int chk = DHT11.read(DHT11PIN);                 //将读取到的值赋给chk
  int tem=(float)DHT11.temperature;               //将温度值赋值给tem
  int hum=(float)DHT11.humidity; 
  checkPerson();
  checkFire();
  client.loop();
  
  properties["esp32_temperature"] = tem;
  properties["esp32_humidity"]    = hum;
  
  pt["alarmPerson"] = pBuf;
  pt["alarmFire"]   = fBuf;

  rt.prettyPrintTo(alarmBuf);
  root.prettyPrintTo(buf);

  Serial.println(buf);
  Serial.println(alarmBuf);
  
  client.publish(pubTopic,buf);

  /*if(flag == 1){
    Serial.println(alarmBuf);
    client.publish(pubTopic,alarmBuf);
  }*/
  
  delay(2000);
}
