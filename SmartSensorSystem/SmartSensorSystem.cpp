// SmartSensorSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "system_controller.h"
#include "temperature_sensor.h"
#include "humidity_sensor.h"
#include "pressure_sensor.h"

#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>

// Hjälpfunktion: tolka datumsträng "YYYY-MM-DD" till time_point
std::optional<std::chrono::system_clock::time_point>
parseDate(const std::string& dateStr) {
    if (dateStr.size() != 10) {
        return std::nullopt; // fel format
    }

    std::tm tm{};
    std::istringstream iss(dateStr + " 00:00:00");
    iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (iss.fail()) {
        return std::nullopt;
    }

    std::time_t tt = std::mktime(&tm);
    if (tt == -1) {
        return std::nullopt;
    }

    return std::chrono::system_clock::from_time_t(tt);
}

int main() {
    SystemController controller;

    controller.addSensor(std::make_unique<TemperatureSensor>("Temp1", 15.0, 30.0));
    controller.addSensor(std::make_unique<HumiditySensor>("Hum1", 20.0, 80.0));
    controller.addSensor(std::make_unique<PressureSensor>("Press1", 950.0, 1050.0));

    char choice;
    do {
        std::cout << "\nMenu:\n"
            << "1. Sample all\n"
            << "2. Show stats for sensor\n"
            << "3. Show all measurements\n"
            << "4. Configure threshold\n"
            << "5. Show alerts\n"
            << "6. Save to file\n"
            << "7. Load from file\n"
            << "8. Show histogram\n"
            << "9. Search measurements\n"
            << "0. Exit\n"
            << "Choice: ";

        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == '1') {
            controller.sampleAllOnce();
        }
        else if (choice == '2') {
            std::string name;
            std::cout << "Sensor name: ";
            std::cin >> name;
            controller.showStatsFor(name);
        }
        else if (choice == '3') {
            controller.showAllMeasurements();
        }
        else if (choice == '4') {
            controller.configureThreshold();
        }
        else if (choice == '5') {
            controller.showAlerts();
        }
        else if (choice == '6') {
            std::string path;
            std::cout << "File path: ";
            std::cin >> path;
            controller.saveToFile(path);
        }
        else if (choice == '7') {
            std::string path;
            std::cout << "File path: ";
            std::cin >> path;
            controller.loadFromFile(path);
        }
        else if (choice == '8') {
            std::string name;
            std::cout << "Sensor name: ";
            std::cin >> name;
            controller.showHistogramForSensor(name);
        }
        else if (choice == '9') {
            std::string sensorFilter;
            std::string startDate;
            std::string endDate;

            std::cout << "Sensor name (empty for all): ";
            std::getline(std::cin, sensorFilter);

            std::cout << "Start date (YYYY-MM-DD, empty for none): ";
            std::getline(std::cin, startDate);

            std::cout << "End date (YYYY-MM-DD, empty for none): ";
            std::getline(std::cin, endDate);

            std::optional<std::chrono::system_clock::time_point> startTime;
            std::optional<std::chrono::system_clock::time_point> endTime;

            if (!startDate.empty()) {
                startTime = parseDate(startDate);
            }
            if (!endDate.empty()) {
                endTime = parseDate(endDate);
            }

            controller.searchMeasurements(sensorFilter, startTime, endTime);
        }
        else if (choice == '0') {
            std::cout << "Exiting...\n";
        }
        else {
            std::cout << "Invalid choice.\n";
        }

    } while (choice != '0');

    return 0;
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file