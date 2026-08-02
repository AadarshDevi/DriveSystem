//
// Created by CryosArtic on 7/30/2026.
//

#ifndef DRIVESYSTEM_DRIVEMODE_H
#define DRIVESYSTEM_DRIVEMODE_H


enum DriveMode {
    /*
     Powertrain goes to the target point then orientates itself to go to the next point
     */
    FOCUSED,

    /*
     Powertrain uses the given target points to plan out a route and calculate power, distance, and orientation
     */
    INTELLI_SENSE
};


#endif //DRIVESYSTEM_DRIVEMODE_H
