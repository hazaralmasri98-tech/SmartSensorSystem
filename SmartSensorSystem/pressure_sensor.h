#pragma once
#include <string>
#include "sensor.h"

class PressureSensor : public Sensor {
public:
    PressureSensor(std::string id, double minV, double maxV);

    double read() override;
    std::string name() const override;
    std::string unit() const override;

private:
    std::string id_;
    double minV_, maxV_;
    double currentValue_;
};
