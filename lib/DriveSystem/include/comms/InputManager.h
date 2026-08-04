//
// Created by CryosArtic on 8/2/2026.
//

#ifndef DRIVESYSTEM_INPUTMANAGER_H
#define DRIVESYSTEM_INPUTMANAGER_H
#include "system/SystemMode.h"
#include "navigation/CoordinateList.h"

enum InputMode {
    REMOTE,
    SYSTEM
};

class InputManager {
private:
    InputMode inputMode;

public:
    InputManager(SystemMode systemMode);

    void parseData();

    Coordinate_t getTarget();

    void run();
};


#endif //DRIVESYSTEM_INPUTMANAGER_H
