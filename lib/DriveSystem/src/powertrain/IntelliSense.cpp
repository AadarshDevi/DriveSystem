//
// Created by CryosArtic on 6/27/2026.
//

#include "../../include/powertrain/IntelliSense.h"

#include "comms/InputManager.h"
#include "powertrain/DifferentialDrive.h"
#include "sensor/InertialUnit.h"
#include "sensor/WheelEncoder.h"

IntelliSense::IntelliSense(
    InertialUnit &inertialUnit,
    WheelEncoder &wheelEncoder,
    InputManager &inputManager,
    DifferentialDrive &differentialDrive
) : inertialUnit(inertialUnit),
    wheelEncoder(wheelEncoder),
    inputManager(inputManager),
    differentialDrive(differentialDrive) {
}

void IntelliSense::run() {
    for (;;) {
        if (!inputManager.hasNewData()) {
            differentialDrive.stop();
            wheelEncoder.resetDistance();
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        Trajectory2_t trajectory = inputManager.getNewData();
        Serial.printf("Angle: %.2f", trajectory.angle);
        Serial.printf("\tDistance: %.2f\n", trajectory.distance);

        if (trajectory.angle == 0.0f && trajectory.distance == 0.0f) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        float starting_angle = inertialUnit.getOrientation().yaw;
        float target_angle = normalizeAngle(trajectory.angle);
        constexpr float delta_angle = 1.0f;

        int rotation_direction = 0;

        if (target_angle <= normalizeAngle(starting_angle + 180)) {
            rotation_direction = 1;
        } else if (target_angle > normalizeAngle(starting_angle + 180)) {
            rotation_direction = -1;
        } else {
            rotation_direction = 0;
        }

        differentialDrive.enableDrive(true);

            Serial.printf(
                "Current: %.2f Target: %.2f Error: %.2f\n",
                current_angle,
                target_angle,
                error_angle
            );

            differentialDrive.rotate(static_cast<int>(target_angle));
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        differentialDrive.stop();

        differentialDrive.enableDrive(true);

        float target_distance = trajectory.distance;
        // const float delta_dist = 2.3f; // cm
        while (wheelEncoder.getDistanceTravelled() < target_distance) {
            Serial.printf(
                "Encoder: %.2f / %.2f\n",
                wheelEncoder.getDistanceTravelled(),
                target_distance
            );

            differentialDrive.drive(static_cast<int>(target_distance));
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        differentialDrive.stop();
        wheelEncoder.resetDistance();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    // code
}


float IntelliSense::normalizeAngle(float angle) {
    while (angle > 180.0f) angle -= 360.0f;
    while (angle < -180.0f) angle += 360.0f;
    return angle;
}
