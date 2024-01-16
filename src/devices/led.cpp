#include <iostream>

#include <driver/gpio.h>

#include "devices/led.hpp"

namespace devices {

/**
 * @brief Default constructor
 * @param pin
 */
gpio_led::gpio_led(gpio_num_t pin, bool opendrain = false, bool active_low = false)
    : _gpio(pin), _open_drain(opendrain), _active_low(active_low)
{
    /* configure GPIO pin */
    gpio_reset_pin(_gpio);
    gpio_set_direction(_gpio, _open_drain ? GPIO_MODE_INPUT_OUTPUT_OD : GPIO_MODE_INPUT_OUTPUT);
    /* init powered down */
    this->set(GPIO_LED_STATE_OFF);
}

/**
 * @brief Set the LED level
 * @param level
 */
void gpio_led::set(const uint8_t& level)
{
    gpio_set_level(_gpio, _active_low ? !level : level);
}

/**
 * @brief get the LED level
 * @param
 */
uint8_t gpio_led::get(void)
{
    uint8_t out = static_cast<uint8_t>(gpio_get_level(_gpio));
    return _active_low ? !out : out;
}

}; // namespace devices