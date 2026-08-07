//
// Created by CryosArtic on 8/4/2026.
//

#ifndef DRIVESYSTEM_NAVIGATION_H
#define DRIVESYSTEM_NAVIGATION_H

#include <Arduino.h>
#include <math.h>

typedef struct {
    float x;
    float y;
    float z;

    void reset() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
} Coordinate_t;

typedef struct {
    Coordinate_t initial;
    Coordinate_t final;
} Trajectory_t;

struct Trajectory2_t {
    float angle;
    float distance;

    Trajectory2_t() : angle(0.0f), distance(0.0f) {
    }

    Trajectory2_t(const Coordinate_t initial, const Coordinate_t final) {
        float dx = final.x - initial.x;
        float dy = final.y - initial.y;

        distance = sqrt(
            pow(final.x - initial.x, 2) +
            pow(final.y - initial.y, 2)
        );
        angle = atan((final.y - initial.y) / (final.x - initial.x)) * (180.0f / M_PI);
    }

    float getAngle() {
        return angle;
    }

    float getDistance() {
        return distance;
    }
};

#endif //DRIVESYSTEM_NAVIGATION_H
