//
// Created by CryosArtic on 7/30/2026.
//

#ifndef DRIVESYSTEM_SYSTEMMODE_H
#define DRIVESYSTEM_SYSTEMMODE_H

/*
 SystemMode tells what type of control & connection the main computer will have on the powertrain
 */
enum SystemMode {
    /*
     This allows the Powertrain to be independent from the Main Computer
     */
    AUTONOMOUS,

    /*
     The Powertrain is handicapped by the IMU or Wheel encoder. Because of that, the Powertrain is dependent on the main computer
     */
    MANUAL
};


#endif //DRIVESYSTEM_SYSTEMMODE_H
