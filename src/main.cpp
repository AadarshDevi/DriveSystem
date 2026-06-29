#include <Arduino.h>

#include "../lib/DriveSystem/include/motor/RawMotor.h"

int power = 0;
constexpr int absMinPower = 0;
constexpr int absMaxPower = 255;
constexpr int shutdownPower = 0;

RawMotor left_control(25, 27, 12, power, absMinPower, absMaxPower, shutdownPower);
RawMotor right_control(26, 33, 32, power, absMinPower, absMaxPower, shutdownPower);

RawMotor left_drive(14, 21, 4, power, absMinPower, absMaxPower, shutdownPower);
RawMotor right_drive(13, 23, 22, power, absMinPower, absMaxPower, shutdownPower);

void setup() {
    Serial.begin(115200);

    left_control.debug(true);
    left_control.setPower(power);
    left_control.start();

    left_drive.debug(true);
    left_drive.setPower(power);
    left_drive.start();

    right_control.debug(true);
    right_control.setPower(power);
    right_control.reverse();
    right_control.start();

    right_drive.debug(true);
    right_drive.setPower(power);
    right_drive.reverse();
    right_drive.start();

    delay(10000); // 10 sec
}

void loop() {
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd.length() > 0) {
            if (cmd.startsWith("--power")) {
                power = cmd.substring(cmd.indexOf("=") + 1).toInt();
                left_control.setPower(power);
                left_drive.setPower(power);
                right_control.setPower(power);
                right_drive.setPower(power);
            } else if (cmd.startsWith("--min-power")) {
                int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
                left_control.setAbsMinPower(powerInput);
                left_drive.setAbsMinPower(powerInput);
                right_control.setAbsMinPower(powerInput);
                right_drive.setAbsMinPower(powerInput);
            } else if (cmd.startsWith("--max-power")) {
                int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
                left_control.setAbsMaxPower(powerInput);
                left_drive.setAbsMaxPower(powerInput);
                right_control.setAbsMaxPower(powerInput);
                right_drive.setAbsMaxPower(powerInput);
            } else if (cmd == "--shutdown") {
                while (power > shutdownPower) {
                    power = (power < (absMaxPower * 0.05)) ? shutdownPower : power * 0.75;

                    left_control.powerVal(power);
                    left_drive.powerVal(power);
                    right_control.powerVal(power);
                    right_drive.powerVal(power);

                    left_control.run();
                    left_drive.run();
                    right_control.run();
                    right_drive.run();
                    delay(200);
                }
                delay(200);
            }
        }
    }

    left_control.run();
    left_drive.run();
    right_control.run();
    right_drive.run();

    left_control.debugAllPower(power);
    left_drive.debugAllPower(power);
    right_control.debugAllPower(power);
    right_drive.debugAllPower(power);

    delay(200);
}

