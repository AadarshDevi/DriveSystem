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

        float target_angle = inertialUnit.getOrientation().yaw + trajectory.angle;
        const float delta_angle = 2.3f;
        while (
            inertialUnit.getOrientation().yaw > target_angle - delta_angle &&
            inertialUnit.getOrientation().yaw < target_angle + delta_angle
        ) {
            differentialDrive.rotate(target_angle);
        }
        differentialDrive.stop();

        float target_distance = trajectory.distance;
        // const float delta_dist = 2.3f; // cm
        while (target_distance < wheelEncoder.getDistanceTravelled()) {
            differentialDrive.drive(target_distance);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    // code
}
