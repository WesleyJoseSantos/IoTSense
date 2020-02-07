#include "Arduino.h"
#include "SenseOperation.hpp"

SenseOperation::SenseOperation(/* args */)
{
}

int SenseOperation::run()
{
    switch (state)
    {
    case ST_SENSE_TURNED_ON:
        Serial.println("\n\nOperation Started");
        senseData.load();
        Serial.println("Data Loaded");
        if(wifiConnect())
            state = ST_SENSE_WIFI_CONNECTED;
        else{
            Serial.printf("WiFi Error. Ssid: %s, Pass: %s", senseData.wifiData.ssid.c_str(), senseData.wifiData.pass.c_str());
            return(1);
        }
        break;
    case ST_SENSE_WIFI_CONNECTED:
        Serial.println("WiFi Connected");
        if(mqttConnect()){
            state = ST_SENSE_MQTT_CONNECTED;
            Serial.println("Mqtt Connected");
        }else{
            Serial.println("Mqtt Error");
            return(1);
        }
        break;
    case ST_SENSE_MQTT_CONNECTED:
        if(senseData.services.live.enabled) liveTask();
        break;
    default:
        break;
    }
    return(0);
}

bool SenseOperation::wifiConnect()
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

bool SenseOperation::mqttConnect()
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

void SenseOperation::liveTask()
{
    static unsigned long interval = senseData.services.live.interval;
    StaticJsonDocument<200> payload;
    String strPayload;
    if(millis()>=interval){
        interval += senseData.services.live.interval;
        payload["service"] = "live";
        payload["name"] = senseData.settings.name;
        payload["localName"] = senseData.settings.localName;
        payload["status"] = 1;
        payload["RSSI"] = WiFi.RSSI();
        payload["interval"] = senseData.settings.interval;
        serializeJson(payload, strPayload);
        mqtt.publish(senseData.mqttData.senseToBrkTopic.c_str(), senseData.mqttData.qos, false, strPayload.c_str());
        Serial.println("Data sended to mqtt.");
    }

}