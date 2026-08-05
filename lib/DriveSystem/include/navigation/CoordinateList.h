//
// Created by CryosArtic on 7/31/2026.
//

#ifndef DRIVESYSTEM_NAVIGATION_H
#define DRIVESYSTEM_NAVIGATION_H

#include "navigation/Navigation.h"

#define SIZE 15

class CoordinateList {
private:
    Coordinate_t target_coords[SIZE];
    int data_index = 0;
    int target_index = 0;

public:
    bool add(float x, float y, float z);

    bool add(const Coordinate_t &coord);

    Coordinate_t getTarget();

    Coordinate_t getFinalTarget() const;

    int getSize() const { return SIZE; }

    void clear();

    bool final_target_reached() {
        if (target_index == data_index) {
            return true;
        }
        return false;
    }
};


#endif //DRIVESYSTEM_NAVIGATION_H
