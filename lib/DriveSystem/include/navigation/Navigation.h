//
// Created by CryosArtic on 7/31/2026.
//

#ifndef DRIVESYSTEM_NAVIGATION_H
#define DRIVESYSTEM_NAVIGATION_H

typedef struct {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    void reset() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
} Coordinate_t;

#define SIZE 15

class Navigation {
private:
    Coordinate_t target_coords[SIZE];
    int index = 0;

public:
    void add(float x, float y, float z);

    Coordinate_t getFirst() const;

    int getSize() const { return SIZE; }

    void clear();
};


#endif //DRIVESYSTEM_NAVIGATION_H
