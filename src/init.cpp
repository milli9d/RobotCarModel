#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>
#include <esp_task_wdt.h>

#include <driver/i2c.h>

#include "comm/i2c_master_bus.hpp"
#include "comm/i2c_slave_device.hpp"

#include "devices/battery_mon.hpp"

#include "logging.h"
LOG_TAG(INIT);

#define MAIN_I2C_PORT I2C_NUM_1

#define MAIN_I2C_MASTER_SDA_PIN GPIO_NUM_13 /*!< I2C master SDA pin number */
#define MAIN_I2C_MASTER_SCL_PIN GPIO_NUM_14 /*!< I2C master SCL pin number */

static std::unique_ptr<devices::battery_monitor> batt_mon{};
static std::shared_ptr<comm::i2c_master_bus> i2c_main{};

/**
 * @brief display a welcome banner
 * @param
 */
static void _banner(void)
{
    printf("\t   -           __                       \n"
           "\t --          ~( @\\   \\                \n"
           "\t---   _________]_[__/_>________         \n"
           "\t     /  ____ \\ <>     |  ____  \\      \n"
           "\t    =\\_/ __ \\_\\_______|_/ __ \\__D   \n"
           "\t________(__)_____________(__)____       \n\n");

    printf("     ______  ______  ______     ______  ______  ______  \n"
           "    /\\  ___\\/\\  ___\\/\\  == \\   /\\  == \\/\\  __ \\/\\__  _\\ \n"
           "    \\ \\  __\\\\ \\___  \\ \\  _-/   \\ \\  __<\\ \\ \\/\\ \\/_/\\ \\/ \n"
           "     \\ \\_____\\/\\_____\\ \\_\\      \\ \\_____\\ \\_____\\ \\ \\_\\ \n"
           "      \\/_____/\\/_____/\\/_/       \\/_____/\\/_____/  \\/_/ \n"
           "                                                        \n");
}

/* Public API */
/**
 * @brief
 * @param
 */
void esp_bot_init(void)
{
    esp_task_wdt_config_t cfg = {
        .timeout_ms = 1000u,
        .idle_core_mask = 0x00,
        .trigger_panic = false,
    };
    esp_task_wdt_init(&cfg);

    _banner();

    LOG_INFO("ESP-IDF Version: v%d.%d.%d", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
    LOG_INFO("ESP-BOT Version: v%s", "0.0.1");

    batt_mon = std::make_unique<devices::battery_monitor>(BATTERY_MON_ADC_CHANNEL);
    i2c_main = std::make_shared<comm::i2c_master_bus>(MAIN_I2C_PORT, MAIN_I2C_MASTER_SDA_PIN, MAIN_I2C_MASTER_SCL_PIN);
    i2c_main->scan_devices();

    comm::i2c_slave_device dev(i2c_main, "PCA9685", 0x5Fu, 100u);
    uint8_t val = 0u;
    if (dev.read_byte(0x00, val)) {
        dev.write_byte_rb(0x00, val | (0u << 4u), 100u);
    }
}