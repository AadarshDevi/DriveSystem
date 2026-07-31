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

// System
SystemMode system_mode = SystemMode::GUIDED;

// Motor > Power
int power = 128; // motor power
constexpr int absMinPower = 50; // min power limit
constexpr int absMaxPower = 255; // max power limit
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
int fifo_buffer[64];

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

void setup() {
    Serial.begin(115200);

    xTaskCreatePinnedToCore(
        imu,
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

void imu(void *pvParameters) {
    // Initialize I2C
    Serial.println("[Before] Initializing Wire");
    Wire.begin(I2C_SCL_PIN, I2C_SDA_PIN, static_cast<int>(1E5)); // 1E5 = 100,000 or 100kHz to steadily upload code
    Serial.println("[After] Wire Initialized");

    // Initialize IMU
    Serial.println("[Before] Initializing MPU6050");
    mpu.initialize();
    if (!mpu.testConnection()) {
        Serial.println("[Error] MPU6050 Connection Failed");
        vTaskDelete(NULL);
    }
    Serial.println("[After] MPU6050 Initialized");

    // IMU Reset
    Serial.println("[Before] Resetting MPU6050");
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    Serial.println("[After] MPU6050 Reset");

    // IMU Auto Calibration
    Serial.println("[Before] Calibrating MPU6050");
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    Serial.println("[After] MPU6050 Calibrated");

    // Initialize Onboard DMP
    Serial.println("[Before] Initializing DMP");
    int dev_status = mpu.dmpInitialize();
    if (dev_status != 0) {
        Serial.println("[Error] DMP Initialization Failed");
        vTaskDelete(NULL);
    }
    mpu.setDMPEnabled(true); // enable mpu's dmp
    packet_size = mpu.dmpGetFIFOPacketSize(); // dmp packet size
    dmp_ready = true; // dmp is ready to be used

    // DMP is ready to start doing calculations really quickly.
    // so i2c can now be faster
    Wire.setClock(static_cast<int>(4E5)); // 4E5 = 400,000 kHz
    Serial.println("[After] DMP Initialized");
}
