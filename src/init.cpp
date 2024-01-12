#include <iostream>
#include <thread>
#include <chrono>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include <driver/i2c.h>

#include "comm/i2c_master_bus.hpp"
#include "devices/battery_mon.hpp"

#include "logging.h"
LOG_TAG(INIT);

#define MAIN_I2C_PORT I2C_NUM_1

#define MAIN_I2C_MASTER_SDA_PIN GPIO_NUM_13 /*!< I2C master SDA pin number */
#define MAIN_I2C_MASTER_SCL_PIN GPIO_NUM_14 /*!< I2C master SCL pin number */

static std::unique_ptr<devices::battery_monitor> batt_mon{};
static std::unique_ptr<comm::i2c_master_bus> i2c_main{};

/**
 * @brief display a welcome banner
 * @param
 */
static void _banner(void)
{
    printf("     ______  ______  ______     ______  ______  ______  \n"
           "    /\\  ___\\/\\  ___\\/\\  == \\   /\\  == \\/\\  __ \\/\\__  _\\ \n"
           "    \\ \\  __\\\\ \\___  \\ \\  _-/   \\ \\  __<\\ \\ \\/\\ \\/_/\\ \\/ \n"
           "     \\ \\_____\\/\\_____\\ \\_\\      \\ \\_____\\ \\_____\\ \\ \\_\\ \n"
           "      \\/_____/\\/_____/\\/_/       \\/_____/\\/_____/  \\/_/ \n"
           "                                                        \n");
    LOG_INFO("ESP-IDF Version: v%d.%d.%d", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
    LOG_INFO("ESP-BOT Version: v%s", "0.0.1");
}

/* Public API */
/**
 * @brief
 * @param
 */
void esp_bot_init(void)
{
    _banner();

    batt_mon = std::make_unique<devices::battery_monitor>(BATTERY_MON_ADC_CHANNEL);

    i2c_main = std::make_unique<comm::i2c_master_bus>(MAIN_I2C_PORT, MAIN_I2C_MASTER_SDA_PIN, MAIN_I2C_MASTER_SCL_PIN);
    i2c_main->scan_devices();
}