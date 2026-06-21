//
// Created by CryosArtic on 6/20/2026.
//

#ifndef RAWMOTOR_H
#define RAWMOTOR_H

#include <Arduino.h>

class RawMotor {
private:
    uint8_t pinPWM;
    uint8_t pinIN1;
    uint8_t pinIN2;

    bool pinIN1Enabled;
    bool pinIN2Enabled;

    bool reverseRotation;
    bool rotationEnabled;

    int power;
    int absMinPower;
    int absMaxPower;
    int shutdownPower;

    bool enableDebug;

public:
    /**
     * All Args Constructor
     * @param new_pinPWM the PWM pin for the motor
     * @param new_pinIN1 the input 1 pin of the motor
     * @param new_pinIN2 the input 2 pin of the motor
     * @param new_pinIN1Enabled help keep track of level (HIGH/LOW) of new_pinIN1
     * @param new_pinIN2Enabled help keep track of level (HIGH/LOW) of new_pinIN2
     * @param new_reverseRotation enables the motor to rotate in the opposite direction
     * @param new_rotationEnabled enables the motor to rotate
     * @param new_power the current power of the motor
     * @param new_absMinPower the absolute min power the motor can use
     * @param new_absMaxPower the absolute max power the motor can use
     * @param new_shutdownPower the safe power level that is used when something with the motor goes wrong
     * @param new_showDebug enables the class methods to print debug info in the Serial Monitor
     */
    RawMotor(
        uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2,
        bool new_pinIN1Enabled, bool new_pinIN2Enabled,
        bool new_reverseRotation, bool new_rotationEnabled,
        int new_power, int new_absMinPower, int new_absMaxPower, int new_shutdownPower,
        bool new_showDebug
    );

    /**
     * A Motor class as close to the hardware with bare minimum essentials
     * @param new_pinPWM the PWM pin for the motor
     * @param new_pinIN1 the input 1 pin of the motor
     * @param new_pinIN2 the input 2 pin of the motor
     * @param new_absMinPower the absolute min power the motor can use
     * @param new_absMaxPower the absolute max power the motor can use
     */
    RawMotor(
        uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2,
        int new_absMinPower, int new_absMaxPower
    );

    /**
     * A Motor class as close to the hardware with bare minimum essentials
     * @param new_pinPWM the PWM pin for the motor
     * @param new_pinIN1 the input 1 pin of the motor
     * @param new_pinIN2 the input 2 pin of the motor
     * @param new_power the current power of the motor
     * @param new_absMinPower the absolute min power the motor can use
     * @param new_absMaxPower the absolute max power the motor can use
     * @param new_shutdownPower the safe power level that is used when something with the motor goes wrong
     */
    RawMotor(
        uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2,
        int new_power, int new_absMinPower, int new_absMaxPower, int new_shutdownPower
    );

    /**
     * A Motor class as close to the hardware with bare minimum essentials
     * @param new_pinPWM the PWM pin for the motor
     * @param new_pinIN1 the input 1 pin of the motor
     * @param new_pinIN2 the input 2 pin of the motor
     * @param new_power the current power of the motor
     * @param new_absMinPower the absolute min power the motor can use
     * @param new_absMaxPower the absolute max power the motor can use
     * @param new_shutdownPower the safe power level that is used when something with the motor goes wrong
     * @param new_reverseRotation enables the motor to rotate in the opposite direction
     */
    RawMotor(
        uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2,
        int new_power, int new_absMinPower, int new_absMaxPower, int new_shutdownPower,
        bool new_reverseRotation
    );

    /**
     * Set all the motor pins at the same time
     * @param new_pinPWM the PWM pin for the motor
     * @param new_pinIN1 the input 1 pin of the motor
     * @param new_pinIN2 the input 2 pin of the motor
     */
    void setMotorPins(uint8_t new_pinPWM, uint8_t new_pinIN1, uint8_t new_pinIN2);

    /**
     * set the PWM pin of the motor
     * @param new_pinPWM the PWM pin for the motor
     */
    void setPinPWM(uint8_t new_pinPWM) { setMotorPins(new_pinPWM, pinIN1, pinIN2); }

