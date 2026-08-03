//
// Created by CryosArtic on 8/3/2026.
//

#ifndef DRIVESYSTEM_WHEELENCODER_H
#define DRIVESYSTEM_WHEELENCODER_H

#include <cmath>
#include <esp32-hal-gpio.h>

class WheelEncoder {
private:
    const uint8_t wheel_encoder_pin;

    const float diameter = 6.2f;
    const float circumference = M_PI * diameter;

    const float slits_on_encoder_wheel = 20.0f;
    const float slit_dist = circumference / slits_on_encoder_wheel;

    bool calcDistance = false;

    volatile float distanceTravelled = 0;

    unsigned int current_state = LOW;
    unsigned int previous_state = HIGH;

public:
    WheelEncoder(uint8_t wheel_encoder_pin);

    void run();

    void setCalculateDistance(bool calculate);

    bool isCalculatingDistance() const;

    volatile float getDistanceTravelled() const;

    void resetDistance();
};


#endif //DRIVESYSTEM_WHEELENCODER_H
