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
    pinPWM = new_pinPWM;
    pinIN1 = new_pinIN1;
    pinIN2 = new_pinIN2;

    pinIN1Enabled = new_pinIN1Enabled;
    pinIN2Enabled = new_pinIN2Enabled;

    pinMode(pinPWM, OUTPUT);
    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);

    reverseRotation = new_reverseRotation;
    rotationEnabled = new_rotationEnabled;

    power = new_power;
    absMinPower = new_absMinPower;
    absMaxPower = new_absMaxPower;
    shutdownPower = new_shutdownPower;

    enableDebug = new_showDebug;
    forward();
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

void RawMotor::setMotorPins(uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2) {
    if (
        new_pinIN1 == new_pinIN2 ||
        new_pinPWM == new_pinIN1 ||
        new_pinPWM == new_pinIN2) {
        return;
    }
    pinPWM = new_pinPWM;
    pinIN1 = new_pinIN1;
    pinIN2 = new_pinIN2;
}

void RawMotor::setPower(int new_power) {
    if (new_power < 0 || new_power < absMinPower) {
        power = absMinPower;
        return;
    }

    // makes sure power doesn't exceed max limit
    if (new_power > absMaxPower) {
        power = absMaxPower;
        return;
    }

    power = new_power;
}

void RawMotor::setAbsMinPower(int new_absMinPower) {
    if (new_absMinPower > absMaxPower) return;
    absMinPower = new_absMinPower;
    power = (power < absMinPower) ? absMinPower : power;
}

void RawMotor::setAbsMaxPower(int new_absMaxPower) {
    if (new_absMaxPower < absMinPower) return;
    absMaxPower = new_absMaxPower;
    power = (power > absMaxPower) ? absMaxPower : power;
}

void RawMotor::setShutdownPower(int new_shutdownPower) {
    shutdownPower = new_shutdownPower;
}

void RawMotor::reverse() {
    reverseRotation = true;

    pinIN1Enabled = false;
    pinIN2Enabled = true;

    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
}

void RawMotor::forward() {
    reverseRotation = false;

    pinIN1Enabled = true;
    pinIN2Enabled = false;

    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinIN2, LOW);
}


void RawMotor::run() {
    if (!rotationEnabled) return;
    analogWrite(pinPWM, power);
}

void RawMotor::shutdown() {
    while (power != shutdownPower) {
        power = (power < (absMaxPower * 0.05)) ? shutdownPower : power * 0.75;
        analogWrite(pinPWM, power);
        debugAllPower(0);
        delay(200);
    }
}

void RawMotor::info() {
    if (!enableDebug) return;
    debugPins();
    debugMovement();
    debugPower();
}

void RawMotor::debugAllPower(int inputPower) {
    if (!enableDebug) return;
    Serial.printf("Input:%d Motor:%d Min:%d Max:%d\r\n", inputPower, power, absMinPower, absMaxPower);
}

void RawMotor::debugPower() {
    if (!enableDebug) return;
    Serial.printf("Power:%d Min:%d Max:%d\r\n", power, absMinPower, absMaxPower);
}

void RawMotor::debugPins() {
    if (!enableDebug) return;
    Serial.printf("PWM:%d IN1:%d IN2:%d\r\n", pinPWM, pinIN1, pinIN2);
}

void RawMotor::debugMovement() {
    if (!enableDebug) return;
    Serial.printf("Rotation:%d Reverse:%d\r\n", rotationEnabled, reverseRotation);
}
