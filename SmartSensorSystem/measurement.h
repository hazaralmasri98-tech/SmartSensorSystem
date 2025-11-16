#pragma once
#include <string>
#include <chrono>

struct Measurement {
    std::string sensorName;
    double value;
    std::chrono::system_clock::time_point timestamp;
};
