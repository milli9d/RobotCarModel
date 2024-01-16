#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include <esp_adc/adc_oneshot.h>

namespace devices {

#define BATTERY_MON_VOLTAGE_THRESHOLD_MV 2100.0
#define BATTERY_MON_PERIOD_S             120u

#define BATTERY_MON_ADC_UNIT    ADC_UNIT_1
#define BATTERY_MON_ADC_CHANNEL ADC_CHANNEL_4

#define BATTERY_MON_ADC_ATTEN_LEVEL      ADC_ATTEN_DB_11
#define BATTERY_MON_ADC_ATTEN_LEVEL_VMAX 2450.0
#define BATTERY_MON_BATTERY_VMAX         8400.0

#define BATTERY_MON_ADC_BIT_WIDTH ADC_BITWIDTH_12
#define BATTERY_MON_ADC_BIT_MAX   pow(2, ADC_BITWIDTH_12)

#define BATTERY_MON_ANTI_ALIASING_FACTOR 10u
#define BATTERY_MON_V_DIVIDER_COEFF      4.0

class battery_monitor
{
  private:
    adc_oneshot_unit_handle_t _handle{};

    adc_channel_t _chan{};
    std::unique_ptr<std::thread> _thread{};
    double _volt{};

    /**
     * @brief Update battery voltage
     *
     * The voltage acquisition range of GPIO32 on ESP32 is 0-3.3V, while the car is powered by two 18650 lithium
     * batteries, whose voltage is 8.4V when they are fully charged, which exceeds the acquisition range of ESP32.
     *
     * Therefore, after passing through the voltage divider circuit composed of R3 and R4, the voltage at A0/WS2812 will
     * be about 1/4 of the battery voltage, 8.4/4=2.1V, which is within the voltage collection range of GPIO32
     *
     * @param mon
     * @return
     */
    void update();

    /**
     * @brief read battery voltage
     * @return
     */
    double inline voltage();

    /**
     * @brief read battery percent
     * @return
     */
    double inline percent();

    /**
     * @brief Runnable section of battery monitor
     */
    static void _run(battery_monitor* mon);

  public:
    explicit battery_monitor(adc_channel_t chan);
};

} // namespace devices