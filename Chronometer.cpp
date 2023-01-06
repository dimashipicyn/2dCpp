#include "stdafx.h"

#include "Chronometer.h"

#ifdef _MSC_VER
#include <windows.h>
#include <timeapi.h>
#endif


Chronometer::Chronometer()
{
#ifdef _MSC_VER
    timeBeginPeriod(1);
#endif
    last_time_ = std::chrono::system_clock::now();
}

Chronometer::~Chronometer()
{
#ifdef _MSC_VER
    timeEndPeriod(1);
#endif
}

float Chronometer::delta_ms()
{
    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time_);
    last_time_ = now;
    return float(elapsed.count());
}

void Chronometer::sleep_until(float time_ms)
{
    using namespace std::chrono_literals;

    if (time_ms > 0) {
        auto now = std::chrono::system_clock::now();
        auto sleep_until_time = std::chrono::milliseconds((int)(time_ms));
        while (sleep_until_time > 1ms) {
            std::this_thread::sleep_for(1ms);
            sleep_until_time -= (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - now));
            now = std::chrono::system_clock::now();
        }
    }
}
