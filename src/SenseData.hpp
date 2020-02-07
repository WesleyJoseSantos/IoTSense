#ifndef SenseData_hpp
#define SenseData_hpp

#include "FS.h"
#include "ArduinoJson.h"

struct seetings_t{
  String name;
  String localName;
  int interval;
  bool provisioned;
};

struct live_service_t{
  bool enabled;
  uint16_t interval;
};

struct services_t{
  live_service_t live;
};

struct wifiData_t{
  String ssid;
  String pass;
};

struct mqttData_t{
  String host;
  int port;
  int qos;
  char clientId[32];
  String senseToBrkTopic;
  String bkrToSenseTopic;
  String cfgTopic;
};

class SenseData
{
private:
    String fileName;
public:
    SenseData();
    seetings_t settings;
    services_t services;
    wifiData_t wifiData;
    mqttData_t mqttData;
    void load();
    void save();
    void print();
};

#endif