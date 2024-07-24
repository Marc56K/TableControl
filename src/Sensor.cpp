#include "Sensor.h"
#include <Wire.h>

Sensor::Sensor()
{
}

void Sensor::init()
{
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C

    _tof.setTimeout(500);
    if (!_tof.init())
    {
        Serial.println(F("Failed to detect and initialize sensor!"));
        while (true);
    }

    // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
    // You can change these settings to adjust the performance of the sensor, but
    // the minimum timing budget is 20 ms for short distance mode and 33 ms for
    // medium and long distance modes. See the VL53L1X datasheet for more
    // information on range and timing limits.
    _tof.setDistanceMode(VL53L1X::Long);
    _tof.setMeasurementTimingBudget(50000);

    // Start continuous readings at a rate of one measurement every 50 ms (the
    // inter-measurement period). This period should be at least as long as the
    // timing budget.
    _tof.startContinuous(50);
}

int Sensor::getDistance()
{
    auto new_distance = _tof.readRangeContinuousMillimeters();
    if (!_tof.timeoutOccurred())
    {
      if (_last_distance < 0)
        _last_distance = new_distance;
      else
        _last_distance = 0.75f * _last_distance + 0.25f * new_distance;
    }
    return _last_distance;
}