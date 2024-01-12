#include <iostream>
#include <thread>
#include <chrono>

#include "init.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Main app entry point
 */
void app_main()
{
    /* init system */
    esp_bot_init();

    /* sleep indefinitely */
    while (1) {
        std::this_thread::sleep_for(std::chrono::hours::max());
    }
}

#ifdef __cplusplus
}; // extern "C"
#endif
