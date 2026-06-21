//
// Created by CryosArtic on 6/20/2026.
//

#ifndef DRIVESYSTEM_MOTOR_H
#define DRIVESYSTEM_MOTOR_H

class RawMotor {

private:

    // pins
    int pinPWM; // pwm pin
    int in1Pin; // in 1 pin
    int in2Pin; // in 2 pin

    // pin status
    int in1Level; // high / low
    int in2Level; // high / low

    // motor
    bool reverseMotor = false; // reverse motor
    bool running = false; // motor running?
    bool shutdownMotor = false; // shutdown motor if exceeding maxPower

    // motor power
    int power; // power of motor
    int minPower; // max power for motor
    int maxPower; // max power for motor

    // debug
    bool canDebug = false; // tells if debug is available

public:

    // constructor
    RawMotor(int new_pinPWM, int new_in1Pin, int new_in2Pin, int new_minPower, int new_maxPower);

    void reverse(); // reverse the direction of the motor
    bool isReverse() { return reverseMotor; } // asking if the motor is reversed
    void forward(); // regular motor rotation direction

    void setMaxPower(int new_maxPower); // set max motor power
    int getMaxPower() { return maxPower; } // get max motor power

    void setMinPower(int new_minPower); // set min motor power
    int getMinPower() { return minPower; } // get min motor power

    void setPower(int new_power); // set motor power
    int getPower() { return power; } // get motor power

    void start(); // enable motor to run
    void stop(); // disaable motor to run
    bool isRunning() { return running; } // checks if motor is running
    void run(); // actually turning the motor
    void shutdown(bool shutdown); // when power > maxPower, if true, power = 0 else power stays = 255
    void powerDown();

    void debug(bool new_canDebug); // debug statements in serial monitor
    void monitor(int new_power); // debug statements in serial monitor
    void info(); // print all the info of the motor like pins, bools, etc,.

};

#endif //DRIVESYSTEM_MOTOR_H
