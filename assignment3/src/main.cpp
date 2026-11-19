#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "sdp/Errors.h"
#include "sdp/Logger.h"
#include "sdp/Pipeline.h"
#include "sdp/Report.h"

static void printSensor(const std::shared_ptr<sdp::Sensor>& s) {
    std::cout << "Found " << s->id() << " (" << s->type() << ", group " << s->groupName()
              << "): count=" << s->count() << " avg=" << s->average()
              << " alarms=" << s->alarms() << '\n';
}

int main(int argc, char* argv[]) {
    // Usage: sensor_processor [input] [-n workers] [-o report.csv] [-f sensorId]
    std::string input = "data/sensor_log.txt";
    std::string output = "report.csv";
    std::string findId;
    int workers = static_cast<int>(std::thread::hardware_concurrency());

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-n" && i + 1 < argc) {
            workers = std::atoi(argv[++i]);
        } else if (a == "-o" && i + 1 < argc) {
            output = argv[++i];
        } else if (a == "-f" && i + 1 < argc) {
            findId = argv[++i];
        } else {
            input = a;
        }
    }
    if (workers < 1) workers = 1;

    try {
        auto logger = std::make_shared<sdp::Logger>(std::cerr);
        sdp::Pipeline pipeline(logger, workers);

        auto start = std::chrono::steady_clock::now();
        pipeline.run(input);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::steady_clock::now() - start)
                      .count();

        bool sortedOk = false;
        auto sorted = sdp::io::sortByAverage(pipeline.registry(), sortedOk);
        sdp::io::printReport(sorted, pipeline);
        sdp::io::writeCsv(output, sdp::io::buildCsv(sorted, pipeline));

        std::cout << "mergeSort matches std::sort: " << (sortedOk ? "yes" : "NO") << '\n';
        std::cout << "Workers: " << workers << ", processing time: " << ms << " ms\n";
        std::cout << "Report written to " << output << '\n';

        if (!findId.empty()) {
            auto s = sdp::io::findSensor(pipeline.registry(), findId);
            if (s) printSensor(s); else std::cout << "Sensor '" << findId << "' not found\n";
        } else {
            std::string id;
            while (true) {
                std::cout << "Enter sensor ID (empty to quit): ";
                if (!std::getline(std::cin, id) || id.empty()) break;
                auto s = sdp::io::findSensor(pipeline.registry(), id);
                if (s) printSensor(s); else std::cout << "Sensor '" << id << "' not found\n";
            }
        }
    } catch (const sdp::FileError& e) {
        std::cerr << "File error: " << e.what() << '\n';
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
