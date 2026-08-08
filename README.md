# Code Structure

### Main

```
src/main.cpp
```

This is where the code starts

### Project Lib

Header files:

```
lib/DriveSystem/include
```

C++ files:

```
lib/DriveSystem/src
```

### Sensor Lib

The Rover uses two sensors, a wheel encoder and an imu.

The first sensor is the MPU-6050 IMU Module, and it is paired with a Complementary Filter to reduce noise. Use the
onboard DMP (Digital Motion Processor) to get the Orientation of the Rover.

```
class InertialUnit
```

The second sensor is the wheel encoder. It is a WYC-H206 Slotted Optical Photointerrupter Sensor Module. We use it top
calculate the distance traveled by the rover.

```
class WheelEncoder
```

### Motor Driver

The rover uses two TB6612FNG Motor Driver Modules with a total of four motor outputs to drive four mecanum wheels and
two omnidirectional wheels. Each omnidirectional wheel uses a motor output. Each pair of mecanum wheels uses a motor
output.

```
class DifferentialDrive
```

### Intelligence

The rover has a brain. It uses global position and imu to calculate the angle it has to rotate and the distance it has
to travel to the new point.

```
class IntelliSense
```