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
#include <string>
#include <memory>
#include <cstdint>

#include <esp_system.h>

#include <driver/i2c.h>

#include "comm/i2c_master_bus.hpp"

namespace comm {

class i2c_slave_device
{
  private:
    std::shared_ptr<comm::i2c_master_bus> _bus{};
    std::string _name{};
    uint8_t _addr{};
    uint8_t _timeout_ms{};

  public:
    /* Constructors */
    i2c_slave_device(const std::shared_ptr<comm::i2c_master_bus>& bus, std::string name, uint8_t addr,
                     uint8_t timeout_ms);

    /* Public API */
    void reg_dump(uint8_t max_addr = UINT8_MAX);
    bool ping(void);
    bool read_byte(const uint8_t& reg, uint8_t& val);
    bool write_byte(const uint8_t& reg, const uint8_t& val);
    bool write_byte_rb(const uint8_t& reg, const uint8_t& val, uint64_t rb_wait_ms);
};

} // namespace comm