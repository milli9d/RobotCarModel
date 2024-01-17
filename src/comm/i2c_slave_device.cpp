/**
 * @file i2c_masterbus.hpp
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
#include <string>
#include <memory>
#include <cstdint>

#include <esp_system.h>

#include <driver/i2c.h>

#include "comm/i2c_master_bus.hpp"
#include "comm/i2c_slave_device.hpp"

#include "logging.h"
LOG_TAG(I2C_SLAVE);

namespace comm {

/**
 * @brief Constructor , initialize an I2C device
 * @param bus[in]           i2c bus
 * @param addr[in]          device addr
 * @param timeout_ms[in]    timeout in ms
 */
i2c_slave_device::i2c_slave_device(const std::shared_ptr<comm::i2c_master_bus>& bus, const std::string& name,
                                   const uint8_t& addr, const uint8_t& timeout_ms)
    : _bus(bus), _name(name), _addr(addr), _timeout_ms(timeout_ms)
{
    if (ping()) {
        LOG_INFO("Device %s [0x%d] registered and present", _name.c_str(), _addr);
    } else {
        LOG_INFO("Device %s [0x%d] registered, not present", _name.c_str(), _addr);
    }
}

/**
 * @brief Ping the device
 *
 * @return true if present
 */
bool i2c_slave_device::ping(void)
{
    return _bus->ping_device(_addr, _timeout_ms);
}

/**
 * @brief Read a byte from this device
 * @param reg[in]       reg address
 * @param val[out]      memory to read to
 * @return
 */
bool i2c_slave_device::read_byte(const uint8_t& reg, uint8_t& val)
{
    return read_bytes(reg, &val, 1u);
}

/**
 * @brief Write a byte to this device
 * @param reg[in]       reg address
 * @param val[in]       memory to write from
 * @return
 */
bool i2c_slave_device::write_byte(const uint8_t& reg, const uint8_t& val)
{
    return write_bytes(reg, &val, 1u);
}

/**
 * @brief Read a byte from this device
 * @param reg[in]       reg address
 * @param val[out]      memory to read to
 * @return
 */
bool i2c_slave_device::read_bytes(const uint8_t& reg, uint8_t* val, size_t sz)
{
    /* enqueue a read for register addr */
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, _addr << 1u | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK);

    /* repeated start */
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, _addr << 1u | I2C_MASTER_READ, I2C_MASTER_ACK);

    /* read byte */
    for (size_t i = 0u; i < s; i++) {
        i2c_master_read_byte(cmd, &val[i], I2C_MASTER_NACK);
    }

    i2c_master_stop(cmd);

    /* get status of current address */
    int ret = i2c_master_cmd_begin(_bus->port(), cmd, pdMS_TO_TICKS(_timeout_ms));
    if (ret != ESP_OK) {
        LOG_ERR("Failed to read %s [0x%X] addr %d: %d", _name, _addr, reg, ret);
        return false;
    }

    /* free memory */
    i2c_cmd_link_delete(cmd);

    return true;
}

/**
 * @brief Write a byte to this device
 * @param reg[in]       reg address
 * @param val[in]       memory to write from
 * @return
 */
bool i2c_slave_device::write_bytes(const uint8_t& reg, const uint8_t* val, size_t sz)
{
    /* enqueue a read for register addr */
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, _addr << 1u | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK);

    /* enqueue bytes */
    for (size_t i = 0u; i < sz; i++) {
        i2c_master_write_byte(cmd, val[i], I2C_MASTER_ACK);
        LOG_INFO("Write %s [0x%X] addr Reg 0x%X  Val 0x%X", _name.c_str(), _addr, reg + i, val[i]);
    }

    i2c_master_stop(cmd);

    /* get status of current address */
    int ret = i2c_master_cmd_begin(_bus->port(), cmd, pdMS_TO_TICKS(_timeout_ms));
    if (ret != ESP_OK) {
        LOG_ERR("Failed to write %s [0x%X] addr %d : %d", _name.c_str(), _addr, reg, ret);
        return false;
    }

    /* free memory */
    i2c_cmd_link_delete(cmd);

    return true;
}

/**
 * @brief Write a byte and read back to verify
 * @param reg[in]           reg address
 * @param val[in]           memory to write from
 * @param rb_wait_ms[in]    time to wait befoe readback
 * @return
 */
bool i2c_slave_device::write_byte_rb(const uint8_t& reg, const uint8_t& val, uint64_t rb_wait_ms)
{
    /* write, wait and read back to verify */
    write_byte(reg, val);
    std::this_thread::sleep_for(std::chrono::milliseconds(rb_wait_ms));

    /* attempt readback */
    uint8_t val_rb = 0u;
    if (read_byte(reg, val_rb)) {
        if (val == val_rb) {
            return true;
        }
    }

    /* if we reach here , we failed readback */
    LOG_ERR("Failed Readback!");
    return false;
}

void i2c_slave_device::reg_dump(uint8_t max_addr)
{
    for (uint16_t i = 0u; i <= max_addr; i++) {
        uint8_t val = 0u;
        if (read_byte((uint8_t)i, val)) {
            printf("%s REG[0x%X] = 0x%X\n", _name.c_str(), i, val);
        }
    }
}

} // namespace comm