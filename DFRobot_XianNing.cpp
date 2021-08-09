#ifndef __DFROBOT_XIANNING_H__
#define __DFROBOT_XIANNING_H__
#include "DFRobot_XianNing.h"
//#include "md5.h"
using namespace std;

#define READ_DATA_SIZE  1024
#define MD5_SIZE        16
#define MD5_STR_LEN     (MD5_SIZE*2)

#define sercureId "test"
#define sercureKey "test"

/*int Compute_file_md5(char *md5_str){
    int i;
    int fd;
    int ret;
	char buf[32];
    unsigned char data[READ_DATA_SIZE];
    unsigned char md5_value[MD5_SIZE];
    MD5_CTX md5;
    MD5Init(&md5);
	sprintf(buf,"%s|%s",sercureId,sercureKey);
	memset(data,0,sizeof(data));
	memcpy(data,buf,strlen(buf));
	//strcat(data,buf);
    while (1){
		//data = sercureKey;
		ret = strlen(buf);
        MD5Update(&md5, data, ret);
        printf("str in md5 = %s\n",data);
        if (0 == ret || ret < READ_DATA_SIZE){
            break;
        }
    }
}*/


DFRobot_XianNing :: DFRobot_XianNing(void){
    
}

DFRobot_XianNing :: ~DFRobot_XianNing(void){
    
}

void DFRobot_XianNing :: init(String XnServer, String ProductId ,String XnProductKey, 
              String XnClientId, String XnDeviceName, String DeviceId,
              String XnDeviceSecret, uint16_t Port){
    this->XnIot_SERVER  = XnServer;
    this->ProductId     = ProductId;
    this->ProductKey    = XnProductKey;
    this->ClientId      = XnClientId;
    this->DeviceName    = XnDeviceName;
    this->DeviceId      = DeviceId;
    this->DeviceSecret  = XnDeviceSecret;
    this->Port          = Port;
    setConfig();
}

void DFRobot_XianNing :: setConfig(){
    String tempSERVER = (this->XnIot_SERVER);
    uint8_t len = tempSERVER.length();
    uint16_t timestamp = 49;
    
    //get host
    if(this->mqtt_server == NULL){
        this->mqtt_server = (char *) malloc(len);
    }
    strcpy(this->mqtt_server,tempSERVER.c_str());

    //client id
    String tempID = (this->DeviceName);
    len = tempID.length();
    if(this->client_id == NULL){
        this->client_id = (char *) malloc(len);
    }
    strcpy(this->client_id,tempID.c_str());


    /*int t = time(0);  
	char *user = sercureId;
    char *pwd = (char*)malloc(32);
	Compute_file_md5(pwd);

    if(this->username == NULL){
        this->username = (char *) malloc(32);
    }
    strcpy(this->username,user);

    if(this->password == NULL){
        this->password = (char *) malloc(32);
    }
    strcpy(this->password,pwd);
    Serial.print("userName=");
    Serial.println(this->username);*/
}
#endif

//861193045386225