#include "sdp/Report.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "sdp/Algo.h"
#include "sdp/Errors.h"

namespace sdp {
namespace io {

// Average giam dan, bang nhau thi id tang dan (de ket qua xac dinh)
static bool sensorBefore(const std::shared_ptr<Sensor>& a, const std::shared_ptr<Sensor>& b) {
    if (a->average() != b->average()) return a->average() > b->average();
    return a->id() < b->id();
}

std::vector<std::shared_ptr<Sensor>> sortByAverage(const SensorRegistry& reg, bool& sortedOk) {
    std::vector<std::shared_ptr<Sensor>> mine;
    for (const auto& entry : reg.sensors()) {
        mine.push_back(entry.second);
    }
    std::vector<std::shared_ptr<Sensor>> reference = mine;

    algo::mergeSort(mine, sensorBefore);
    std::sort(reference.begin(), reference.end(), sensorBefore);

    sortedOk = (mine == reference);
    return mine;
}

static std::string num(double v) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(2) << v;
    return os.str();
}

std::string buildCsv(const std::vector<std::shared_ptr<Sensor>>& sorted, const Pipeline& p) {
    std::ostringstream os;
    os << "id,type,group,count,min,max,avg,moving_avg,alarms\n";
    for (const auto& s : sorted) {
        os << s->id() << ',' << s->type() << ',' << s->groupName() << ',' << s->count() << ','
           << num(s->min()) << ',' << num(s->max()) << ',' << num(s->average()) << ','
           << num(s->movingAverage()) << ',' << s->alarms() << '\n';
    }
    os << "total_lines,valid_lines,invalid_lines\n";
    os << p.totalLines() << ',' << p.validLines() << ',' << p.invalidLines() << '\n';
    return os.str();
}

void writeCsv(const std::string& path, const std::string& csv) {
    std::ofstream out(path);
    if (!out) {
        throw FileError("cannot write report file: " + path);
    }
    out << csv;
}

void printReport(const std::vector<std::shared_ptr<Sensor>>& sorted, const Pipeline& p) {
    std::cout << "\n=== Sensor report (sorted by average, descending) ===\n";
    std::cout << std::left << std::setw(12) << "ID" << std::setw(8) << "Type" << std::setw(8)
              << "Count" << std::setw(10) << "Min" << std::setw(10) << "Max" << std::setw(10)
              << "Avg" << std::setw(12) << "MovingAvg" << "Alarms\n";
    for (const auto& s : sorted) {
        std::cout << std::left << std::setw(12) << s->id() << std::setw(8) << s->type()
                  << std::setw(8) << s->count() << std::setw(10) << num(s->min()) << std::setw(10)
                  << num(s->max()) << std::setw(10) << num(s->average()) << std::setw(12)
                  << num(s->movingAverage()) << s->alarms() << '\n';
    }
    std::cout << "\nTotal lines: " << p.totalLines() << ", valid: " << p.validLines()
              << ", invalid: " << p.invalidLines() << '\n';
}

std::shared_ptr<Sensor> findSensor(const SensorRegistry& reg, const std::string& id) {
    // map da sap xep theo id -> copy ra vector de binarySearch
    std::vector<std::shared_ptr<Sensor>> byId;
    for (const auto& entry : reg.sensors()) {
        byId.push_back(entry.second);
    }
    int idx = algo::binarySearch(byId, id,
                                 [](const std::shared_ptr<Sensor>& s) { return s->id(); });
    return idx < 0 ? nullptr : byId[idx];
}

}  // namespace io
}  // namespace sdp
