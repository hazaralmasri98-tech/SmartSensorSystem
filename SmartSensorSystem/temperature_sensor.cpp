#include "temperature_sensor.h"
#include <random>
#include <utility>

TemperatureSensor::TemperatureSensor(std::string id, double minV, double maxV)
    : id_(std::move(id)), minV_(minV), maxV_(maxV), currentValue_((minV + maxV) / 2.0) {
}

double TemperatureSensor::read() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    currentValue_ += dis(gen);
    if (currentValue_ < minV_) currentValue_ = minV_;
    if (currentValue_ > maxV_) currentValue_ = maxV_;
    return currentValue_;
}

std::string TemperatureSensor::name() const {
    return id_;
}

std::string TemperatureSensor::unit() const {
    return "°C";
}
