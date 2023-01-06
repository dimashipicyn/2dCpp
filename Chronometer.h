#pragma once

#include <chrono>

class Chronometer
{
public:
    Chronometer();
    ~Chronometer();

    float delta_ms();
    void sleep_until(float time_ms);

private:
    std::chrono::system_clock::time_point last_time_;
};

