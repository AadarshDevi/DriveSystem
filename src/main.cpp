#include <Arduino.h>                        // Arduino Lib
#include <motor/RawMotor.h>                 // Custom Motor
#include <imu/InertialUnit.h>               // Custom IMU
#include <MPU6050_6Axis_MotionApps20.h>     // MPU-6050 Lib
#include <system/SystemMode.h>              // Type of Data Transfer
#include <system/DriveMode.h>               // Powertrain driving mode

// FreeRTOS
SemaphoreHandle_t mutex;

#define MUTEX_TIMEOUT_MS 10

struct MutexGuard {
    SemaphoreHandle_t mutex;
    bool locked;

    MutexGuard(
        const SemaphoreHandle_t mutex,
        const TickType_t timeout = pdMS_TO_TICKS(MUTEX_TIMEOUT_MS)
    ) : mutex(mutex) {
        locked = (xSemaphoreTake(mutex, timeout) == pdTRUE);
    }

    ~MutexGuard() {
        if (locked)
            xSemaphoreGive(mutex);
    }

    explicit operator bool() const { return locked; }
};


// Navigation


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

static void orient(void *pvParameters);

static void printOrientation(void *pvParameters);

void setup() {
    // Set basic info
    Serial.begin(115200);
    delay(10000);

    // set semaphore
    Serial.println("[Before] Initializing Semaphore");
    mutex = xSemaphoreCreateMutex();
    if (mutex == nullptr) {
        Serial.println("[Error] Could not create mutex");
        return;
    }
    Serial.println("[After] Semaphore Initialized");

    // set imu
    static InertialUnit imu(I2C_SDA_PIN, I2C_SCL_PIN);

    if (imu.ready()) {
        system_mode = AUTONOMOUS;
    } else {
        system_mode = GUIDED;
    }

    if (system_mode == AUTONOMOUS) {
        Serial.println("[Powertrain::Mode] AUTONOMOUS");
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
    } else {
        Serial.println("[Powertrain::Mode] GUIDED");
    }
}

void orient(void *pvParameters) {
    InertialUnit &imu = *static_cast<InertialUnit *>(pvParameters);

    // Ready and compute orientation
    Orientation_t orientation = {.roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};

    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;

    Serial.println("[Log] Reading Orientation");
    uint8_t *buffer = imu.get_buffer();
    for (;;) {
        if (!(imu.is_dmp_ready() && imu.get_mpu().dmpGetCurrentFIFOPacket(buffer))) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        orientation = imu.getOrientation();

        roll = orientation.roll - imu.get_base_orientation().roll;
        pitch = orientation.pitch - imu.get_base_orientation().pitch;
        yaw = orientation.yaw - imu.get_base_orientation().yaw;

        if (MutexGuard lock{
            mutex
        }) {
            current_orientation.roll = roll;
            current_orientation.pitch = pitch;
            current_orientation.yaw = yaw;
        }

        // Serial.printf(
        //     "Roll:  %6.2f° | Pitch: %6.2f° | Yaw: %6.2f°\n",
        //     current_orientation.roll,
        //     current_orientation.pitch,
        //     current_orientation.yaw
        // );

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
        if (MutexGuard lock{
            mutex
        }) {
            roll = current_orientation.roll;
            pitch = current_orientation.pitch;
            yaw = current_orientation.yaw;
            print = true;
        }

        if (print) {
            Serial.printf("Roll:  %6.2f° | Pitch: %6.2f° | Yaw: %6.2f°\n", roll, pitch, yaw);
        }
        vTaskDelay(pdMS_TO_TICKS(200)); // 200 ms for each print
    }
}

void loop() {
    vTaskDelete(nullptr);
}
