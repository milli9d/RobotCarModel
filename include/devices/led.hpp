#pragma once

#include <iostream>
#include <driver/gpio.h>

/**
 * @brief
 */
namespace devices {

#define GPIO_LED_STATE_ON  1u
#define GPIO_LED_STATE_OFF 0u

class gpio_led
{
  private:
    gpio_num_t _gpio{};
    bool _open_drain{};
    bool _active_low{};

  public:
    gpio_led(gpio_num_t pin, bool opendrain, bool active_low);
    void set(const uint8_t& level);
    uint8_t get(void);
};

}; // namespace devices