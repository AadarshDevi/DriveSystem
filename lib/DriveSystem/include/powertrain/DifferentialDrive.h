//
// Created by CryosArtic on 6/29/2026.
//

#ifndef DRIVESYSTEM_DIFFERENTIALROVERDRIVE_H
#define DRIVESYSTEM_DIFFERENTIALROVERDRIVE_H

#include <vector>

#include "motor/RawMotor.h"

class DifferentialDrive {
    RawMotor left_omni;
    RawMotor right_omni;
    RawMotor left_mecanum;
    RawMotor right_mecanum;

public:
    DifferentialDrive(
        RawMotor &left_omni, RawMotor &right_omni,
        RawMotor &left_mecanum, RawMotor &right_mecanum
    );

    // void debugPower(int powerInput) const;
    //
    void setPower(int new_power);

    void setPower(int new_left_power, int new_right_power);

    void drive(int direction);

    void rotate(int direction);

    void stop();

    void run() const;

    void enableDrive(bool enableMovement);

    //
    // void setPower(int new_motorNum, int new_power);
    //
    // void shutdownMotors();
    //
    // void shutdownMotor(int motorNum) { motors[motorNum].shutdown(); }
    //
    // void debug(bool new_debug);
    //
    // void run();
};


#endif //DRIVESYSTEM_DIFFERENTIALROVERDRIVE_H
