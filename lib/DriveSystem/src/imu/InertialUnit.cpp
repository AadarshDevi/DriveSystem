//
// Created by CryosArtic on 7/30/2026.
//
#include <cstdint>

#include <HardwareSerial.h>
#include <Wire.h>

#include "../../include/imu/InertialUnit.h"


InertialUnit::InertialUnit(int i2c_scl_pin, int i2c_sda_pin) {
    // Initialize I2C
    Serial.println("[Before] Initializing Wire");
    Wire.begin(i2c_scl_pin, i2c_sda_pin, static_cast<int>(1E5)); // 1E5 = 100,000 or 100kHz to steadily upload code
    Serial.println("[After] Wire Initialized");

    // Initialize MPU6050
    Serial.println("[Before] Initializing MPU6050");
    mpu.initialize();
    if (!mpu.testConnection()) {
        Serial.println("[Error] MPU6050 Connection Failed");
        vTaskDelete(nullptr);
    }
    Serial.println("[After] MPU6050 Initialized");

    reset();
    calibrate(6);
    dmp();

    // DMP is ready to start doing calculations really quickly.
    // so i2c can now be faster
    Wire.setClock(static_cast<int>(4E5)); // 4E5 = 400,000 kHz
    Serial.println("[After] DMP Initialized");

    stabilize();
    imu_usable = true;
}

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

void InertialUnit::calibrate(const int loops = 6) {
    Serial.println("[Before] Calibrating MPU6050");
    mpu.CalibrateAccel(loops);
    mpu.CalibrateGyro(loops);
    mpu.PrintActiveOffsets();
    mpu.setDLPFMode(MPU6050_DLPF_BW_20); // low pass band filter
    Serial.println("[After] MPU6050 Calibrated");
}

void InertialUnit::dmp() {
    // Initialize Onboard DMP
    Serial.println("[Before] Initializing DMP");
    const int dev_status = mpu.dmpInitialize();
    if (dev_status != 0) {
        Serial.println("[Error] DMP Initialization Failed");
        vTaskDelete(nullptr);
    }
    mpu.setDMPEnabled(true); // enable mpu's dmp
    packet_size = mpu.dmpGetFIFOPacketSize(); // dmp packet size
    dmp_ready = true; // dmp is ready to be used
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

Orientation_t InertialUnit::getOrientation() {
    Orientation_t orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};

    // Compute 4D Orientation
    // Compute Roll Pitch
    // Compute 3D Orientation
    mpu.dmpGetQuaternion(&quaternion, fifo_buffer);
    mpu.dmpGetGravity(&gravity, &quaternion);
    mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);

    // Compute 3D Orientation from 4D Orientation
    // Convert rad to deg
    orientation.yaw = ypr[0] * 180.0f / static_cast<float>(M_PI);
    orientation.pitch = ypr[1] * 180.0f / static_cast<float>(M_PI);
    orientation.roll = ypr[2] * 180.0f / static_cast<float>(M_PI);
    return orientation;
}

void InertialUnit::run() {
    if (!imu_usable) {
        return;
    }

    // Ready and compute orientation
    Orientation_t orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};

    Serial.println("[Log] Reading Orientation");
    for (;;) {
        if (!(dmp_ready && mpu.dmpGetCurrentFIFOPacket(fifo_buffer))) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        // Compute 4D Orientation
        // mpu.dmpGetQuaternion(&quaternion, fifo_buffer);
        // mpu.dmpGetGravity(&gravity, &quaternion);
        // mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
        //
        // // Compute 3D Orientation from 4D Orientation
        // // Convert rad to deg
        // orientation.yaw = ypr[0] * 180.0f / M_PI;
        // orientation.pitch = ypr[1] * 180.0f / M_PI;
        // orientation.roll = ypr[2] * 180.0f / M_PI;

        orientation = getOrientation();

        current_orientation.roll = orientation.roll - base_orientation.roll;
        current_orientation.pitch = orientation.pitch - base_orientation.pitch;
        current_orientation.yaw = orientation.yaw - base_orientation.yaw;

        Serial.printf(
            "Roll:  %6.2f° | Pitch: %6.2f° | Yaw: %6.2f°\n",
            current_orientation.roll,
            current_orientation.pitch,
            current_orientation.yaw
        );

        // ~100Hz
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

bool InertialUnit::ready() const {
    return imu_usable;
}

void InertialUnit::setBaseOrientation() {
    base_orientation = getOrientation();
}

MPU6050 InertialUnit::get_mpu() const {
    return mpu;
}
uint8_t *InertialUnit::get_buffer() {
    return fifo_buffer;
}
