//
// Created by CryosArtic on 8/2/2026.
//

#ifndef DRIVESYSTEM_INPUTMANAGER_H
#define DRIVESYSTEM_INPUTMANAGER_H
#include "navigation/CoordinateList.h"
#include "navigation/Navigation.h"
#include "system/SystemMode.h"


enum InputMode {
    REMOTE,
    SYSTEM
};

class InputManager {
private:
    InputMode inputMode;
    CoordinateList coordinateList;
    Coordinate_t base_location;
    Coordinate_t target_location;
    bool has_new_data = false;

public:
    InputManager(SystemMode systemMode);

    void parseData();

    Coordinate_t getTarget();

    void run();
};


#endif //DRIVESYSTEM_INPUTMANAGER_H
