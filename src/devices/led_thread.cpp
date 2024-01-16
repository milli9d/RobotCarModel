#include <iostream>
#include <chrono>
#include <thread>
#include <cstdint>

#include "esp_pthread.h"

#include "logging.h"
LOG_TAG("STATUS_LED");

#include "devices/led.hpp"
#include "led_thread.hpp"

namespace tasks {

#define STATUS_LED GPIO_NUM_3

/**
 * @brief LED Status thread
 * @param
 */
void status_led::_run(status_led* _this)
{
    devices::gpio_led io2_led(STATUS_LED, false, true);
    LOG_INFO("LED thread started!");

    while (1) {
        io2_led.set(GPIO_LED_STATE_ON);
        std::this_thread::sleep_for(std::chrono::milliseconds(50u));

        io2_led.set(!io2_led.get());
        std::this_thread::sleep_for(std::chrono::milliseconds(950u));
    }
}

/**
 * @brief Constructor
 * @param
 */
status_led::status_led(void)
{
    esp_pthread_cfg_t _cfg = {
        .stack_size = 512u,
        .prio = 2u,
        .inherit_cfg = false,
        .thread_name = "status_led",
        .pin_to_core = 1u,
    };

    esp_pthread_set_cfg(&_cfg);

    _thread = std::make_unique<std::thread>(this->_run, this);
}

} // namespace tasks