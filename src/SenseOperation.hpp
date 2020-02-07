#ifndef SenseOperation_hpp
#define SenseOperation_hpp

#include "Arduino.h"
#include "SenseData.hpp"
#include "ESP8266WiFi.h"
#include "AsyncMqttClient.h"


enum SenseOperationState{
    ST_SENSE_TURNED_ON=0,
    ST_SENSE_WIFI_CONNECTED,
    ST_SENSE_MQTT_CONNECTED
};

class SenseOperation
{
private:
    SenseOperationState state;
    AsyncMqttClient mqtt;
    SenseData senseData;
    bool wifiConnect();
    bool mqttConnect();
    void liveTask();
public:
    SenseOperation(/* args */);
    int run();
};

#endif