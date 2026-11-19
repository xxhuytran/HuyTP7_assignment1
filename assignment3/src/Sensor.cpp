#include "sdp/Sensor.h"

#include <algorithm>
#include <cmath>

#include "sdp/RingBuffer.h"

namespace sdp {

bool isInRange(const Sample& s) {
    if (s.type == "TEMP") return s.value >= -40 && s.value <= 150;
    if (s.type == "VOLT") return s.value >= 0 && s.value <= 18;
    if (s.type == "SPEED") return s.value >= 0 && s.value <= 300;
    return false;
}

void Sensor::addSample(const Sample& s) {
    std::lock_guard<std::mutex> lock(mutex_);
    samples_.push_back(s);
}

std::string Sensor::groupName() const {
    std::shared_ptr<Group> g = group_.lock();
    return g ? g->name() : "none";
}

static bool isAlarm(const std::string& type, double value, double prev, bool hasPrev) {
    if (type == "TEMP") return value > 105;
    if (type == "VOLT") return value < 9 || value > 16;
    if (type == "SPEED") return hasPrev && std::fabs(value - prev) > 50;
    return false;
}

void Sensor::finalize() {
    std::lock_guard<std::mutex> lock(mutex_);

    // Sort theo (timestamp, so dong) -> ket qua khong phu thuoc thu tu xu ly
    std::sort(samples_.begin(), samples_.end(), [](const Sample& a, const Sample& b) {
        if (a.timestamp != b.timestamp) return a.timestamp < b.timestamp;
        return a.lineNo < b.lineNo;
    });

    count_ = static_cast<int>(samples_.size());
    min_ = max_ = avg_ = movingAvg_ = 0;
    alarms_ = 0;
    if (samples_.empty()) return;

    ds::RingBuffer<double, 10> last10;
    double sum = 0;
    min_ = max_ = samples_[0].value;
    double prev = 0;
    bool hasPrev = false;

    for (const Sample& s : samples_) {
        sum += s.value;
        min_ = std::min(min_, s.value);
        max_ = std::max(max_, s.value);
        last10.push(s.value);
        if (isAlarm(type_, s.value, prev, hasPrev)) ++alarms_;
        prev = s.value;
        hasPrev = true;
    }
    avg_ = sum / count_;
    movingAvg_ = last10.average();
}

void Group::addSensor(const std::shared_ptr<Sensor>& s) {
    std::lock_guard<std::mutex> lock(mutex_);
    sensors_.push_back(s);
}

std::size_t Group::sensorCount() {
    std::lock_guard<std::mutex> lock(mutex_);
    return sensors_.size();
}

void SensorRegistry::addSample(const Sample& s) {
    std::shared_ptr<Sensor> sensor;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = sensors_.find(s.sensorId);
        if (it == sensors_.end()) {
            std::shared_ptr<Group>& group = groups_[s.type];
            if (!group) {
                group = std::make_shared<Group>(s.type);
            }
            sensor = std::make_shared<Sensor>(s.sensorId, s.type);
            sensor->setGroup(group);
            group->addSensor(sensor);
            sensors_[s.sensorId] = sensor;
        } else {
            sensor = it->second;
        }
    }
    sensor->addSample(s);  // lock rieng cua sensor, registry mutex da nha
}

void SensorRegistry::finalizeAll() {
    for (auto& entry : sensors_) {
        entry.second->finalize();
    }
}

std::shared_ptr<Sensor> SensorRegistry::find(const std::string& id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = sensors_.find(id);
    return it == sensors_.end() ? nullptr : it->second;
}

}  // namespace sdp
