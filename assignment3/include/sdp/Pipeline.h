#pragma once
#include <atomic>
#include <memory>
#include <string>

#include "Logger.h"
#include "Sensor.h"

namespace sdp {

// Producer (1 thread) -> ThreadSafeQueue -> N worker -> SensorRegistry
class Pipeline {
public:
    Pipeline(std::shared_ptr<Logger> logger, int workers);

    // Throw FileError neu thieu file. Exception trong thread duoc nem lai sau join
    void run(const std::string& path);

    const SensorRegistry& registry() const { return *registry_; }
    int totalLines() const { return totalLines_; }
    int invalidLines() const { return invalidLines_; }
    int validLines() const { return totalLines_ - invalidLines_; }

private:
    std::shared_ptr<Logger> logger_;
    int workers_;
    std::unique_ptr<SensorRegistry> registry_;
    std::atomic<int> totalLines_{0};
    std::atomic<int> invalidLines_{0};
};

}  // namespace sdp
