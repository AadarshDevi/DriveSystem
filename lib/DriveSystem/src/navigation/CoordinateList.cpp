//
// Created by CryosArtic on 7/31/2026.
//

#include "../../include/navigation/CoordinateList.h"

bool CoordinateList::add(const float x, const float y, const float z) {
    const int new_index = data_index + 1;
    if (new_index >= SIZE) {
        return false;
    }
    target_coords[new_index].x = x;
    target_coords[new_index].y = y;
    target_coords[new_index].z = z;
    data_index = new_index;
    return true;
}

bool CoordinateList::add(const Coordinate_t &coord) {
    const int new_index = data_index + 1;
    if (new_index >= SIZE) {
        return false;
    }
    target_coords[new_index].x = coord.x;
    target_coords[new_index].y = coord.y;
    target_coords[new_index].z = coord.z;
    data_index = new_index;
    return true;
}

Coordinate_t CoordinateList::getTarget() {
    Coordinate_t target = target_coords[data_index];
    target_index += 1;
    return target;
}

Coordinate_t CoordinateList::getFinalTarget() const {
    return target_coords[data_index];
}
