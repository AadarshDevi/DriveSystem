#include <Arduino.h>

#include "../lib/DriveSystem/include/motor/RawMotor.h"

const int pinIn1 = 21;
const int pinIn2 = 22;
const int pinPWM = 23;

const int minPower = 35;
const int maxPower = 255;
int power = 255;
const int shutdownPower = 0;

// RawMotor motor(pinPWM, pinIn1, pinIn2, minPower, maxPower);

RawMotor motor(pinPWM, pinIn1, pinIn2, power, minPower, maxPower, shutdownPower);

void setup() {
    Serial.begin(115200);
    motor.debug(true);
    motor.setPower(127);
    motor.start();
    delay(10000);
}

void loop() {
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd.length() > 0) {
            if (cmd.startsWith("--power")) {
                power = cmd.substring(cmd.indexOf("=") + 1).toInt();
                motor.setPower(power);
            } else if (cmd.startsWith("--min-power")) {
                int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
                motor.setAbsMinPower(powerInput);
            } else if (cmd.startsWith("--max-power")) {
                int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
                motor.setAbsMaxPower(powerInput);
            } else if (cmd == "--shutdown") {
                motor.shutdown();
                delay(200);
                for (;;) {
                    delay(1000);
                }
            }
        }
    }
    motor.run();
    motor.debugAllPower(power);
    delay(200);
}

