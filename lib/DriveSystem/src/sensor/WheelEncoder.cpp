//
// Created by CryosArtic on 8/3/2026.
//

#include "../../include/sensor/WheelEncoder.h"
#include <Arduino.h>
#include <esp32-hal-gpio.h>
#include <mutex>

WheelEncoder::WheelEncoder(const uint8_t wheel_encoder_pin)
    : wheel_encoder_pin(wheel_encoder_pin) {
    pinMode(wheel_encoder_pin, INPUT);
}

void WheelEncoder::run() {
    for (;;) {
        current_state = digitalRead(wheel_encoder_pin);

        if (current_state == HIGH && previous_state == LOW) {
            distanceTravelled += slit_dist;
        }
        previous_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void WheelEncoder::setCalculateDistance(bool calculate) {
    calcDistance = calculate;
}


bool WheelEncoder::isCalculatingDistance() const {
    return calcDistance;
}

volatile float WheelEncoder::getDistanceTravelled() const {
    return distanceTravelled;
}

void WheelEncoder::resetDistance() {
    distanceTravelled = 0.0f;
}
