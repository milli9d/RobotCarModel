#include <iostream>
#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#define _TAG_INIT "INIT"

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
    printf("ESP-IDF Version: v%d.%d.%d\n", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
    printf("ESP-BOT Version: v%s\n", "0.0.1");
}

/* Public API */

void esp_bot_init(void)
{
    _banner();
}
