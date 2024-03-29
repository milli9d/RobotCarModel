/**
 * @file init.cpp
 *
 * @author Milind Singh (milind345@gmail.com)
 *
 * Initiliaze ESP BOT system software
 *
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>
#include <esp_pm.h>
#include <esp_task_wdt.h>

#include <driver/i2c.h>

#include "comm/i2c_master_bus.hpp"
#include "comm/i2c_slave_device.hpp"

#include "drivers/pca9685.hpp"

#include "tasks/battery_mon.hpp"
#include "tasks/led_thread.hpp"

#include "utils/console.hpp"

#include "logging.h"

/* ========================================================================= */
/* Definitions */
/* ========================================================================= */

LOG_TAG(INIT);

#define MAIN_I2C_PORT I2C_NUM_1

#define MAIN_I2C_MASTER_SDA_PIN GPIO_NUM_13 /*!< I2C master SDA pin number */
#define MAIN_I2C_MASTER_SCL_PIN GPIO_NUM_14 /*!< I2C master SCL pin number */

/* ========================================================================= */
/* ESP CONSOLE */
/* ========================================================================= */

/* ========================================================================= */
/* Static Members */
/* ========================================================================= */

static void inline _wdt_init();
static void inline _banner(void);

static std::unique_ptr<tasks::status_led> status_led{};
static std::unique_ptr<devices::battery_monitor> batt_mon{};
static std::unique_ptr<utils::esp_console> console{};
static std::unique_ptr<drivers::pca9685> pca9685{};

static std::shared_ptr<comm::i2c_master_bus> i2c_main{};

/* ========================================================================= */
/* Private API */
/* ========================================================================= */

/**
 * @brief Power Management
 */
static void inline _pm_init(void)
{
    /* set PM to auto downclock and sleep */
    esp_pm_config_t _cfg = {
        .max_freq_mhz = 160000,
        .min_freq_mhz = CONFIG_XTAL_FREQ * 1000u,
        .light_sleep_enable = true,
    };
    esp_pm_configure(&_cfg);
}

/**
 * @brief Task watchdog init
 */
static void inline _wdt_init(void)
{
    /* configure the global timeout */
    esp_task_wdt_config_t cfg = {
        .timeout_ms = 1000u,
        .idle_core_mask = 0x00,
        .trigger_panic = true,
    };
    esp_task_wdt_init(&cfg);
}

/**
 * @brief display a welcome banner
 * @param
 */
static void inline _banner(void)
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

/**
 * @brief Log version
 */
static void inline _version()
{
    LOG_INFO("ESP-IDF Version: v%d.%d.%d", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
    LOG_INFO("ESP-BOT Version: v%s", "0.0.1");
}

/* ========================================================================= */
/* Public API */
/* ========================================================================= */

static void tb()
{
    i2c_main->scan_devices();
}

/**
 * @brief
 * @param
 */
void esp_bot_init(void)
{
    /* init ESP32 subsys */
    _wdt_init();
    _pm_init();
    _banner();
    _version();

    /* initialize status led */
    status_led = std::make_unique<tasks::status_led>();

    /* initialize battery monitor */
    batt_mon = std::make_unique<devices::battery_monitor>(BATTERY_MON_ADC_CHANNEL);

    /* initialize esp console */
    // console = std::make_unique<utils::esp_console>();

    /* initialize i2c master bus */
    i2c_main = std::make_shared<comm::i2c_master_bus>(MAIN_I2C_PORT, MAIN_I2C_MASTER_SDA_PIN, MAIN_I2C_MASTER_SCL_PIN);

    pca9685 = std::make_unique<drivers::pca9685>(i2c_main);

    /* @todo remove after dev */
    tb();
}