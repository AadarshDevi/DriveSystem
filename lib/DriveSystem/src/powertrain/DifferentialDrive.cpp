//
// Created by CryosArtic on 6/29/2026.
//

#include "../../include/powertrain/DifferentialDrive.h"

#include <vector>

#include "motor/RawMotor.h"

DifferentialDrive::DifferentialDrive(
    int new_wheelCount,
    std::vector<std::vector<int> > new_motorPins,
    std::vector<int> new_powerInput
) {
    int wheelCount = -1;

    if (new_wheelCount % 2 != 0) {
        new_wheelCount = new_wheelCount - 1;
    }

    if (new_wheelCount <= 0) {
        wheelCount = 4;
    }

    int power = new_powerInput.at(0);
    int minPower = new_powerInput.at(0);
    int maxPower = new_powerInput.at(0);
    int shutdownPower = new_powerInput.at(0);

    for (int i = 0; i < wheelCount; i++) {
        std::vector<int> motorPins = new_motorPins[i];
        RawMotor motor(
            motorPins.at(0), motorPins.at(1), motorPins.at(2),
            power, minPower, maxPower, shutdownPower
        );
        motor.setPower(power);
        motors.push_back(motor);
    }
}

void DifferentialDrive::debugPower(int powerInput) const {
    for (int i = 0; i < motors.size(); i++) {
        RawMotor motor = motors[i];
        Serial.printf(
            "ippwr%d:%d pwr_%d:%d minp_%d:%d maxp_%d:%d stdpwr_%d:%d ",
            i + 1, powerInput,
            i + 1, motor.getPower(),
            i + 1, motor.getAbsMinPower(),
            i + 1, motor.getAbsMaxPower(),
            i + 1, motor.getShutdownPower()
        );
    }
    Serial.println();
}

void DifferentialDrive::setPower(int new_power) const {
    for (RawMotor motor: motors) {
        motor.setPower(new_power);
    }
}

void DifferentialDrive::setPower(int new_motorNum, int new_power) {
    motors[new_motorNum].setPower(new_power);
}

void DifferentialDrive::shutdownMotors() {
    for (RawMotor motor: motors) {
        while (motor.getPower() > motor.getShutdownPower()) {
            motor.powerVal(
                (motor.getPower() < (motor.getAbsMaxPower() * 0.05))
                    ? motor.getShutdownPower()
                    : motor.getPower() * 0.75);
            analogWrite(motor.getPinPWM(), motor.getPower());
            motor.debugAllPower(0);
            delay(200);
        }
    }
}

void DifferentialDrive::debug(bool new_debug) {
    for (RawMotor motor: motors) {
        motor.debug(new_debug);
    }
}

void DifferentialDrive::run() {
    for (RawMotor motor: motors) {
        motor.run();
    }
}
