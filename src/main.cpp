// #include <Arduino.h>
//
// #include "../lib/DriveSystem/include/motor/RawMotor.h"
//
// // motor speed = 255 --> rps = 3.25
// float wheelCircumference = 81.6814089933; // cm
//
// int power = 128;
// constexpr int absMinPower = 50;
// constexpr int absMaxPower = 255;
// constexpr int shutdownPower = 0;
//
// // wheel encoder sensor
// #define issmPin 34
// int steps = 0;
// int oldSteps = 0;
// int counterToCm = 1;
// int temp = 0;
// float rps = 0;
// float distance = 0;
//
// unsigned long startTime = 0;
// unsigned long endTime = 0;
//
// RawMotor left_mechanum(27, 23, 4, power, absMinPower, absMaxPower, shutdownPower);
// RawMotor right_mechanum(12, 18, 25, power, absMinPower, absMaxPower, shutdownPower);
// RawMotor left_omnidir(14, 19, 2, power, absMinPower, absMaxPower, shutdownPower);
// RawMotor right_omnidir(13, 26, 15, power, absMinPower, absMaxPower, shutdownPower);
//
// void setup() {
//     Serial.begin(115200);
//     pinMode(issmPin, INPUT);
//
//     left_mechanum.debug(true);
//     left_mechanum.setPower(power);
//     left_mechanum.start();
//
//     left_omnidir.debug(true);
//     left_omnidir.setPower(power);
//     left_omnidir.start();
//
//     right_mechanum.debug(true);
//     right_mechanum.setPower(power);
//     //right_control.reverse();
//     right_mechanum.start();
//
//     right_omnidir.debug(true);
//     right_omnidir.setPower(power);
//     //right_drive.reverse();
//     right_omnidir.start();
//
//     delay(10000); // 10 sec
// }
//
// void loop() {
//     if (Serial.available() > 0) {
//         String cmd = Serial.readStringUntil('\n');
//         cmd.trim();
//
//         if (cmd.length() > 0) {
//             if (cmd.startsWith("--power")) {
//                 power = cmd.substring(cmd.indexOf("=") + 1).toInt();
//                 left_mechanum.setPower(power);
//                 left_omnidir.setPower(power);
//                 right_mechanum.setPower(power);
//                 right_omnidir.setPower(power);
//             } else if (cmd.startsWith("--min-power")) {
//                 int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
//                 left_mechanum.setAbsMinPower(powerInput);
//                 left_omnidir.setAbsMinPower(powerInput);
//                 right_mechanum.setAbsMinPower(powerInput);
//                 right_omnidir.setAbsMinPower(powerInput);
//             } else if (cmd.startsWith("--max-power")) {
//                 int powerInput = cmd.substring(cmd.indexOf("=") + 1).toInt();
//                 left_mechanum.setAbsMaxPower(powerInput);
//                 left_omnidir.setAbsMaxPower(powerInput);
//                 right_mechanum.setAbsMaxPower(powerInput);
//                 right_omnidir.setAbsMaxPower(powerInput);
//             } else if (cmd == "--shutdown") {
//                 while (power > shutdownPower) {
//                     power = (power < (absMaxPower * 0.05)) ? shutdownPower : power * 0.75;
//
//                     left_mechanum.powerVal(power);
//                     left_omnidir.powerVal(power);
//                     right_mechanum.powerVal(power);
//                     right_omnidir.powerVal(power);
//
//                     left_mechanum.run();
//                     left_omnidir.run();
//                     right_mechanum.run();
//                     right_omnidir.run();
//                     delay(200);
//                 }
//                 delay(200);
//             }
//         }
//     }
//
//     left_mechanum.run();
//     left_omnidir.run();
//     right_mechanum.run();
//     right_omnidir.run();
//
//     left_mechanum.debugAllPower(power);
//     left_omnidir.debugAllPower(power);
//     right_mechanum.debugAllPower(power);
//     right_omnidir.debugAllPower(power);
//
//
//     // todo create class
//
//     // wheel encoder
//     startTime = millis();
//     endTime = startTime + 1000;
//
//     while (millis() < endTime) {
//         if (digitalRead(issmPin)) {
//             steps++;
//             while (digitalRead(issmPin));
//         }
//     }
//
//     temp = steps - oldSteps;
//     oldSteps = steps;
//     rps = temp / 20.0; // res per sec
//
//     Serial.print("RPS:");
//     Serial.print(rps);
//     Serial.print(" Distance:");
//     Serial.print(rps * wheelCircumference);
//     Serial.print(" Distance:");
//     Serial.println(distance += (rps * wheelCircumference) / (100 * 1000));
//
//     delay(200);
// }

#include <Arduino.h>

#include "../lib/DriveSystem/include/motor/RawMotor.h"

// motor speed = 255 --> rps = 3.25
float wheelCircumference = 81.6814089933; // cm

int power = 128;
constexpr int absMinPower = 50;
constexpr int absMaxPower = 255;
constexpr int shutdownPower = 0;

// wheel encoder sensor
#define issmPin 34
int steps = 0;
int oldSteps = 0;
int counterToCm = 1;
int temp = 0;
float rps = 0;
float distance = 0;

unsigned long startTime = 0;
unsigned long endTime = 0;

RawMotor left_mechanum(27, 25, 23, power, absMinPower, absMaxPower, shutdownPower);
RawMotor right_mechanum(14, 4, 19, power, absMinPower, absMaxPower, shutdownPower);
RawMotor left_omnidir(12, 2, 18, power, absMinPower, absMaxPower, shutdownPower);
RawMotor right_omnidir(13, 15, 26, power, absMinPower, absMaxPower, shutdownPower);

void setup() {
}

void loop() {
}
