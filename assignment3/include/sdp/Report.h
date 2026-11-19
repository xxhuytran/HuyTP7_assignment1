#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Pipeline.h"

namespace sdp {
namespace io {

// Sensor sap xep theo average giam dan bang algo::mergeSort.
// sortedOk = ket qua khop voi std::sort
std::vector<std::shared_ptr<Sensor>> sortByAverage(const SensorRegistry& reg, bool& sortedOk);

// Noi dung report.csv (khong co thoi gian xu ly)
std::string buildCsv(const std::vector<std::shared_ptr<Sensor>>& sorted, const Pipeline& p);

void writeCsv(const std::string& path, const std::string& csv);
void printReport(const std::vector<std::shared_ptr<Sensor>>& sorted, const Pipeline& p);

// Tra sensor theo ID bang algo::binarySearch (nullptr neu khong co)
std::shared_ptr<Sensor> findSensor(const SensorRegistry& reg, const std::string& id);

}  // namespace io
}  // namespace sdp
