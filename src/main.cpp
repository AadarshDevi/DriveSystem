#include <Arduino.h>

#include "motor/RawMotor.h"
#include <bits/stdc++.h>

const int pinIn1 = 21;
const int pinIn2 = 22;
const int pinPWM = 23;

const int minPower = 35;
const int maxPower = 255;
int power = 0;

RawMotor motor(pinPWM, pinIn1, pinIn2, minPower, maxPower);

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
                motor.setMinPower(powerInput);
            } else if (cmd.startsWith("--max-power")) {
                int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
                motor.setMaxPower(powerInput);
            } else if (cmd == "--shutdown") {
                motor.powerDown();
                // Serial.println("[LOG] End of Motor Power Test");
                delay(250);
                for (;;) {
                    delay(1000);
                }
            }
        }
    }
    motor.run();
    Serial.printf("Input:%d Motor:%d Min:%d Max:%d\r\n", power, motor.getPower(), motor.getMinPower(),
                  motor.getMaxPower()); // prints power to serial graph
    delay(200);
}

