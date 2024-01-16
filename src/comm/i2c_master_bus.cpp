/**
 * @file i2c_masterbus.cpp
 *
 * @brief I2C Master Bus instance
 *
 * This is an instance of the i2c_master_bus to be shared by
 * all devices on the bus.
 *
 */

#include <iostream>
#include <thread>
#include <chrono>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include <driver/i2c.h>

#include "comm/i2c_master_bus.hpp"

#include "logging.h"
LOG_TAG(I2C_MASTER_BUS);

namespace comm {

/**
 * @brief Default constructor for I2C bus
 * @param port
 * @param sda
 * @param scl
 * @param scl_pullup_en
 * @param sda_pullup_en
 */
i2c_master_bus::i2c_master_bus(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, bool scl_pullup_en, bool sda_pullup_en)
    : _port(port)
{
    _conf.mode = I2C_MODE_MASTER;
    _conf.sda_io_num = sda;
    _conf.scl_io_num = scl;
    _conf.sda_pullup_en = sda_pullup_en;
    _conf.scl_pullup_en = scl_pullup_en;
    _conf.master.clk_speed = MAIN_I2C_MASTER_FREQ_HZ;

    ESP_ERROR_CHECK(i2c_param_config(_port, &_conf));
    ESP_ERROR_CHECK(
        i2c_driver_install(_port, _conf.mode, MAIN_I2C_MASTER_RX_BUF_DISABLE, MAIN_I2C_MASTER_TX_BUF_DISABLE, 0));
}

/**
 * @brief Scan all available devices on the I2C bus
 * @param
 */
void i2c_master_bus::scan_devices(void)
{
    LOG_INFO("Scanning for I2C devices on bus.");

    int count = 0;
    for (size_t i = 0u; i < 8u; i++) {
        /* formatting */
        if (!count) {
            printf("     [ ");
        }

        printf("%4ld ", (unsigned long)i);

        /* formatting */
        if (++count == 8) {
            printf("]\n");
            count = 0u;
        }
    }

    for (size_t i = 0u; i < 128u; i++) {
        /* formatting */
        if (!count) {
            printf("%4d [ ", i);
        }

        if (ping_device(i, 10u)) {
            printf("0x%02lX ", (unsigned long)i);
        } else {
            printf("---- ");
        }

        /* formatting */
        if (++count == 8) {
            printf("]\n");
            count = 0u;
        }
    }
    printf("\n");
}

/**
 * @brief Ping the device
 * @param addr
 * @param timeout_ms
 * @return
 */
bool i2c_master_bus::ping_device(uint8_t addr, uint64_t timeout_ms)
{
    /* scan available I2C device on current i as address */
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, addr << 1u | I2C_MASTER_WRITE, 1u);
    i2c_master_stop(cmd);

    /* get status of current address */
    int ret = i2c_master_cmd_begin(_port, cmd, pdMS_TO_TICKS(timeout_ms));
    if (ret != ESP_OK) {
        return false;
    }

    /* free heap memory */
    i2c_cmd_link_delete(cmd);
    return true;
}

/**
 * @brief Return the I2C port instance
 * @return
 */
const i2c_port_t& i2c_master_bus::port()
{
    return _port;
}

} // namespace comm