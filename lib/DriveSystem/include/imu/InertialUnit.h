//
// Created by CryosArtic on 7/30/2026.
//

#ifndef DRIVESYSTEM_INERTIALUNIT_H
#define DRIVESYSTEM_INERTIALUNIT_H

#include <cstdint>

#include "MPU6050_6Axis_MotionApps612.h"

/*
 Object to store the orientation
 */
typedef struct {
    float roll;
    float pitch;
    float yaw;
} Orientation_t;

class InertialUnit {
public:
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
    bool ready() const;

    void setBaseOrientation();

    bool is_dmp_ready() const;

    MPU6050 get_mpu() const;

    Orientation_t get_base_orientation() const;

    uint8_t get_fifo_buffer() const;

    uint8_t *get_buffer();

private:
    MPU6050 mpu;

    Orientation_t base_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};
    Orientation_t current_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};

    bool dmp_ready = false; // on board computer to do math for orientation
    int packet_size = 0;
    uint8_t fifo_buffer[64]{};

    Quaternion quaternion;
    VectorFloat gravity;
    float ypr[3] = {0.0f, 0.0f, 0.0f};

    bool imu_usable = false;
};


#endif //DRIVESYSTEM_INERTIALUNIT_H
