//
// Created by CryosArtic on 6/27/2026.
//

#ifndef DRIVESYSTEM_INTELLISENSE_H
#define DRIVESYSTEM_INTELLISENSE_H

#include "comms/InputManager.h"
#include "DifferentialDrive.h"
#include "sensor/WheelEncoder.h"
#include "sensor/InertialUnit.h"


class IntelliSense {
private:
    InertialUnit &inertialUnit;
    WheelEncoder &wheelEncoder;
    InputManager &inputManager;
    DifferentialDrive &differentialDrive;

public:
    IntelliSense(
        InertialUnit &inertialUnit,
        WheelEncoder &wheelEncoder,
        InputManager &inputManager,
        DifferentialDrive &differentialDrive
    );

    void run();
};

#endif //DRIVESYSTEM_DRIVESYSTEM_H
