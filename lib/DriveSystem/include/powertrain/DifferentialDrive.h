//
// Created by CryosArtic on 6/29/2026.
//

#ifndef DRIVESYSTEM_DIFFERENTIALROVERDRIVE_H
#define DRIVESYSTEM_DIFFERENTIALROVERDRIVE_H

#include <vector>

#include "motor/RawMotor.h"

class DifferentialDrive {
    std::vector<RawMotor> motors;

public:
    DifferentialDrive(
        int new_wheelCount,
        std::vector<std::vector<int> > new_motorPins,
        std::vector<int> new_powerInput
    );

    void debugPower(int powerInput) const;

    void setPower(int new_power) const;

    void setPower(int new_motorNum, int new_power);

    void shutdownMotors();

    void shutdownMotor(int motorNum) { motors[motorNum].shutdown(); }

    void debug(bool new_debug);

    void run();
};


#endif //DRIVESYSTEM_DIFFERENTIALROVERDRIVE_H
