/**
 * @file i2c_masterbus.hpp
 *
 * @brief I2C Master Bus instance
 *
 * This is an instance of the i2c_master_bus to be shared by
 * all devices on the bus.
 *
 */

#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include <driver/i2c.h>

#define MAIN_I2C_MASTER_FREQ_HZ        400000u /*!< I2C master clock frequency */
#define MAIN_I2C_MASTER_TX_BUF_DISABLE 0       /*!< I2C master doesn't need buffer */
#define MAIN_I2C_MASTER_RX_BUF_DISABLE 0       /*!< I2C master doesn't need buffer */

namespace comm {

class i2c_master_bus
{
  private:
    i2c_port_t _port{};
    i2c_config_t _conf{};

  public:
    /* Constructors */
    // i2c_master_bus();
    i2c_master_bus(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, bool scl_pullup_en = true,
                   bool sda_pullup_en = true);

    /* Public API */
    void scan_devices(void);
    bool ping_device(uint8_t addr, uint64_t timeout_ms);
    const i2c_port_t& port();
};

} // namespace comm