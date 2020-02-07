#ifndef IoTSense_h
#define IoTSense_h

#include "Arduino.h"
#include "SenseData.hpp"
#include "SenseProv.hpp"
#include "SenseOperation.hpp"

class IoTSense
{
private:
    SenseProv prov;
    SenseOperation operation;
    int err;
public:
    IoTSense();
    void begin();
    void run();
    SenseData senseData;
};

#endif