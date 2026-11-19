#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "Sample.h"

namespace sdp {

class Group;

class Sensor {
public:
    Sensor(const std::string& id, const std::string& type) : id_(id), type_(type) {}

    void setGroup(const std::shared_ptr<Group>& g) { group_ = g; }  // luu weak_ptr
    void addSample(const Sample& s);  // thread-safe
    void finalize();                  // goi sau khi cac worker da join

    const std::string& id() const { return id_; }
    const std::string& type() const { return type_; }
    std::string groupName() const;

    // Chi hop le sau finalize()
    int count() const { return count_; }
    double min() const { return min_; }
    double max() const { return max_; }
    double average() const { return avg_; }
    double movingAverage() const { return movingAvg_; }
    int alarms() const { return alarms_; }

private:
    std::string id_;
    std::string type_;
    std::weak_ptr<Group> group_;  // tro nguoc ve Group, khong so huu

    std::mutex mutex_;
    std::vector<Sample> samples_;

    int count_ = 0;
    double min_ = 0, max_ = 0, avg_ = 0, movingAvg_ = 0;
    int alarms_ = 0;
};

// Moi loai sensor 1 group, giu sensor bang shared_ptr
class Group {
public:
    explicit Group(const std::string& name) : name_(name) {}
    void addSensor(const std::shared_ptr<Sensor>& s);
    const std::string& name() const { return name_; }
    std::size_t sensorCount();

private:
    std::string name_;
    std::mutex mutex_;
    std::vector<std::shared_ptr<Sensor>> sensors_;
};

class SensorRegistry {
public:
    void addSample(const Sample& s);  // thread-safe
    void finalizeAll();
    std::shared_ptr<Sensor> find(const std::string& id);
    // map sap xep theo id
    const std::map<std::string, std::shared_ptr<Sensor>>& sensors() const { return sensors_; }

private:
    std::mutex mutex_;  // bao ve sensors_ va groups_
    std::map<std::string, std::shared_ptr<Sensor>> sensors_;
    std::map<std::string, std::shared_ptr<Group>> groups_;
};

}  // namespace sdp
