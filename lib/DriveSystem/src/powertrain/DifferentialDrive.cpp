//
// Created by CryosArtic on 6/29/2026.
//

#include "../../include/powertrain/DifferentialDrive.h"

#include <vector>

#include "motor/RawMotor.h"

DifferentialDrive::DifferentialDrive(
    RawMotor &left_omni, RawMotor &right_omni,
    RawMotor &left_mecanum, RawMotor &right_mecanum
) : left_omni(left_omni), right_omni(right_omni), left_mecanum(left_mecanum), right_mecanum(right_mecanum) {
    setPower(200);
}

void DifferentialDrive::setPower(int power) {
    left_omni.setPower(power);
    right_omni.setPower(power);
    left_mecanum.setPower(power);
    right_mecanum.setPower(power);
}

void DifferentialDrive::drive(float distance) {
    if (distance > 0) {
        left_omni.forward();
        right_omni.forward();
        left_mecanum.forward();
        right_mecanum.forward();
    } else {
        left_omni.reverse();
        right_omni.reverse();
        left_mecanum.reverse();
        right_mecanum.reverse();
    }
    run();
}

void DifferentialDrive::rotate(float angle) {
    if (angle > 0) {
        // right
        left_omni.forward();
        right_omni.reverse();
        left_mecanum.forward();
        right_mecanum.reverse();
    } else {
        // left
        right_omni.forward();
        left_omni.reverse();
        right_mecanum.forward();
        left_mecanum.reverse();
    }
    // code
    run();
}

void DifferentialDrive::stop() {
    left_omni.stop();
    right_omni.stop();
    left_mecanum.stop();
    right_mecanum.stop();
}

void DifferentialDrive::run() {
    left_omni.run();
    right_omni.run();
    left_mecanum.run();
    right_mecanum.run();
}
