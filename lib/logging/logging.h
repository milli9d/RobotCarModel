/**
 * @brief Logging API
 *
 * @file logging.h
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __LOGGING_H_
#define __LOGGING_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#define LOG_LEVEL_ERROR 0u
#define LOG_LEVEL_WARN  1u
#define LOG_LEVEL_FAIL  2u
#define LOG_LEVEL_PASS  3u
#define LOG_LEVEL_INFO  4u
#define LOG_LEVEL_DEBUG 5u

#define LOG_TAG(str) static const char* sTag = #str;

#define LOG_ERR(fmt, ...)  text_log(LOG_LEVEL_ERROR, sTag, __func__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) text_log(LOG_LEVEL_WARN, sTag, __func__, fmt, ##__VA_ARGS__)
#define LOG_FAIL(fmt, ...) text_log(LOG_LEVEL_FAIL, sTag, __func__, fmt, ##__VA_ARGS__)
#define LOG_PASS(fmt, ...) text_log(LOG_LEVEL_PASS, sTag, __func__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) text_log(LOG_LEVEL_INFO, sTag, __func__, fmt, ##__VA_ARGS__)
#define LOG_DBG(fmt, ...)  text_log(LOG_LEVEL_DEBUG, sTag, __func__, fmt, ##__VA_ARGS__)

/**
 * @brief Underlying text log API
 * @param log_level
 * @param tag
 * @param func
 * @param fmt
 * @param
 */
void text_log(uint8_t log_level, const char* tag, const char* func, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __LOGGING_H_ */
