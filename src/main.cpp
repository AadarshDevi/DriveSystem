// #include <Arduino.h>
//
// #include "../lib/DriveSystem/include/motor/RawMotor.h"
//
// // motor speed = 255 --> rps = 3.25
// float wheelCircumference = 81.6814089933; // cm
//
// int power = 128;
// constexpr int absMinPower = 50;
// constexpr int absMaxPower = 255;
// constexpr int shutdownPower = 0;
//
// // wheel encoder sensor
// #define issmPin 34
// int steps = 0;
// int oldSteps = 0;
// int counterToCm = 1;
// int temp = 0;
// float rps = 0;
// float distance = 0;
//
// unsigned long startTime = 0;
// unsigned long endTime = 0;
//
// RawMotor left_mechanum(27, 23, 4, power, absMinPower, absMaxPower, shutdownPower);
// RawMotor right_mechanum(12, 18, 25, power, absMinPower, absMaxPower, shutdownPower);
// RawMotor left_omnidir(14, 19, 2, power, absMinPower, absMaxPower, shutdownPower);
// RawMotor right_omnidir(13, 26, 15, power, absMinPower, absMaxPower, shutdownPower);
//
// void setup() {
//     Serial.begin(115200);
//     pinMode(issmPin, INPUT);
//
//     left_mechanum.debug(true);
//     left_mechanum.setPower(power);
//     left_mechanum.start();
//
//     left_omnidir.debug(true);
//     left_omnidir.setPower(power);
//     left_omnidir.start();
//
//     right_mechanum.debug(true);
//     right_mechanum.setPower(power);
//     //right_control.reverse();
//     right_mechanum.start();
//
//     right_omnidir.debug(true);
//     right_omnidir.setPower(power);
//     //right_drive.reverse();
//     right_omnidir.start();
//
//     delay(10000); // 10 sec
// }
//
// void loop() {
//     if (Serial.available() > 0) {
//         String cmd = Serial.readStringUntil('\n');
//         cmd.trim();
//
//         if (cmd.length() > 0) {
//             if (cmd.startsWith("--power")) {
//                 power = cmd.substring(cmd.indexOf("=") + 1).toInt();
//                 left_mechanum.setPower(power);
//                 left_omnidir.setPower(power);
//                 right_mechanum.setPower(power);
//                 right_omnidir.setPower(power);
//             } else if (cmd.startsWith("--min-power")) {
//                 int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
//                 left_mechanum.setAbsMinPower(powerInput);
//                 left_omnidir.setAbsMinPower(powerInput);
//                 right_mechanum.setAbsMinPower(powerInput);
//                 right_omnidir.setAbsMinPower(powerInput);
//             } else if (cmd.startsWith("--max-power")) {
//                 int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
//                 left_mechanum.setAbsMaxPower(powerInput);
//                 left_omnidir.setAbsMaxPower(powerInput);
//                 right_mechanum.setAbsMaxPower(powerInput);
//                 right_omnidir.setAbsMaxPower(powerInput);
//             } else if (cmd == "--shutdown") {
//                 while (power > shutdownPower) {
//                     power = (power < (absMaxPower * 0.05)) ? shutdownPower : power * 0.75;
//
//                     left_mechanum.powerVal(power);
//                     left_omnidir.powerVal(power);
//                     right_mechanum.powerVal(power);
//                     right_omnidir.powerVal(power);
//
//                     left_mechanum.run();
//                     left_omnidir.run();
//                     right_mechanum.run();
//                     right_omnidir.run();
//                     delay(200);
//                 }
//                 delay(200);
//             }
//         }
//     }
//
//     left_mechanum.run();
//     left_omnidir.run();
//     right_mechanum.run();
//     right_omnidir.run();
//
//     left_mechanum.debugAllPower(power);
//     left_omnidir.debugAllPower(power);
//     right_mechanum.debugAllPower(power);
//     right_omnidir.debugAllPower(power);
//
//     // wheel encoder
//     startTime = millis();
//     endTime = startTime + 1000;
//
//     while (millis() < endTime) {
//         if (digitalRead(issmPin)) {
//             steps++;
//             while (digitalRead(issmPin));
//         }
//     }
//
//     temp = steps - oldSteps;
//     oldSteps = steps;
//     rps = temp / 20.0; // res per sec
//
//     Serial.print("RPS:");
//     Serial.print(rps);
//     Serial.print(" Distance:");
//     Serial.print(rps * wheelCircumference);
//     Serial.print(" Distance:");
//     Serial.println(distance += (rps * wheelCircumference) / (100 * 1000));
//
//     delay(200);
// }

