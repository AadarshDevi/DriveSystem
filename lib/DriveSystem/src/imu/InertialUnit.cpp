//
// Created by CryosArtic on 7/30/2026.
//

#include "../../include/imu/InertialUnit.h"

#include <HardwareSerial.h>
#include <Wire.h>

#include "MPU6050_6Axis_MotionApps612.h"

MPU6050 mpu;

void InertialUnit::reset() {
    Serial.println("[Before] Resetting MPU6050");
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    Serial.println("[After] MPU6050 Reset");
}
