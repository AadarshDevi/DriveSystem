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
#include <imu/InertialUnit.h>               // Custom IMU
#include <MPU6050_6Axis_MotionApps20.h>     // MPU-6050 Lib
#include <SystemMode.h>                     // Type of Data Transfer
#include <DriveMode.h>                      // Type of Driving

// FreeRTOS
#define MUTEX_TIMEOUT_MS 10
SemaphoreHandle_t mutex;

// System
static SystemMode system_mode = SystemMode::GUIDED;
static DriveMode drive_mode = DriveMode::SINGLE_TARGET;

// System > Enabler
static bool mpu_working = false;
static bool wheel_encoder_working = false;

// System > Drive
static int dist_to_travel = 0;
static int dist_travelled = 0;

static volatile Orientation_t current_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f}; // continuously set
static Orientation_t target_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f}; // main computer sets

// Motor > Power
static int power = 128; // motor power
constexpr int absMinPower = 50; // min power limit
constexpr int absMaxPower = 175; // max power limit
constexpr int shutdownPower = 0; // shutdown motor power - for emergency uses

// Motor
static RawMotor left_mechanum(27, 23, 4, power, absMinPower, absMaxPower, shutdownPower);
static RawMotor right_mechanum(12, 18, 25, power, absMinPower, absMaxPower, shutdownPower);
static RawMotor left_omnidir(14, 19, 2, power, absMinPower, absMaxPower, shutdownPower);
static RawMotor right_omnidir(13, 26, 15, power, absMinPower, absMaxPower, shutdownPower);

// MPU-6050
#define I2C_SCL_PIN 21
#define I2C_SDA_PIN 22

void rotateLeft(void *pvParameters);

void rotateRight(void *pvParameters);

void drive(void *pvParameters);

void stop(void *pvParameters);

void wheelEncoder(void *pvParameters);

void imu(void *pvParameters);

void coords(void *pvParameters);

void orient(void *pvParameters);

void printOrientation(void *pvParameters);

void setup() {
    Serial.begin(115200);
    delay(10000);

    Serial.println("[Before] Initializing Semaphore");
    mutex = xSemaphoreCreateMutex();
    if (mutex == nullptr) {
        Serial.println("[Error] Could not create mutex");
        return;
    }
    Serial.println("[After] Semaphore Initialized");

    static InertialUnit imu(I2C_SDA_PIN, I2C_SCL_PIN);

    if (imu.ready()) {
        system_mode = AUTONOMOUS;
    } else {
        system_mode = GUIDED;
    }

    if (system_mode == AUTONOMOUS) {
        xTaskCreatePinnedToCore(
            orient,
            "IMU",
            4096,
            &imu,
            1,
            nullptr,
            1
        );
        xTaskCreatePinnedToCore(
            printOrientation,
            "PrintOrientation",
            4096,
            nullptr,
            1,
            nullptr,
            1
        );
    }
}

void orient(void *pvParameters) {
    InertialUnit &imu = *static_cast<InertialUnit *>(pvParameters);

    // Ready and compute orientation
    Orientation_t orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};

    Serial.println("[Log] Reading Orientation");
    uint8_t *buffer = imu.get_buffer();
    for (;;) {
        if (!(imu.is_dmp_ready() && imu.get_mpu().dmpGetCurrentFIFOPacket(buffer))) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        orientation = imu.getOrientation();

        current_orientation.roll = orientation.roll - imu.get_base_orientation().roll;
        current_orientation.pitch = orientation.pitch - imu.get_base_orientation().pitch;
        current_orientation.yaw = orientation.yaw - imu.get_base_orientation().yaw;

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

void printOrientation(void *pvParameters) {
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    bool print;

    for (;;) {
        print = false;
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(MUTEX_TIMEOUT_MS)) == pdTRUE) {
            roll = current_orientation.roll;
            pitch = current_orientation.pitch;
            yaw = current_orientation.yaw;
            print = true;
            xSemaphoreGive(mutex);
        }

        if (print) {
            Serial.printf("Roll:  %6.2f° | Pitch: %6.2f° | Yaw: %6.2f°\n", roll, pitch, yaw);
        }
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}


void loop() {
    vTaskDelete(nullptr);
}
