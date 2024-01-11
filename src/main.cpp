#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include "init.hpp"

#define _TAG_MAIN "Main"

extern "C" {
void app_main()
{
    esp_bot_init();

    while (1) {
        std::cout << "Hello ESP_BOT" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000u));
    }
}
};
