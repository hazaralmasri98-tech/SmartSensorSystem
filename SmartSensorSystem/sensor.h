#pragma once
#include <string>

class Sensor {
public:
    virtual ~Sensor() = default;
    virtual double read() = 0;
    virtual std::string name() const = 0;
    virtual std::string unit() const = 0;
};

