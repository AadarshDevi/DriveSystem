//
// Created by CryosArtic on 7/30/2026.
//

#ifndef DRIVESYSTEM_INERTIALUNIT_H
#define DRIVESYSTEM_INERTIALUNIT_H


class InertialUnit {
public:
    /*
     Object to store the orientation
     */
    typedef struct {
        float roll;
        float pitch;
        float yaw;
    } Orientation_t;

    /*
     Constructor
     */
    InertialUnit(int i2c_scl_pin, int i2c_sda_pin);

    /*
     Reset IMU Offsets
     */
    void reset();

    /*
     Calibrate IMU
     */
    void calibrate(int loops);

    /*
     Initialize DMP
     */
    void dmp();

    /*
     Stabilize IMU Readings
     */
    void stabilize();

    /*
     Gets Orientation
     */
    Orientation_t getOrientation();

    /*
     Run the IMU to monitor orientation
     */
    void run();

    /*
    Checks if the IMU can be used
    */
    bool ready();

    void setBaseOrientation();
    Orientation_t base_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};
    Orientation_t current_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};
};


#endif //DRIVESYSTEM_INERTIALUNIT_H
