//
// Created by CryosArtic on 7/30/2026.
//

#ifndef DRIVESYSTEM_DRIVEMODE_H
#define DRIVESYSTEM_DRIVEMODE_H


enum DriveMode {
    /*
     Powertrain goes to the target point then orientates itself to go to the next point
     */
    SINGLE_TARGET,

    /*
     Powertrain uses the given target points to plan out a route and calculate power, distance, and orientation
     */
    MULTI_TARGET
};


#endif //DRIVESYSTEM_DRIVEMODE_H
