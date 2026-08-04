//
// Created by CryosArtic on 8/2/2026.
//

#include "../../include/comms/InputManager.h"

#include <HardwareSerial.h>

#include "navigation/CoordinateList.h"
#include "system/SystemMode.h"


InputManager::InputManager(SystemMode system_mode) {
    if (system_mode == REMOTE_CONTROL) {
        inputMode = REMOTE;
    } else if (system_mode == AUTONOMOUS) {
        inputMode = SYSTEM;
    }
}


void InputManager::run() {
    if (!Serial) {
        Serial.begin(115200);
    }

    bool data_write = false;
    for (;;) {
        while (Serial.available() > 0) {
            String data_char = Serial.readStringUntil(' ');
            data_char.trim();

            if (data_char.length() == 0) continue;

            if (data_char.equals("--data-write")) {
                data_write = true;
                continue;
            } else if (data_char.equals("--data-end")) {
                data_write = false;
                continue;
            }

            // todo make Coordinate_t objects and parse them
            if (data_write) {
                Coordinate_t coordinate = {
                    .x = data_char.toFloat(),
                    .y = Serial.readStringUntil(' ').toFloat(),
                    .z = Serial.readStringUntil(' ').toFloat()
                };
                coordinateList.add(coordinate);
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void InputManager::parseData() {
}

Coordinate_t InputManager::getTarget() {
}
