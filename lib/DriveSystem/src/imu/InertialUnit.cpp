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

void InertialUnit::stabilize() {
    // Stabilize MPU605
    Serial.println("[Before] Stabilizing MPU6050");
    Orientation_t previous = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};
    Orientation_t current = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};
    constexpr int MAX_SAMPLES = 80;
    int samples = 0;

    vTaskDelay(pdMS_TO_TICKS(10000)); // 3 sec delay to stabilize mpu before setting up stable orientation
    while (samples < MAX_SAMPLES) {
        if (!mpu.dmpGetCurrentFIFOPacket(fifo_buffer)) {
            vTaskDelay(pdMS_TO_TICKS(2));
            continue;
        }

        current = getOrientation();

        if (
            fabs(current.roll - previous.roll) >= 0.02f ||
            fabs(current.pitch - previous.pitch) >= 0.02f ||
            fabs(current.yaw - previous.yaw) >= 0.02f
        ) {
            samples = 0;
            previous = current;
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        samples++;
        previous = current;
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    base_orientation = getOrientation();
    Serial.println("[After] MPU6050 Stabilized");
}
