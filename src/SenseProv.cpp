#include "Arduino.h"
#include "SenseProv.hpp"

SenseProv::SenseProv():server(80)
{
    formRecieved = false;
    serverStarted = false;
}

void SenseProv::run()
{
    switch (state)
    {
    case ST_PROV_TURNED_ON:
        Serial.println("\n\nProvisionament Started");
        senseData.load();
        startServer();
        Serial.println("Data loaded.");
        Serial.println("Server Started.");
        state = ST_PROV_SERVER_STARTED;
        break;
    case ST_PROV_SERVER_STARTED:
        processDns();
        if(formRecieved){
            formRecieved = false;
            Serial.println("Form Recieved.");
            state = ST_PROV_FORM_RECIEVED;
        }
        if(millis() > 100*1000)
            ESP.reset();
        break;
    case ST_PROV_FORM_RECIEVED:
        stopAP();
        Serial.println("Acess Point Closed.");
        if(testWiFiCredentials()){
            state = ST_PROV_WIFI_ACCEPTED;
            Serial.println("WiFi Accepted.");
        }
        else{
            state = ST_PROV_TURNED_ON;
            Serial.printf("WiFi Error. Ssid: %s, Pass: %s", senseData.wifiData.ssid.c_str(), senseData.wifiData.pass.c_str());
        }
        break;
    case ST_PROV_WIFI_ACCEPTED:
        if(testMqttConnection()){
            state = ST_PROV_MQTT_ACCEPTED;
            Serial.println("Mqtt Accepted.");
        }
        else{
            state = ST_PROV_TURNED_ON;
            Serial.println("Mqtt Error.");
        }
        break;
    case ST_PROV_MQTT_ACCEPTED:
        Serial.println("Provisionament Complete.");
        senseData.settings.provisioned = true;
        senseData.save();
        delay(1000);
        ESP.reset();
    default:
        break;
    }
}

void SenseProv::startServer()
{
    WiFi.softAP("IoT Sense CFG");
    if(!serverStarted){
        dnsServer.start(53, "iotsense.com", WiFi.softAPIP());
        setFilter(ON_AP_FILTER);
        server.addHandler(this);
        server.begin();
        serverStarted = true;
    }
}

bool SenseProv::canHandle(AsyncWebServerRequest *request)
{
    return true;
}

void SenseProv::processDns()
{
    dnsServer.processNextRequest();
}

void SenseProv::handleRequest(AsyncWebServerRequest *request)
{
    Serial.printf("Request recieved: %s\n", request->url().c_str());

    if (request->hasParam("name"))
        senseData.settings.name = request->getParam("name")->value();
    if (request->hasParam("localName"))
        senseData.settings.localName = request->getParam("localName")->value();
    if (request->hasParam("interval"))
        senseData.settings.interval = request->getParam("interval")->value().toInt();
    if (request->hasParam("provisioned"))
        senseData.settings.provisioned = request->getParam("provisioned")->value();
    if (request->hasParam("live_enabled"))
        senseData.services.live.enabled = request->getParam("live_enabled")->value();
    if (request->hasParam("live_interval"))
        senseData.services.live.interval = request->getParam("live_interval")->value().toInt();
    if (request->hasParam("ssid"))
        senseData.wifiData.ssid = request->getParam("ssid")->value();
    if (request->hasParam("pass"))
        senseData.wifiData.pass = request->getParam("pass")->value();
    if (request->hasParam("host"))
        senseData.mqttData.host = request->getParam("host")->value();
    if (request->hasParam("port"))
        senseData.mqttData.port = request->getParam("port")->value().toInt();
    if (request->hasParam("qos"))
        senseData.mqttData.qos = request->getParam("qos")->value().toInt();
    if (request->hasParam("senseToBrkTopic"))
        senseData.mqttData.senseToBrkTopic = request->getParam("senseToBrkTopic")->value();
    if (request->hasParam("bkrToSenseTopic"))
        senseData.mqttData.bkrToSenseTopic = request->getParam("bkrToSenseTopic")->value();
    if (request->hasParam("cfgTopic"))
        senseData.mqttData.cfgTopic = request->getParam("cfgTopic")->value();
    
    if(request->params() == 0){
        request->send(SPIFFS, "/provisioning/prov.html", String(), false);
        formRecieved = false;
        return;
    }

    request->send(SPIFFS, "/provisioning/load.html", String(), false);
    delay(5000);
    formRecieved = true;
}

void SenseProv::stopAP()
{
    WiFi.softAPdisconnect(true);
    WiFi.disconnect(true);
}

bool SenseProv::testWiFiCredentials()
{
    WiFi.begin(senseData.wifiData.ssid, senseData.wifiData.pass);
    int cont = 1;
    while(!WiFi.isConnected()){
        cont++;
        delay(1000);
        if(cont>=10){
            return(0);
        }
    }
    return(1);
}

bool SenseProv::testMqttConnection()
{
    mqtt.setServer(senseData.mqttData.host.c_str(), senseData.mqttData.port);
    mqtt.setClientId(senseData.settings.name.c_str());
    mqtt.connect();
    int cont = 1;
    while(!mqtt.connected()){
        cont++;
        delay(1000);
        if(cont>=10){
            return(0);
        }
    }
    return(1);
}