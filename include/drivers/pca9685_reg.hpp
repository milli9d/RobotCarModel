#pragma once

#include <iostream>
#include <thread>
#include <cstdint>
#include <cmath>
#include <chrono>

#define PCA9685_DEFAULT_I2C_ADDR_7_BIT       0x5Fu
#define PCA9685_DEFAULT_I2C_ADDR_SWRST_7_BIT 0x00u

/* REG MODE 1 [0x00u] */
#define PCA9685_REG_MODE_1                   0x00u

#define PCA9685_REG_MODE_1_ALLCALL           (1u << 0u)
#define PCA9685_REG_MODE_1_SUB3_B            (1u << 1u)
#define PCA9685_REG_MODE_1_SUB2_B            (1u << 2u)
#define PCA9685_REG_MODE_1_SUB1_B            (1u << 3u)
#define PCA9685_REG_MODE_1_SLEEP_B           (1u << 4u)
#define PCA9685_REG_MODE_1_AI_B              (1u << 5u)
#define PCA9685_REG_MODE_1_EXTCLK_B          (1u << 6u)
#define PCA9685_REG_MODE_1_RESTART_B         (1u << 7u)

/* REG MODE 2 [0x01u] */
#define PCA9685_REG_MODE_2                   0x01u

#define PCA9685_REG_MODE_2_OUTNE_2B(x)       (x << 0u)
#define PCA9685_REG_MODE_2_OUTDRV_B          (1u << 2u)
#define PCA9685_REG_MODE_2_OCH_B             (1u << 3u)
#define PCA9685_REG_MODE_2_INVRT_B           (1u << 4u)

/* LEDX registers [Xmax = 15u] */
#define PCA9685_REG_LED_VAL_MAX              pow(2u, 12u)
#define PCA9685_REG_LEDX_REG_COUNT           (4u)
#define PCA9685_REG_LEDX_BASE_ADDR           (0x06u)
#define PCA9685_REG_LEDX_BASE(i)             (PCA9685_REG_LEDX_BASE_ADDR + (i * PCA9685_REG_LEDX_REG_COUNT))

#define PCA9685_REG_LEDX_ON_L(i)             (PCA9685_REG_LEDX_BASE(i) + 0u)
#define PCA9685_REG_LEDX_ON_H(i)             (PCA9685_REG_LEDX_BASE(i) + 1u)
#define PCA9685_REG_LEDX_OFF_L(i)            (PCA9685_REG_LEDX_BASE(i) + 2u)
#define PCA9685_REG_LEDX_OFF_H(i)            (PCA9685_REG_LEDX_BASE(i) + 3u)

#define PCA9685_REG_LEDX_H(x)                (x << 0u)
#define PCA9685_REG_LEDX_ON_OFF_B            (1u << 4u)

/* ALL_LED registers */
#define PCA9685_REG_ALL_LEDX_BASE_ADDR       (0xFAu)

#define PCA9685_REG_ALL_LEDX_ON_L            (PCA9685_REG_ALL_LEDX_BASE_ADDR + 0u)
#define PCA9685_REG_ALL_LEDX_ON_H            (PCA9685_REG_ALL_LEDX_BASE_ADDR + 1u)
#define PCA9685_REG_ALL_LEDX_OFF_L           (PCA9685_REG_ALL_LEDX_BASE_ADDR + 2u)
#define PCA9685_REG_ALL_LEDX_OFF_H           (PCA9685_REG_ALL_LEDX_BASE_ADDR + 3u)

/* Pre-Scaler Register */

#define PCA9685_REG_PRE_SCALE                (0xFEu)
#define PCA9685_REG_PRE_SCALE_MIN            (0x03u)
#define PCA9685_REG_PRE_SCALE_MAX            (0xFFu)

#define PCA9685_INT_CLK_FREQ_HZ              (25u * 1000u * 1000u)

#define PCA9685_CLK_FREQ_HZ                  (PCA9685_INT_CLK_FREQ_HZ)

#define PCA9685_PRE_SCALE_FREQ_HZ_RES        (PCA9685_CLK_FREQ_HZ / 4096u)

#define PCA9685_FREQ_HZ_TO_PRE_SCALE(x)      ((PCA9685_CLK_FREQ_HZ / (4096u * x)) - 1u)
#define PCA9685_PRE_SCALE_TO_FREQ_HZ(x)      (PCA9685_CLK_FREQ_HZ / (4096u * (x + 1u)))

#define PCA9685_REG_PRE_SCALE_MAX_FREQ_HZ    ((PCA9685_CLK_FREQ_HZ / (4096u * (PCA9685_REG_PRE_SCALE_MIN + 1u))) + 1u)
#define PCA9685_REG_PRE_SCALE_MIN_FREQ_HZ    ((PCA9685_CLK_FREQ_HZ / (4096u * (PCA9685_REG_PRE_SCALE_MAX + 1u))) + 1u)