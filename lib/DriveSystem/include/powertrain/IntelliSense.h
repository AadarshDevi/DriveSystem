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
    InertialUnit inertialUnit;
    WheelEncoder wheelEncoder;

    void run();
};

#endif //DRIVESYSTEM_DRIVESYSTEM_H
