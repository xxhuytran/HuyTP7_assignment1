#pragma once
#include <iostream>
#include <mutex>
#include <string>

namespace sdp {

// Logger thread-safe, dung chung qua shared_ptr
class Logger {
public:
    explicit Logger(std::ostream& out) : out_(out) {}

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        out_ << "[log] " << message << '\n';
    }

private:
    std::ostream& out_;
    std::mutex mutex_;
};

}  // namespace sdp
