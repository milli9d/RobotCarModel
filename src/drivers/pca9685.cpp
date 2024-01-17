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
 * @brief Default constructor
 * @param _bus
 */
pca9685::pca9685(const std::shared_ptr<comm::i2c_master_bus>& _bus)
{
    /* init the device */
    _dev = std::make_unique<comm::i2c_slave_device>(_bus, "PCA9685", PCA9685_DEFAULT_I2C_ADDR_7_BIT,
                                                    PCA9685_DEFAULT_I2C_TIMEOUT_MS);

    /* init powered down */
    // this->sleep(true);
    // std::cout << "Sleep " << this->sleep() << std::endl;
    this->sleep(false);
    this->set_pwm(0u, 50u);
    this->set_pwm(1u, 0u);
}

/**
 * @brief Set sleep mode
 * @param sleep
 */
void pca9685::sleep(bool sleep)
{
    uint8_t val = 0u;
    if (!_dev->read_byte(PCA9685_REG_MODE_1, val)) {
        LOG_ERR("Error reading MODE_1 val.");
    }

    /* try a read back write */
    if (_dev->write_byte_rb(PCA9685_REG_MODE_1, val | PCA9685_REG_MODE_1_SLEEP_B((int)sleep), 100u)) {
        LOG_INFO("PCA9685 Sleep %s", ((sleep == 1u) ? "enabled" : "disabled"));
    } else {
        LOG_INFO("ERROR: writing sleep mode failed");
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
    if (!_dev->read_byte(PCA9685_REG_MODE_1, val)) {
        LOG_ERR("Error reading MODE_1 val.");
    }

    /* compare bit */
    return (val & PCA9685_REG_MODE_1_SLEEP_B(1u));
}

bool pca9685::set_pwm(uint8_t pwm_port, uint8_t duty_cycle_percent)
{
    /* calculate time of PWM */
    uint16_t t_on = (static_cast<uint16_t>(duty_cycle_percent) * static_cast<uint16_t>(PCA9685_REG_LED_VAL_MAX)) / 100u;
    uint16_t t_off = (static_cast<uint16_t>(PCA9685_REG_LED_VAL_MAX) - t_on - 1u);

    LOG_INFO("PWM On = %d, Off = %d , val = %d", t_on, t_off, (uint32_t)duty_cycle_percent);

    _dev->write_bytes(PCA9685_REG_LEDX_ON_L(pwm_port), (uint8_t*)&t_on, sizeof(t_on));
    _dev->write_bytes(PCA9685_REG_LEDX_OFF_L(pwm_port), (uint8_t*)&t_off, sizeof(t_off));

    /* restart PWM */
    uint8_t val = 0u;
    _dev->read_byte(PCA9685_REG_MODE_1, val);
    _dev->write_byte(PCA9685_REG_MODE_1, val | PCA9685_REG_MODE_1_RESTART(1u));
    return true;
}

} // namespace drivers