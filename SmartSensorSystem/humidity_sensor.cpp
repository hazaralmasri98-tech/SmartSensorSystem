#include "humidity_sensor.h"
#include <random>
#include <utility>

HumiditySensor::HumiditySensor(std::string id, double minV, double maxV)
    : id_(std::move(id)), minV_(minV), maxV_(maxV), currentValue_((minV + maxV) / 2.0) {
}

double HumiditySensor::read() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-2.0, 2.0);
    currentValue_ += dis(gen);
    if (currentValue_ < minV_) currentValue_ = minV_;
    if (currentValue_ > maxV_) currentValue_ = maxV_;
    return currentValue_;
}

std::string HumiditySensor::name() const {
    return id_;
}

std::string HumiditySensor::unit() const {
    return "%";
}
