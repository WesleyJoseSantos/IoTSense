#include "Arduino.h"
#include "IoTSense.h"
#include "SenseData.hpp"
#include "SenseProv.hpp"
#include "SenseOperation.hpp"

IoTSense::IoTSense()
{

}

void IoTSense::begin()
{
    SPIFFS.begin();
    senseData.load();
}

void IoTSense::run()
{
    if(!senseData.settings.provisioned || err){
        prov.run();
    }
    else
    {
        err = operation.run();
    }
}