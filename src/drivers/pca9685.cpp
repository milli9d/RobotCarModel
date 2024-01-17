#include <iostream>
#include <thread>
#include <chrono>

#include "comm/i2c_master_bus.hpp"
#include "comm/i2c_slave_device.hpp"
#include "drivers/pca9685.hpp"
#include "drivers/pca9685_reg.hpp"

#include "logging.h"
LOG_TAG(PCA9685);

namespace drivers {

/**
 * @brief Lock auto-increment of registers
 * @param lock
 */
bool pca9685::lock_multi_register_access(bool lock)
{
    return append_byte(PCA9685_REG_MODE_1, PCA9685_REG_MODE_1_AI_B((int)lock));
}

/**
 * @brief Set sleep mode
 * @param sleep
 */
void pca9685::sleep(bool sleep)
{
    /* try a read back write */
    if (write_byte_rb(PCA9685_REG_MODE_1, PCA9685_REG_MODE_1_SLEEP_B(sleep ? 1u : 0u), 100u)) {
        LOG_DBG("PCA9685 Sleep %s", ((sleep == 1u) ? "enabled" : "disabled"));
    } else {
        LOG_ERR("ERROR: writing sleep mode failed");
    }
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
    return (val & PCA9685_REG_MODE_1_SLEEP_B(1u));
}

bool pca9685::set_pwm(uint8_t pwm_port, uint8_t duty_cycle_percent)
{
    /* calculate time of PWM */
    uint16_t t_on = (static_cast<uint16_t>(duty_cycle_percent) * static_cast<uint16_t>(PCA9685_REG_LED_VAL_MAX)) / 100u;
    uint16_t t_off = (static_cast<uint16_t>(PCA9685_REG_LED_VAL_MAX) - t_on - 1u);

    LOG_DBG("PWM On = %d, Off = %d , val = %d", t_on, t_off, (uint32_t)duty_cycle_percent);

    // lock_multi_register_access();

    write_bytes(PCA9685_REG_LEDX_ON_L(pwm_port), (uint8_t*)&t_on, sizeof(t_on));

    // write_bytes(PCA9685_REG_LEDX_OFF_L(pwm_port), (uint8_t*)&t_off, sizeof(t_off));

    /* restart PWM */
    // append_byte(PCA9685_REG_MODE_1, PCA9685_REG_MODE_1_RESTART(1u) | PCA9685_REG_MODE_1_AI_B(0u));

    return true;
}

/**
 * @brief Default constructor
 * @param _bus
 */
pca9685::pca9685(const std::shared_ptr<comm::i2c_master_bus>& _bus)
    : i2c_slave_device(_bus, "PCA9685", PCA9685_DEFAULT_I2C_ADDR_7_BIT, PCA9685_DEFAULT_I2C_TIMEOUT_MS)
{
    sleep(true);
}

} // namespace drivers