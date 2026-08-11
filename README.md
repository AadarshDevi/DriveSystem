# Rover Powertrain DriveSystem

## Author

1. Name: Aadarsh Devi
2. Project Position: Powertrain Engineer
3. Internship: NASA Space Grant
4. Year: May 2026 - August 2026

## Description

The Nasa Internship Mapping Rover Powertrain uses four Mecanum wheels and two omnidirectional wheels. Each
omnidirectional wheel uses a motor output. Each pair of mecanum wheels uses a motor output. It runs on an ESP32 using
FreeRTOS to enable multiprocessing for precise sensor data separating sensor calculations from core logic. It runs on
two packs of AA batteries with each pack containing four AA batteries. The rover uses Differential Drive because it was
easy to implement.

## Components

### Sensor

#### WheelEncoder

The **_WheelEncoder_** calculates the distance traveled by the rover. The Wheel Encoder is the WYC-H206 Slotted Optical
Photointerrupter Sensor Module. It measures the distance in centimeters (cm).

#### InertialUnit

The **_InertialUnit_** calculates the orientation of the rover. The IMU is the MPU-6050 IMU Module paired with a
Complementary Filter to reduce noise. It measures the angle in degrees (deg).

### Motors

To drive the motors, the **_DifferentialDrive_** controls the motors as a group, easing the painful coding of each motor
whenever it is necessary.

Each motor has its software equivalent, the **_RawMotor_**. It controls the speed and rotation direction of the motor.

All the six motors are controlled by two TB6612FNG Motor Driver Modules.

### Data Input

To drive the rover, it receives data via the **_InputManager_**. The default orientation of the rover is positive x. It
creates coordinates of current and target Coordinate_t and returns a Trajectory2_t which contains the angle to rotate
and the distance to travel.

### Intelligence

To calculate the trajectory and drive to the target point, **_IntelliSense_** rotates to the correct angle and drive to
the correct distance.

## Position

Each point the rover has to drive through is the Coordinate_t. The path for the rover to travel to each point is called
the Trajectory2_t.

## Pipeline

From getting the data to driving to the target point:

1. Get input data from a computer
2. calculate trajectory
3. rotate the rover to face the target point dead straight
4. drive the rover straight to the target point