#include <Arduino.h>                        // Arduino Lib
#include <motor/RawMotor.h>                 // Custom Motor
#include <freertos/FreeRTOS.h>              // Parallel Processing
#include <MPU6050_6Axis_MotionApps20.h>     // MPU-6050 Lib
#include <Wire.h>                           // I2C Hardware Driver
#include <I2Cdev.h>                         // I2C Auto Transmission Layer aka Wrapper
#include <SystemMode.h>
#include <DriveMode.h>

#include "imu/InertialUnit.h"

// System
SystemMode system_mode = SystemMode::GUIDED;
DriveMode drive_mode = DriveMode::SINGLE_TARGET;

// System > Enabler
bool mpu_working = false;
bool wheel_encoder_working = false;

// System > Drive
int dist_to_travel = 0;
int dist_travelled = 0;

// System > Orientation
typedef struct {
    float roll;
    float pitch;
    float yaw;
} Orientation_t;

Orientation_t base_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f}; // main computer sets
Orientation_t current_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f}; // continuously set
Orientation_t target_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f}; // main computer sets

// Motor > Power
int power = 128; // motor power
constexpr int absMinPower = 50; // min power limit
constexpr int absMaxPower = 175; // max power limit
constexpr int shutdownPower = 0; // shutdown motor power - for emergency uses

// Motor
RawMotor left_mechanum(27, 23, 4, power, absMinPower, absMaxPower, shutdownPower);
RawMotor right_mechanum(12, 18, 25, power, absMinPower, absMaxPower, shutdownPower);
RawMotor left_omnidir(14, 19, 2, power, absMinPower, absMaxPower, shutdownPower);
RawMotor right_omnidir(13, 26, 15, power, absMinPower, absMaxPower, shutdownPower);

// MPU-6050
#define I2C_SCL_PIN 21
#define I2C_SDA_PIN 22
MPU6050 mpu;

// MPU-6050 > DMP
bool dmp_ready = false; // on board computer to do math for orientation
int packet_size;
uint8_t fifo_buffer[64];

// MPU-6050 > Orientation
Quaternion quaternion;
VectorFloat gravity;
float ypr[3];

void rotateLeft(void *pvParameters);

void rotateRight(void *pvParameters);

void drive(void *pvParameters);

void stop(void *pvParameters);

void wheelEncoder(void *pvParameters);

void imu(void *pvParameters);

void coords(void *pvParameters);

void orient(void *p);

void setup() {
    Serial.begin(115200);

    delay(10000);

    InertialUnit imu(I2C_SDA_PIN, I2C_SCL_PIN);

    if (imu.ready()) {
        system_mode = AUTONOMOUS;
    } else {
        system_mode = GUIDED;
    }

    xTaskCreatePinnedToCore(
        orient,
        "IMU",
        4096,
        NULL,
        1,
        NULL,
        1
    );
}

void loop() {
}

void orient(void *pvParameters) {
}

