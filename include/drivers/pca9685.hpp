#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include <driver/i2c.h>

#include "comm/i2c_master_bus.hpp"
#include "comm/i2c_slave_device.hpp"

namespace drivers {

#define PCA9685_DEFAULT_I2C_TIMEOUT_MS 100u

class pca9685 : public comm::i2c_slave_device
{
  public:
    /* constructor */
    pca9685(const std::shared_ptr<comm::i2c_master_bus>& _bus);

    /* public API */
    void sleep(bool sleep);
    bool sleep(void);

    bool lock_multi_register_access(bool lock = true);
    bool set_pwm(uint8_t pwm_port, uint8_t duty_cycle_percent);
};

} // namespace drivers