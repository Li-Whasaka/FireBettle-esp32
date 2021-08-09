#ifndef __DFROBOT_XIANNING__
#define __DFROBOT_XIANNING__

#include "Arduino.h"

class DFRobot_XianNing{
  public:
    DFRobot_XianNing(void);
    ~DFRobot_XianNing(void);
    void init(String XnServer, String ProductId ,String XnProductKey, 
              String XnClientId, String XnDeviceName, String DeviceId,
              String XnDeviceSecret, uint16_t XnPort = 1883);
    void setConfig();
    String XnIot_SERVER;
    String ProductId;
    String ProductKey;
    String ClientId;
    String DeviceName;
    String DeviceId;
    String DeviceSecret;

    uint16_t Port;
    char * mqtt_server;
    char * client_id;
    char * username;
    char * password;
  private:
    
};
int Compute_file_md5(char *md5_str);
#endif