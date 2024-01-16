#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdint>

#include "devices/led.hpp"

namespace tasks {

class status_led
{
  private:
    std::unique_ptr<std::thread> _thread{};

    /* private API */
    static void _run(status_led* _this);

  public:
    status_led(void);
};

} // namespace tasks