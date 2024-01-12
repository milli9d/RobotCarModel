#include <iostream>
#include <chrono>
#include <thread>
#include <cstdint>

#include "logging.h"
LOG_TAG("STATUS_LED");

#include "devices/led.hpp"

#define STATUS_LED GPIO_NUM_3

/**
 * @brief LED Status thread
 * @param
 */
static void status_led_thread(void)
{
    devices::gpio_led io2_led(STATUS_LED, false, true);
    LOG_INFO("LED thread started!");

    while (1) {
        io2_led.set(GPIO_LED_STATE_ON);
        std::this_thread::sleep_for(std::chrono::milliseconds(10u));

        io2_led.set(!io2_led.get());
        std::this_thread::sleep_for(std::chrono::milliseconds(90u));
    }
}

static std::thread led_thread(status_led_thread);