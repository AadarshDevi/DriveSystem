//
// Created by CryosArtic on 8/2/2026.
//

#include "../../include/comms/InputManager.h"

#include <HardwareSerial.h>

#include "navigation/CoordinateList.h"
#include "system/DriveMode.h"
#include "system/SystemMode.h"


InputManager::InputManager(SystemMode system_mode) {
    if (system_mode == REMOTE_CONTROL) {
        inputMode = REMOTE;
    } else if (system_mode == AUTONOMOUS) {
        inputMode = SYSTEM;
    }
}


void InputManager::readData(volatile CoordinateList *coordinateList) {
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

            if (data_write) {
                String x_str = data_char;
                x_str.trim();
                String y_str = Serial.readStringUntil(' ');
                y_str.trim();
                String z_str = Serial.readStringUntil(' ');
                z_str.trim();
                float x = x_str.toFloat();
                float y = y_str.toFloat();
                float z = z_str.toFloat();
                Serial.println(String(x) + ", " + String(y) + ", " + String(z));
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

void InputManager::run() {
}
