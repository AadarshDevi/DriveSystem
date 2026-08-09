#include <Arduino.h>                        // Arduino Lib
#include <motor/RawMotor.h>                 // Custom Motor
#include <sensor/InertialUnit.h>               // Custom IMU
#include <system/SystemMode.h>              // Type of Data Transfer
#include <system/DriveMode.h>               // Powertrain driving mode
#include <freertos/semphr.h>                // Semaphore
#include <freertos/task.h>                  // xTaskCreate
#include "comms/InputManager.h"
#include "powertrain/IntelliSense.h"
#include "sensor/WheelEncoder.h"
// FreeRTOS
SemaphoreHandle_t mutex;

// Navigation


// System
static SystemMode system_mode = SystemMode::MANUAL;
// static DriveMode drive_mode = DriveMode::FOCUSED;

// System > Enabler
// static bool mpu_working = false;
// static bool wheel_encoder_working = false;

// System > Drive
#define WHEEL_ENCODER_PIN 36
//static int dist_to_travel = 0;
//static volatile int dist_travelled = 0;

//static volatile Orientation_t external_current_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};
// continuously set
//static Orientation_t target_orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f}; // main computer sets

// Motor > Power
static int power = 128; // motor power
constexpr int absMinPower = 0; // min power limit
constexpr int absMaxPower = 175; // max power limit
constexpr int shutdownPower = 0; // shutdown motor power - for emergency uses

// Motor
static RawMotor left_mecanum(27, 23, 4, (power + 10), absMinPower, absMaxPower, shutdownPower);
static RawMotor left_omnidir(14, 19, 32, (power + 10), absMinPower, absMaxPower, shutdownPower);
static RawMotor right_mecanum(12, 18, 25, power, absMinPower, absMaxPower, shutdownPower);
static RawMotor right_omnidir(13, 26, 33, power, absMinPower, absMaxPower, shutdownPower);

// MPU-6050
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

void run_input_manager(void *p);

void run_imu(void *p);

void run_wheel_encoder(void *p);

void run_intelli_sense(void *p);

void setup() {
    // Set basic info
    Serial.begin(115200);
    delay(1000);

    // set semaphore
    Serial.println("[Before] Initializing Semaphore");
    mutex = xSemaphoreCreateMutex();
    if (mutex == nullptr) {
        Serial.println("[Error] Could not create mutex");
        return;
    }
    Serial.println("[After] Semaphore Initialized");

    static InertialUnit imu(I2C_SDA_PIN, I2C_SCL_PIN, mutex);
    if (imu.ready()) {
        system_mode = AUTONOMOUS;
    } else {
        system_mode = MANUAL;
    }

    // const char *send_mode = "[Powertrain] --sys-mode=%d --drive-mode=%d\n";

    static InputManager inputManager(system_mode);
    xTaskCreatePinnedToCore(
        run_input_manager,
        "run_input_manager",
        4069,
        &inputManager,
        1,
        nullptr,
        0
    );

    if (system_mode == AUTONOMOUS) {
        static WheelEncoder encoder(WHEEL_ENCODER_PIN);

        xTaskCreatePinnedToCore( // imu ready
            run_imu,
            "run_imu",
            4096,
            &imu,
            1,
            nullptr,
            1
        );
        xTaskCreatePinnedToCore(
            run_wheel_encoder,
            "run_wheel_encoder",
            4096,
            &encoder,
            1,
            nullptr,
            1
        );

        static DifferentialDrive differentialDrive(left_omnidir, right_omnidir, left_mecanum, right_mecanum);

        static IntelliSense intelliSense(imu, encoder, inputManager, differentialDrive);
        xTaskCreatePinnedToCore(
            run_intelli_sense,
            "run_intelli_sense",
            8192,
            &intelliSense,
            1,
            nullptr,
            0);
    } else if (system_mode == MANUAL) {
        // Serial.printf(send_mode, system_mode, drive_mode);
        // DifferentialDrive differentialDrive(left_omnidir, right_omnidir, left_mecanum, right_mecanum);
    }
}

void run_imu(void *pvParameters) {
    InertialUnit &imu = *static_cast<InertialUnit *>(pvParameters);
    Serial.printf(
        "[Powertrain::Running] InertialUnit >  Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n",
        imu.getOrientation().roll, imu.getOrientation().pitch, imu.getOrientation().yaw
    );
    imu.run();
}

void loop() {
    vTaskDelete(nullptr);
}

void run_input_manager(void *pvParameters) {
    InputManager &inputManager = *static_cast<InputManager *>(pvParameters);
    Serial.println("[Powertrain::Running] InputManager");
    inputManager.run();
}

void run_wheel_encoder(void *pvParameters) {
    WheelEncoder &encoder = *static_cast<WheelEncoder *>(pvParameters);
    Serial.println("[Powertrain::Running] WheelEncoder");
    encoder.run();
}

void run_intelli_sense(void *pvParameters) {
    IntelliSense &intelli_sense = *static_cast<IntelliSense *>(pvParameters);
    Serial.println("[Powertrain::Running] IntelliSense");
    intelli_sense.run();
}
