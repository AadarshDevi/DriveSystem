#include <Arduino.h>
#include <vector>

#include "../lib/DriveSystem/include/motor/RawMotor.h"
#include "drive/DifferentialDrive.h"

int power = 128;
const int minPower = 35;
const int maxPower = 255;
const int shutdownPower = 0;

std::vector<std::vector<int> > pinInput{
    {25, 12, 27}, // l-ctrl
    {14, 4, 21}, // l-drive
    {26, 32, 33}, // r-ctrl
    {13, 22, 23} // r-drive
};

std::vector<int> powerInput{
    127, // left control
    100, // left drive
    127, // right control
    100, // right drive
};

int powerRangeInput[3] = {
    35, // min
    255, // max
    0 // shutdown
};

DifferentialDrive dd1(pinInput, power, minPower, maxPower, shutdownPower);
DifferentialDrive dd2(pinInput, powerInput, minPower, maxPower, shutdownPower);
DifferentialDrive dd3(pinInput, powerInput, powerRangeInput);
DifferentialDrive dd4(pinInput, power, powerRangeInput);

RawMotor left_control(25, 12, 27, power, minPower, maxPower, shutdownPower);
RawMotor right_control(26, 32, 33, power, minPower, maxPower, shutdownPower);

RawMotor left_drive(14, 4, 21, power, minPower, maxPower, shutdownPower);
RawMotor right_drive(13, 22, 23, power, minPower, maxPower, shutdownPower);

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
    right_control.start();

    right_drive.debug(true);
    right_drive.setPower(power);
    right_drive.start();

    delay(10000);
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
                left_control.shutdown();
                left_drive.shutdown();
                right_control.shutdown();
                right_drive.shutdown();
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

