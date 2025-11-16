#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <optional>

#include "measurement.h"

class Sensor;  // framåtreferens

struct Threshold {
    double limit;
    bool over;  // true: larma om > limit, false: larma om < limit
};

struct Alert {
    std::chrono::system_clock::time_point timestamp;
    std::string sensorName;
    double value;
    std::string rule;
};

class SystemController {
public:
    void addSensor(std::unique_ptr<Sensor> s);

    void sampleAllOnce();                           // läs alla sensorer
    void configureThreshold();                      // ställ in tröskel
    void showAlerts() const;                        // visa larm
    void showStatsFor(const std::string& name) const;
    void showAllMeasurements() const;

    void saveToFile(const std::string& path) const;
    void loadFromFile(const std::string& path);

    // VG: Histogram
    void showHistogramForSensor(const std::string& sensorName) const;

    // VG: Sökning
    void searchMeasurements(
        const std::string& sensorFilter,
        std::optional<std::chrono::system_clock::time_point> startTime,
        std::optional<std::chrono::system_clock::time_point> endTime
    ) const;

private:
    std::vector<std::unique_ptr<Sensor>> sensors_;
    std::vector<Measurement> measurements_;
    std::map<std::string, Threshold> thresholds_;
    std::vector<Alert> alerts_;

    std::string getSensorUnit(const std::string& sensorName) const;
};