// void imu(void *pvParameters) {
//     // Initialize I2C
//     Serial.println("[Before] Initializing Wire");
//     Wire.begin(I2C_SCL_PIN, I2C_SDA_PIN, static_cast<int>(1E5)); // 1E5 = 100,000 or 100kHz to steadily upload code
//     Serial.println("[After] Wire Initialized");
//
//     // Initialize IMU
//     Serial.println("[Before] Initializing MPU6050");
//     mpu.initialize();
//     if (!mpu.testConnection()) {
//         Serial.println("[Error] MPU6050 Connection Failed");
//         vTaskDelete(NULL);
//     }
//     Serial.println("[After] MPU6050 Initialized");
//
//     // IMU Reset
//     Serial.println("[Before] Resetting MPU6050");
//     mpu.setXAccelOffset(0);
//     mpu.setYAccelOffset(0);
//     mpu.setZAccelOffset(0);
//     mpu.setXGyroOffset(0);
//     mpu.setYGyroOffset(0);
//     mpu.setZGyroOffset(0);
//     Serial.println("[After] MPU6050 Reset");
//
//     // IMU Auto Calibration
//     Serial.println("[Before] Calibrating MPU6050");
//     mpu.CalibrateAccel(6);
//     mpu.CalibrateGyro(6);
//     mpu.PrintActiveOffsets();
//     mpu.setDLPFMode(MPU6050_DLPF_BW_20); // low pass band filter
//     Serial.println("[After] MPU6050 Calibrated");
//
//     // Initialize Onboard DMP
//     Serial.println("[Before] Initializing DMP");
//     int dev_status = mpu.dmpInitialize();
//     if (dev_status != 0) {
//         Serial.println("[Error] DMP Initialization Failed");
//         vTaskDelete(NULL);
//     }
//     mpu.setDMPEnabled(true); // enable mpu's dmp
//     packet_size = mpu.dmpGetFIFOPacketSize(); // dmp packet size
//     dmp_ready = true; // dmp is ready to be used
//
//     // DMP is ready to start doing calculations really quickly.
//     // so i2c can now be faster
//     Wire.setClock(static_cast<int>(4E5)); // 4E5 = 400,000 kHz
//     Serial.println("[After] DMP Initialized");
//
//     // Stabilize MPU605
//     Serial.println("[Before] Stabilizing MPU6050");
//     Orientation_t previous = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};
//     Orientation_t current;
//     constexpr int MAX_SAMPLES = 80;
//     int samples = 0;
//
//     vTaskDelay(pdMS_TO_TICKS(10000)); // 3 sec delay to stabilize mpu before setting up stable orientation
//     while (samples < MAX_SAMPLES) {
//         if (!mpu.dmpGetCurrentFIFOPacket(fifo_buffer)) {
//             vTaskDelay(pdMS_TO_TICKS(2));
//             continue;
//         }
//
//         // Compute 4D Orientation
//         // Compute Roll Pitch
//         // Compute 3D Orientation
//         mpu.dmpGetQuaternion(&quaternion, fifo_buffer);
//         mpu.dmpGetGravity(&gravity, &quaternion);
//         mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
//
//         // Compute 3D Orientation from 4D Orientation
//         // Convert rad to deg
//         current.yaw = ypr[0] * 180.0f / M_PI;
//         current.pitch = ypr[1] * 180.0f / M_PI;
//         current.roll = ypr[2] * 180.0f / M_PI;
//
//         if (
//             fabs(current.roll - previous.roll) >= 0.02f ||
//             fabs(current.pitch - previous.pitch) >= 0.02f ||
//             fabs(current.yaw - previous.yaw) >= 0.02f
//         ) {
//             samples = 0;
//             previous = current;
//             vTaskDelay(pdMS_TO_TICKS(10));
//             continue;
//         }
//
//         samples++;
//         previous = current;
//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
//
//     mpu.dmpGetQuaternion(&quaternion, fifo_buffer);
//     mpu.dmpGetGravity(&gravity, &quaternion);
//     mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
//
//     // Compute 3D Orientation from 4D Orientation
//     // Convert rad to deg
//     base_orientation.yaw = ypr[0] * 180.0f / M_PI;
//     base_orientation.pitch = ypr[1] * 180.0f / M_PI;
//     base_orientation.roll = ypr[2] * 180.0f / M_PI;
//     Serial.println("[After] MPU6050 Stabilized");
//
//     // Ready and compute orientation
//     float roll = 0.0;
//     float pitch = 0.0;
//     float yaw = 0.0;
//
//     Serial.println("[Log] Reading Orientation");
//     for (;;) {
//         if (!(dmp_ready && mpu.dmpGetCurrentFIFOPacket(fifo_buffer))) {
//             vTaskDelay(pdMS_TO_TICKS(10));
//             continue;
//         }
//
//         // Compute 4D Orientation
//         mpu.dmpGetQuaternion(&quaternion, fifo_buffer);
//         mpu.dmpGetGravity(&gravity, &quaternion);
//         mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
//
//         // Compute 3D Orientation from 4D Orientation
//         // Convert rad to deg
//         yaw = ypr[0] * 180.0f / M_PI;
//         pitch = ypr[1] * 180.0f / M_PI;
//         roll = ypr[2] * 180.0f / M_PI;
//
//         current_orientation.roll = roll - base_orientation.roll;
//         current_orientation.pitch = pitch - base_orientation.pitch;
//         current_orientation.yaw = yaw - base_orientation.yaw;
//
//         Serial.printf(
//             "Roll:  %6.2f° | Pitch: %6.2f° | Yaw: %6.2f°\n",
//             current_orientation.roll,
//             current_orientation.pitch,
//             current_orientation.yaw
//         );
//
//         // ~100Hz
//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }
