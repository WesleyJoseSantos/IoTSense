#include "SenseData.hpp"

SenseData::SenseData(/* args */)
{

}

void SenseData::load()
{
    File f = SPIFFS.open("/SenseData.json", "r");
    StaticJsonDocument<1024> jsonDoc;
    deserializeJson(jsonDoc, f);
    settings.name = jsonDoc["settings"]["name"].as<String>();
    settings.interval = jsonDoc["settings"]["interval"];
    settings.localName = jsonDoc["settings"]["localName"].as<String>();
    settings.provisioned = jsonDoc["settings"]["provisioned"];
    services.live.enabled = jsonDoc["services"]["live"]["enabled"];
    services.live.interval = jsonDoc["services"]["live"]["interval"];
    wifiData.ssid = jsonDoc["wifiData"]["ssid"].as<String>();
    wifiData.pass = jsonDoc["wifiData"]["pass"].as<String>();
    mqttData.host = jsonDoc["mqttData"]["host"].as<String>();
    mqttData.port = jsonDoc["mqttData"]["port"];
    mqttData.qos = jsonDoc["mqttData"]["qos"];
    mqttData.senseToBrkTopic = jsonDoc["mqttData"]["senseToBrkTopic"].as<String>();
    mqttData.bkrToSenseTopic = jsonDoc["mqttData"]["bkrToSenseTopic"].as<String>();
    mqttData.cfgTopic = jsonDoc["mqttData"]["cfgTopic"].as<String>();
    f.close();
}

void SenseData::save()
{
    StaticJsonDocument<1024> jsonDoc;
    File f = SPIFFS.open("/SenseData.json", "w");
    jsonDoc["settings"]["name"] = settings.name;
    jsonDoc["settings"]["interval"] = settings.interval;
    jsonDoc["settings"]["localName"] = settings.localName;
    jsonDoc["settings"]["provisioned"] = settings.provisioned;
    jsonDoc["services"]["live"]["enabled"] = services.live.enabled;
    jsonDoc["services"]["live"]["interval"] = services.live.interval;
    jsonDoc["wifiData"]["ssid"] = wifiData.ssid;
    jsonDoc["wifiData"]["pass"] = wifiData.pass;
    jsonDoc["mqttData"]["host"] = mqttData.host;
    jsonDoc["mqttData"]["port"] = mqttData.port;
    jsonDoc["mqttData"]["qos"] = mqttData.qos;
    jsonDoc["mqttData"]["senseToBrkTopic"] = mqttData.senseToBrkTopic;
    jsonDoc["mqttData"]["bkrToSenseTopic"] = mqttData.bkrToSenseTopic;
    jsonDoc["mqttData"]["cfgTopic"] = mqttData.cfgTopic;
    serializeJson(jsonDoc, f);
    f.close();
}

void SenseData::print()
{
    File f = SPIFFS.open("/SenseData.json", "r");
    StaticJsonDocument<1024> jsonDoc;
    deserializeJson(jsonDoc, f);
    Serial.print("\nJson: ");
    serializeJson(jsonDoc, Serial);
    Serial.print("\n");

    f.close();
}