    /**
     * set IN1 pin of the motor
     * @param new_pinIN1 the input 1 pin of the motor
     */
    void setPinIN1(uint8_t new_pinIN1) { setMotorPins(pinPWM, new_pinIN1, pinIN2); }

    /**
     * set IN2 pin of the motor
     * @param new_pinIN2 the input 2 pin of the motor
     */
    void setPinIN2(uint8_t new_pinIN2) { setMotorPins(pinPWM, pinIN1, new_pinIN2); }

    /**
     * get the motor pin for PWM
     * @return PWM pin
     */
    uint8_t getPinPWM() { return pinPWM; }

    /**
     * get the motor pin for IN1
     * @return IN1 pin
     */
    uint8_t getPinIN1() { return pinIN1; }

    /**
     * get the motor pin for IN2
     * @return IN2 pin
     */
    uint8_t getPinIN2() { return pinIN2; }

    /**
     * checks if the motor IN1 pin is HIGH
     * @return IN1 pin HIGH
     */
    bool isPinIN1Enabled() { return pinIN1Enabled; }

    /**
     * checks if the motor IN2 pin is HIGH
     * @return IN2 pin HIGH
     */
    bool isPinIN2Enabled() { return pinIN2Enabled; }

    /**
     * reverses the rotation direction of the motor
     */
    void reverse() { reverseRotation = true; }

    /**
     * the normal rotation direction of the motor
     */
    void forward() { reverseRotation = false; }

    /**
     * checks if the motor is rotating in the opposite direction
     * @return rotation reversed
     */
    bool isReverse() { return reverseRotation; }

    /**
     * enables the motor to be able to rotate
     */
    void start() { rotationEnabled = true; }

    /**
     * disables the motor from being able to rotate
     */
    void stop() { rotationEnabled = false; }

    /**
     * checks if the motor can turn
     * @return motor can turn
     */
    bool isRotationEnabled() { return rotationEnabled; }

    /**
     * set the power of the motor
     * @param new_power the power of the motor
     */
    void setPower(int new_power);

    /**
     * set the absolute minimum power the motor can use
     * @param new_absMinPower the absolute min power the motor can use
     */
    void setAbsMinPower(int new_absMinPower);

    /**
     * set the absolute maximum power of motor
     * @param new_absMaxPower the absolute max power the motor can use
     */
    void setAbsMaxPower(int new_absMaxPower);

    /**
     * set the shutdown power for the motor
     * @param new_shutdownPower the safe power level that is used when something with the motor goes wrong
     */
    void setShutdownPower(int new_shutdownPower);

    /**
     * get the power of the motor
     * @return motor power
     */
    int getPower() { return power; }

    /**
     * get the absolute min power of the motor
     * @return motor min power
     */
    int getAbsMinPower() { return absMinPower; }

    /**
     * get the absolute max power of the motor
     * @return motor max ower
     */
    int getAbsMaxPower() { return absMaxPower; }

    /**
     * get the shutdown power of the motor
     * @return motor shutdown power
     */
    int getShutdownPower() { return shutdownPower; }

    /**
     * rotating the motor at the power level saved at that time
     */
    void run();

    /**
     * powers down the motor to a safe power level of the motor to stop the motor and reduce deterioration
     */
    void shutdown();

    /**
     * Allow the class to print debug information in the Serial Monitor for debugging purposes
     * @param new_enableDebug enables the class methods to print debug info in the Serial Monitor
     */
    void debug(bool new_enableDebug) { enableDebug = new_enableDebug; }

    /**
     * prints all vars of the obj in the Serial Monitor
     */
    void info();

    /**
     * prints power vars in the Serial Monitor
     */
    void debugAllPower(int inputPower);

    /**
     * prints motor power vars in the Serial Monitor
     */
    void debugPower();

    /**
     * prints motor pins in the Serial Monitor
     */
    void debugPins();

    /**
     * prints rotation vars in the Serial Monitor
     */
    void debugMovement();
};

#endif //RAWMOTOR_H
