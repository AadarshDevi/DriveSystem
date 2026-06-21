//
// Created by CryosArtic on 6/20/2026.
//

#include "RawMotor.h"

RawMotor::RawMotor(uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2,
                   bool new_pinIN1Enabled, bool new_pinIN2Enabled,
                   bool new_reverseRotation, bool new_rotationEnabled,
                   int new_power, int new_absMinPower, int new_absMaxPower, int new_shutdownPower,
                   bool new_showDebug
) {
    this->pinPWM = new_pinPWM;
    this->pinIN1 = new_pinIN1;
    this->pinIN2 = new_pinIN2;

    this->pinIN1Enabled = new_pinIN1Enabled;
    this->pinIN2Enabled = new_pinIN2Enabled;

    this->reverseRotation = new_reverseRotation;
    this->rotationEnabled = new_rotationEnabled;

    this->power = new_power;
    this->absMinPower = new_absMinPower;
    this->absMaxPower = new_absMaxPower;
    this->shutdownPower = new_shutdownPower;

    this->enableDebug = new_showDebug;
}

RawMotor::RawMotor(
    uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2,
    int new_absMinPower, int new_absMaxPower
) : RawMotor(
    new_pinPWM, new_pinIN1, new_pinIN2,
    false, false,
    false, false,
    new_absMinPower, new_absMinPower, new_absMaxPower, new_absMinPower,
    false
) {
}

RawMotor::RawMotor(
    uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2,
    int new_power, int new_absMinPower, int new_absMaxPower, int new_shutdownPower
) : RawMotor(
    new_pinPWM, new_pinIN1, new_pinIN2,
    false, false,
    false, false,
    new_power, new_absMinPower, new_absMaxPower, new_shutdownPower,
    false
) {
}

RawMotor::RawMotor(
    uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2,
    int new_power, int new_absMinPower, int new_absMaxPower, int new_shutdownPower,
    bool new_reverseRotation
) : RawMotor(
    new_pinPWM, new_pinIN1, new_pinIN2,
    false, false,
    new_reverseRotation, false,
    new_power, new_absMinPower, new_absMaxPower, new_shutdownPower,
    false
) {
}


