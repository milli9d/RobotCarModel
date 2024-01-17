#pragma once

#include <iostream>
#include <thread>
#include <cstdint>
#include <cmath>
#include <chrono>

#define PCA9685_DEFAULT_I2C_ADDR_7_BIT          0x5Fu
#define PCA9685_DEFAULT_I2C_ADDR_SWRST_7_BIT    0x00u

/* REG MODE 1 [0x00u] */
#define PCA9685_REG_MODE_1             0x00u

#define PCA9685_REG_MODE_1_ALLCALL(x)  (x << 0u)
#define PCA9685_REG_MODE_1_SUB3_B(x)   (x << 1u)
#define PCA9685_REG_MODE_1_SUB2_B(x)   (x << 2u)
#define PCA9685_REG_MODE_1_SUB1_B(x)   (x << 3u)
#define PCA9685_REG_MODE_1_SLEEP_B(x)  (x << 4u)
#define PCA9685_REG_MODE_1_AI_B(x)     (x << 5u)
#define PCA9685_REG_MODE_1_EXTCLK(x)   (x << 6u)
#define PCA9685_REG_MODE_1_RESTART(x)  (x << 7u)

/* REG MODE 2 [0x01u] */
#define PCA9685_REG_MODE_2             0x01u

#define PCA9685_REG_MODE_2_OUTNE_2B(x) (x << 0u)
#define PCA9685_REG_MODE_2_OUTDRV_B(x) (x << 2u)
#define PCA9685_REG_MODE_2_OCH_B(x)    (x << 3u)
#define PCA9685_REG_MODE_2_INVRT_B(x)  (x << 4u)

/* LEDX registers [Xmax = 15u] */
#define PCA9685_REG_LED_VAL_MAX        pow(2u, 12u)
#define PCA9685_REG_LEDX_REG_COUNT     (4u)
#define PCA9685_REG_LEDX_BASE_ADDR     (0x06u)
#define PCA9685_REG_LEDX_BASE(i)       (PCA9685_REG_LEDX_BASE_ADDR + (i * PCA9685_REG_LEDX_REG_COUNT))

#define PCA9685_REG_LEDX_ON_L(i)       (PCA9685_REG_LEDX_BASE(i) + 0u)
#define PCA9685_REG_LEDX_ON_H(i)       (PCA9685_REG_LEDX_BASE(i) + 1u)
#define PCA9685_REG_LEDX_OFF_L(i)      (PCA9685_REG_LEDX_BASE(i) + 2u)
#define PCA9685_REG_LEDX_OFF_H(i)      (PCA9685_REG_LEDX_BASE(i) + 3u)

/* ALL_LED registers */
#define PCA9685_REG_ALL_LEDX_BASE_ADDR (0xFAu)

#define PCA9685_REG_ALL_LEDX_ON_L      (PCA9685_REG_ALL_LEDX_BASE_ADDR + 0u)
#define PCA9685_REG_ALL_LEDX_ON_H      (PCA9685_REG_ALL_LEDX_BASE_ADDR + 1u)
#define PCA9685_REG_ALL_LEDX_OFF_L     (PCA9685_REG_ALL_LEDX_BASE_ADDR + 2u)
#define PCA9685_REG_ALL_LEDX_OFF_H     (PCA9685_REG_ALL_LEDX_BASE_ADDR + 3u)
#define PCA9685_REG_PRE_SCALE          (PCA9685_REG_ALL_LEDX_BASE_ADDR + 4u)
