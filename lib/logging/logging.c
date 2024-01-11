/**
 * @brief Logging API
 *
 * @file logging.c
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "colors.h"
#include "logging.h"

#define GLOBAL_LOG_LEVEL LOG_LEVEL_DEBUG

extern const char* sTag;

/**
 * @brief logging API base function
 * @param log_level
 * @param func
 * @param fmt
 * @param
 */
void text_log(uint8_t log_level, const char* tag, const char* func, const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);

    /* filter by log level */
    if (log_level > GLOBAL_LOG_LEVEL) {
        return;
    }

    /* append time stamp */
    time_t now = time(NULL);
    char time_buf[64u] = "";
    strftime(time_buf, 64u, COLOR_CYAN "[%Y/%m/%d %02I:%02M:%02S %p] ", localtime(&now));
    printf("%s", time_buf);

    /* append tag */
    switch (log_level) {
        case LOG_LEVEL_PASS:
            printf(COLOR_HIGH_GREEN "%-6s" COLOR_HIGH_GREEN, "<PASS>");
            break;
        case LOG_LEVEL_FAIL:
            printf(COLOR_HIGH_RED "%-6s" COLOR_HIGH_RED, "<FAIL>");
            break;
        case LOG_LEVEL_INFO:
            printf(COLOR_HIGH_WHITE "%-6s" COLOR_RESET, "<INFO>");
            break;
        case LOG_LEVEL_WARN:
            printf(COLOR_HIGH_YELLOW "%-6s", "<WARN>");
            break;
        case LOG_LEVEL_ERROR:
            printf(COLOR_HIGH_RED "%-6s", "<ERR>");
            break;
        case LOG_LEVEL_DEBUG:
            printf(COLOR_HIGH_GREEN "%-6s", "<DBG>");
            break;
        default:
            break;
    }

    /* print function name */
    if (tag) {
        printf(" [%s | %s]: ", tag, func);
    }
    vprintf(fmt, list);
    printf("\n" COLOR_RESET);

    va_end(list);
}

#ifdef __cplusplus
}
#endif
