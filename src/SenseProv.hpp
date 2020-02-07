#ifndef SenseProv_hpp
#define SenseProv_hpp

#include "Arduino.h"
#include "SenseData.hpp"
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "ESP8266WiFi.h"
#include "AsyncMqttClient.h"

enum SenseProvState{
    ST_PROV_TURNED_ON=0,
    ST_PROV_SERVER_STARTED,
    ST_PROV_FORM_RECIEVED,
    ST_PROV_WIFI_ACCEPTED,
    ST_PROV_MQTT_ACCEPTED,
    ST_PROV_PROVISIONED
};

class SenseProv : private AsyncWebHandler 
{
private:
    DNSServer dnsServer;
    AsyncMqttClient mqtt;
    AsyncWebServer server;
    SenseData senseData;
    SenseProvState state;
    void startServer();
    void stopAP();
    bool canHandle(AsyncWebServerRequest *request);
    void processDns();
    void handleRequest(AsyncWebServerRequest *request);
    bool testWiFiCredentials();
    bool testMqttConnection();
    bool formRecieved;
    bool serverStarted;
public:
    SenseProv();
    void run();
};

#endif