#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include <esp_vfs_dev.h>
#include <esp_console.h>
#include <driver/uart.h>
#include <linenoise/linenoise.h>
#include <argtable3/argtable3.h>

namespace utils {

class esp_console
{
  private:
    /* private data members */
    std::unique_ptr<std::thread> _thread;

    /* private API */
    static void _run();
    static void _configure_uart(void);

  public:
    esp_console(void);
};

} // namespace utils