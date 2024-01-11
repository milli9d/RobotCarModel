#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include "logging.h"
LOG_TAG("Main");

#include "init.hpp"

#define _TAG_MAIN "Main"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Main app entry point
 */
void app_main()
{
    esp_bot_init();

    /* @todo remove work from this loop */
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000u));
        LOG_INFO("Ping!");
    }
}

#ifdef __cplusplus
}; // extern "C"
#endif
