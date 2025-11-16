#include "system_controller.h"
#include "sensor.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <numeric>


static std::tm to_tm(std::chrono::system_clock::time_point tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &t);
#endif
    return tm;
}

void SystemController::addSensor(std::unique_ptr<Sensor> s) {
    sensors_.push_back(std::move(s));
}

void SystemController::sampleAllOnce() {
    auto now = std::chrono::system_clock::now();

    for (const auto& sensor : sensors_) {
        double value = sensor->read();
        measurements_.push_back({ sensor->name(), value, now });

        // kolla tröskel
        auto it = thresholds_.find(sensor->name());
        if (it != thresholds_.end()) {
            const auto& t = it->second;
            bool trigger =
                (t.over && value > t.limit) ||
                (!t.over && value < t.limit);

            if (trigger) {
                std::ostringstream rule;
                rule << (t.over ? "Over " : "Under ")
                    << t.limit << sensor->unit();

                alerts_.push_back({ now, sensor->name(), value, rule.str() });
            }
        }
    }
}

void SystemController::configureThreshold() {
    std::cout << "Enter sensor name: ";
    std::string name;
    std::cin >> name;

    std::cout << "Enter limit: ";
    double limit;
    std::cin >> limit;

    std::cout << "Alarm if over (1) or under (0)? ";
    int x;
    std::cin >> x;
    bool over = (x == 1);

    thresholds_[name] = { limit, over };
}

void SystemController::showAlerts() const {
    if (alerts_.empty()) {
        std::cout << "No alerts.\n";
        return;
    }

    for (const auto& a : alerts_) {
        std::tm tm = to_tm(a.timestamp);
        std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
            << " | " << a.sensorName
            << " | " << a.value
            << " | " << a.rule << "\n";
    }
}

void SystemController::showStatsFor(const std::string& sensorName) const {
    std::vector<double> values;
    for (const auto& m : measurements_) {
        if (m.sensorName == sensorName)
            values.push_back(m.value);
    }

    if (values.empty()) {
        std::cout << "No data for " << sensorName << "\n";
        return;
    }

    double minV = *std::min_element(values.begin(), values.end());
    double maxV = *std::max_element(values.begin(), values.end());
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    double avg = sum / values.size();

    std::cout << "Stats for " << sensorName << ":\n"
        << "  Min: " << minV << "\n"
        << "  Max: " << maxV << "\n"
        << "  Avg: " << avg << "\n";
}

void SystemController::showAllMeasurements() const {
    for (const auto& m : measurements_) {
        std::tm tm = to_tm(m.timestamp);
        std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
            << " | " << m.value
            << " | " << m.sensorName << "\n";
    }
}

void SystemController::saveToFile(const std::string& path) const {
    std::ofstream file(path);
    if (!file) {
        std::cerr << "Could not open file for writing: " << path << "\n";
        return;
    }

    for (const auto& m : measurements_) {
        std::tm tm = to_tm(m.timestamp);
        file << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
            << "," << m.value
            << "," << m.sensorName << "\n";
    }
}

void SystemController::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file for reading: " << path << "\n";
        return;
    }

    measurements_.clear();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string timeStr, valueStr, name;

        std::getline(iss, timeStr, ',');
        std::getline(iss, valueStr, ',');
        std::getline(iss, name);

        std::tm tm = {};
        std::istringstream timeIss(timeStr);
        timeIss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        double value = std::stod(valueStr);

        measurements_.push_back({ name, value, tp });
    }
}

std::string SystemController::getSensorUnit(const std::string& sensorName) const {
    for (const auto& s : sensors_) {
        if (s->name() == sensorName)
            return s->unit();
    }
    return "";
}



void SystemController::showHistogramForSensor(const std::string& sensorName) const {
    std::vector<double> values;
    for (const auto& m : measurements_) {
        if (m.sensorName == sensorName)
            values.push_back(m.value);
    }

    if (values.empty()) {
        std::cout << "No measurements for " << sensorName << "\n";
        return;
    }

    double minVal = *std::min_element(values.begin(), values.end());
    double maxVal = *std::max_element(values.begin(), values.end());

    int numBins = 10;
    double range = maxVal - minVal;
    double binWidth = (range == 0.0) ? 1.0 : (range / numBins);

    std::vector<int> counts(numBins, 0);
    for (double v : values) {
        int idx = static_cast<int>((v - minVal) / binWidth);
        if (idx >= numBins) idx = numBins - 1;
        if (idx < 0) idx = 0;
        counts[idx]++;
    }

    std::string unit = getSensorUnit(sensorName);

    std::cout << "Histogram for " << sensorName << ":\n";
    for (int i = 0; i < numBins; ++i) {
        double start = minVal + i * binWidth;
        double end = start + binWidth;
        std::cout << std::fixed << std::setprecision(1)
            << start << "-" << end << unit << " | ";
        for (int j = 0; j < counts[i]; ++j)
            std::cout << "*";
        std::cout << "\n";
    }
}


void SystemController::searchMeasurements(
    const std::string& sensorFilter,
    std::optional<std::chrono::system_clock::time_point> startTime,
    std::optional<std::chrono::system_clock::time_point> endTime
) const {
    std::vector<const Measurement*> results;

    for (const auto& m : measurements_) {
        bool okSensor = sensorFilter.empty() || m.sensorName == sensorFilter;
        bool okTime =
            (!startTime || m.timestamp >= *startTime) &&
            (!endTime || m.timestamp <= *endTime);

        if (okSensor && okTime) {
            results.push_back(&m);
        }
    }

    if (results.empty()) {
        std::cout << "No matches.\n";
        return;
    }

    for (const auto* m : results) {
        std::tm tm = to_tm(m->timestamp);
        std::string unit = getSensorUnit(m->sensorName);
        std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
            << " | " << m->value << unit
            << " | " << m->sensorName << "\n";
    }
}
