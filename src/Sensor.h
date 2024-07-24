#pragma once
#include <Arduino.h>
#include <VL53L1X.h>

class Sensor
{
public:
    Sensor();
    void init();
    int getDistance();

private:
    VL53L1X _tof;
    int _last_distance = -1;
};