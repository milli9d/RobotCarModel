#include <iostream>
#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>

#define _TAG_INIT "Main"

static void _banner(void)
{
    printf("     ______  ______  ______     ______  ______  ______  \n"
           "    /\\  ___\\/\\  ___\\/\\  == \\   /\\  == \\/\\  __ \\/\\__  _\\ \n"
           "    \\ \\  __\\\\ \\___  \\ \\  _-/   \\ \\  __<\\ \\ \\/\\ \\/_/\\ \\/ \n"
           "     \\ \\_____\\/\\_____\\ \\_\\      \\ \\_____\\ \\_____\\ \\ \\_\\ \n"
           "      \\/_____/\\/_____/\\/_/       \\/_____/\\/_____/  \\/_/ \n"
           "                                                        \n");
    printf("ESP-IDF Version: %d.%d.%d\\n", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
    printf("ESP-BOT Version: %s\\n", "0.0");
}

/* Public API */

void esp_bot_init(void)
{
    _banner();
}
