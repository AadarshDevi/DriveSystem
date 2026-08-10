//
// Created by CryosArtic on 8/2/2026.
//

#include "../../include/comms/InputManager.h"

#include <HardwareSerial.h>

#include "navigation/CoordinateList.h"
#include "system/SystemMode.h"
#include "navigation/Navigation.h"


InputManager::InputManager(SystemMode system_mode) : coordinateList() {
    if (system_mode == MANUAL) {
        inputMode = REMOTE;
    } else if (system_mode == AUTONOMOUS) {
        inputMode = SYSTEM;
    }
}


void InputManager::run() {
    if (!Serial) {
        Serial.begin(115200);
    }

    Serial.println("[Powertrain::InputManager] Ready 2 Run");

    bool data_write = false;
    for (;;) {
        // Serial.println("[Powertrain::InputManager] Forever Loop...");
        while (Serial.available() > 0) {
            Serial.println("[Powertrain::InputManager] Reading Serial...");
            String data_char = Serial.readStringUntil(' ');
            data_char.trim();

            if (data_char.length() == 0) continue;

            Serial.println("[Powertrain::InputManager] Checking for CMDs...");
            if (data_char.equals("--data-write")) {
                data_write = true;
                continue;
            } else if (data_char.equals("--data-end")) {
                data_write = false;
                continue;
            } else if (data_char.equals("--set-location") && !has_new_data) { // fixme this will not work
                // --set-location x0 y0 x1 y1

                base_location = {
                    .x = Serial.parseFloat(),
                    .y = Serial.parseFloat(),
                    .z = 0.0f
                };
                target_location = {
                    .x = Serial.parseFloat(),
                    .y = Serial.parseFloat(),
                    .z = 0.0f
                };
                has_new_data = true;
                continue;
            } else if (data_char.equals("--force-shutdown")) {
                force_shutdown = true;
            } else if (data_char.startsWith("--")) {
                continue;
            }

            // todo make Coordinate_t objects and parse them
            if (data_write) {
                Coordinate_t coordinate = {
                    .x = data_char.toFloat(),
                    .y = Serial.parseFloat(),
                    .z = Serial.parseFloat(),
                };
                coordinateList.add(coordinate);
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(10));
}

Trajectory2_t InputManager::getNewData() {
    Trajectory2_t trajectory;

    if (!has_new_data) {
        trajectory = {
            {.x = 0, .y = 0, .z = 0},
            {.x = 0, .y = 0, .z = 0}
        };
    }
    has_new_data = false;
    trajectory = {base_location, target_location};
    return trajectory;
}
