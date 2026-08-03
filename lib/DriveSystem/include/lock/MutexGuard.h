//
// Created by CryosArtic on 8/1/2026.
//

#ifndef DRIVESYSTEM_MUTEXGUARD_H
#define DRIVESYSTEM_MUTEXGUARD_H

#include <freertos/semphr.h>                // Semaphore
#include <freertos/task.h>                  // xTaskCreate

#define MUTEX_TIMEOUT_MS 10

struct MutexGuard {
    SemaphoreHandle_t mutex;
    bool locked;

    MutexGuard(
        const SemaphoreHandle_t mutex,
        const TickType_t timeout = pdMS_TO_TICKS(MUTEX_TIMEOUT_MS)
    ) : mutex(mutex) {
        locked = (xSemaphoreTake(mutex, timeout) == pdTRUE);
    }

    ~MutexGuard() {
        if (locked)
            xSemaphoreGive(mutex);
    }

    explicit operator bool() const { return locked; }
};

#endif //DRIVESYSTEM_MUTEXGUARD_H
