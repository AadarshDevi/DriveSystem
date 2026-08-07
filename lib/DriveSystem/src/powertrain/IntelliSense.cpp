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

        if (trajectory.angle == 0.0f || trajectory.distance == 0.0f) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        float starting_angle = inertialUnit.getOrientation().yaw;
        float target_angle = normalizeAngle(starting_angle - trajectory.angle);
        const float delta_angle = 2.3f;

        float current_angle = 0.0f;
        float error_angle = 0.0f;
        differentialDrive.enableDrive(true);
        while (
            // inertialUnit.getOrientation().yaw < target_angle - delta_angle ||
            // inertialUnit.getOrientation().yaw > target_angle + delta_angle
            // fabsf(target_angle - inertialUnit.getOrientation().yaw) > delta_angle
            true
        ) {
            current_angle = inertialUnit.getOrientation().yaw;
            error_angle = normalizeAngle(target_angle - current_angle);

            if (fabsf(error_angle) <= delta_angle) {
                break;
            }

            Serial.printf(
                "Current: %.2f Target: %.2f Error: %.2f\n",
                current_angle,
                target_angle,
                error_angle
            );

            differentialDrive.rotate(trajectory.angle);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        differentialDrive.stop();

        differentialDrive.enableDrive(true);

        float target_distance = trajectory.distance;
        // const float delta_dist = 2.3f; // cm
        while (target_distance < wheelEncoder.getDistanceTravelled()) {
            differentialDrive.drive(target_distance);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    // code
}
