//
// Created by CryosArtic on 6/27/2026.
//

#ifndef DRIVESYSTEM_DRIVESYSTEM_H
#define DRIVESYSTEM_DRIVESYSTEM_H
#include "sensor/WheelEncoder.h"
#include "sensor/InertialUnit.h"

class IntelliSense {
    InertialUnit inertialUnit;
    WheelEncoder wheelEncoder;

    void run();
};

#endif //DRIVESYSTEM_DRIVESYSTEM_H
