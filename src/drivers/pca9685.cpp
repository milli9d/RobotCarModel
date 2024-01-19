#include <iostream>
#include <thread>
#include <bitset>
#include <cstdint>
#include <chrono>

#include "comm/i2c_master_bus.hpp"
#include "comm/i2c_slave_device.hpp"
#include "drivers/pca9685.hpp"
#include "drivers/pca9685_reg.hpp"

#include "utils/common_lib.hpp"

#include "logging.h"
LOG_TAG(PCA9685);

namespace drivers {

/**
 * @brief Lock auto-increment of registers
 * @param lock
 */
bool pca9685::lock_multi_register_access(bool lock)
{
    uint8_t val = 0u;
    if (!read_byte(PCA9685_REG_MODE_1, val)) {
        LOG_ERR("Failed read.");
        return false;
    }

    if (lock) {
        val = utils::set_bits<uint8_t>(val, PCA9685_REG_MODE_1_AI_B);
    } else {
        val = utils::clear_bits<uint8_t>(val, PCA9685_REG_MODE_1_AI_B);
    }

    return write_byte(PCA9685_REG_MODE_1, val);
}

/**
 * @brief Set sleep mode
 * @param sleep
 */
void pca9685::sleep(bool sleep)
{
    uint8_t val = 0u;
    if (!read_byte(PCA9685_REG_MODE_1, val)) {
        LOG_ERR("Failed read.");
        return;
    }

    if (sleep) {
        val = utils::set_bits<uint8_t>(val, PCA9685_REG_MODE_1_SLEEP_B);
    } else {
        val = utils::clear_bits<uint8_t>(val, PCA9685_REG_MODE_1_SLEEP_B);
    }

    write_byte(PCA9685_REG_MODE_1, val);
}

/**
 * @brief get the current sleep state
 * @return
 */
bool pca9685::sleep(void)
{
    /* read mode 1 register */
    uint8_t val = 0u;
    if (!read_byte(PCA9685_REG_MODE_1, val)) {
        LOG_ERR("Error reading MODE_1 sleep val.");
    }

    /* compare bit */
    return (val & PCA9685_REG_MODE_1_SLEEP_B);
}

bool pca9685::set_freq(uint16_t freq_hz)
{
    std::cout << "MAX " << PCA9685_REG_PRE_SCALE_MAX_FREQ_HZ << " MIN " << PCA9685_REG_PRE_SCALE_MIN_FREQ_HZ
              << std::endl;
    std::cout << "RES " << PCA9685_PRE_SCALE_FREQ_HZ_RES << " PSCALE " << PCA9685_FREQ_HZ_TO_PRE_SCALE(50u)
              << " PSCALE " << PCA9685_PRE_SCALE_TO_FREQ_HZ(121u) << std::endl;

    return write_byte(PCA9685_REG_PRE_SCALE, PCA9685_FREQ_HZ_TO_PRE_SCALE(freq_hz));
}

/**
 * @brief Set PWM output
 * @param pwm_port
 * @param duty_cycle_percent
 * @return
 */
bool pca9685::set_pwm(uint8_t pwm_port, uint8_t duty_cycle_percent)
{
    /* sanity check */
    if (duty_cycle_percent > 100u) {
        LOG_ERR("PWM value out of range");
        return false;
    }

    /* calculate time of PWM */
    uint16_t t_on = 0u;
    uint16_t t_off = ((duty_cycle_percent * (uint16_t)PCA9685_REG_LED_VAL_MAX) / 100u) & 0x00FFFFFFu;
    LOG_DBG("PWM On = %d, Off = %d , val = %d", t_on, t_off, (uint32_t)duty_cycle_percent);

    /* write PWM control registers */
    lock_multi_register_access(true);

    t_on = utils::set_bits<uint16_t>(t_on, PCA9685_REG_LEDX_ON_OFF_B << 8u);
    write_bytes(PCA9685_REG_LEDX_ON_L(pwm_port), (uint8_t*)&t_on, sizeof(t_on));
    write_bytes(PCA9685_REG_LEDX_OFF_L(pwm_port), (uint8_t*)&t_off, sizeof(t_off));

    lock_multi_register_access(false);

    return true;
}

/**
 * @brief Do a software reset on the PCA9685 chip
 * @param
 */
void pca9685::sw_reset(void)
{
    /* enqueue a read for register addr */
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (PCA9685_DEFAULT_I2C_ADDR_SWRST_7_BIT << 1u) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, 0x06u, I2C_MASTER_ACK);
    i2c_master_stop(cmd);

    /* get status of current address */
    int ret = i2c_master_cmd_begin(_bus->port(), cmd, pdMS_TO_TICKS(_timeout_ms));
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        LOG_ERR("Failed SWRST : rc %d", ret);
    }

    LOG_INFO("Software reset done.");
}

/**
 * @brief Default constructor
 * @param _bus
 */
pca9685::pca9685(const std::shared_ptr<comm::i2c_master_bus>& _bus)
    : i2c_slave_device(_bus, "PCA9685", PCA9685_DEFAULT_I2C_ADDR_7_BIT, PCA9685_DEFAULT_I2C_TIMEOUT_MS)
{
    sw_reset();
    set_freq(50u);

    uint8_t val = 0u;
    if (read_byte(PCA9685_REG_MODE_2, val)) {
        val = utils::set_bits<uint8_t>(val, PCA9685_REG_MODE_2_OUTDRV_B);
        write_byte(PCA9685_REG_MODE_2, val);
    }

    for (int i = 0; i <= 100u; i++) {
        sleep(true);
        set_pwm(0u, i);
        sleep(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(250u));
    }
}

} // namespace drivers