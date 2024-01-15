#include <iostream>
#include <thread>
#include <chrono>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>

#include "logging.h"
LOG_TAG(BATTERY_MON);

#include "devices/battery_mon.hpp"

namespace devices {

/**
 * @brief Read battery voltage
 *
 * The voltage acquisition range of GPIO32 on ESP32 is 0-3.3V, while the car is powered by two 18650 lithium batteries,
 * whose voltage is 8.4V when they are fully charged, which exceeds the acquisition range of ESP32.
 *
 * Therefore, after passing through the voltage divider circuit composed of R3 and R4, the voltage at A0/WS2812 will be
 * about 1/4 of the battery voltage, 8.4/4=2.1V, which is within the voltage collection range of GPIO32
 *
 * @param mon
 * @return
 */
void battery_monitor::update()
{
    int sum = 0;
    for (int i = 0; i < BATTERY_MON_ANTI_ALIASING_FACTOR; i++) {
        int out = 0;
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_read(_handle, _chan, &out));
        std::this_thread::sleep_for(std::chrono::milliseconds(50u));
        sum += out;
    }
    sum /= BATTERY_MON_ANTI_ALIASING_FACTOR;
    _volt = (sum * BATTERY_MON_ADC_ATTEN_LEVEL_VMAX / BATTERY_MON_ADC_BIT_MAX) * BATTERY_MON_V_DIVIDER_COEFF;
}

/**
 * @brief read battery voltage
 * @return
 */
double inline battery_monitor::voltage()
{
    return _volt;
}

/**
 * @brief read battery percent
 * @return
 */
double inline battery_monitor::percent()
{
    return ((_volt / BATTERY_MON_BATTERY_VMAX) * 100.0);
}

/**
 * @brief Runnable section of battery monitor
 */
void battery_monitor::_run(battery_monitor* mon)
{
    while (1) {
        // LOG_INFO("Battery monitor!");
        mon->update();

        if (mon->voltage() < BATTERY_MON_VOLTAGE_THRESHOLD_MV) {
            LOG_WARN("Battery Monitor : Battery not present or discharged!");
        } else {
            LOG_INFO("Battery Monitor : %.3lf mV %.2f %%", mon->voltage(), mon->percent());
        }

        std::this_thread::sleep_for(std::chrono::seconds(BATTERY_MON_PERIOD_S));
    }
}

/**
 * @brief Default constructor for battery monitor
 *
 * Configures ADC for battery monitor
 * @param chan
 */
battery_monitor::battery_monitor(adc_channel_t chan) : _chan(chan)
{
    /* init adc oneshot unit */
    adc_oneshot_unit_init_cfg_t _cfg{};
    _cfg.unit_id = BATTERY_MON_ADC_UNIT;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&_cfg, &_handle));

    /* init adc oneshot channel */
    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = BATTERY_MON_ADC_ATTEN_LEVEL,
        .bitwidth = BATTERY_MON_ADC_BIT_WIDTH,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(_handle, _chan, &chan_cfg));

    /* log result */
    _thread = std::make_unique<std::thread>(this->_run, this);
    LOG_INFO("Battery Monitor inited on Channel %d", _chan);
}

} // namespace devices
