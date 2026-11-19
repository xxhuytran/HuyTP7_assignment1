#pragma once
#include <condition_variable>
#include <mutex>
#include <utility>

#include "LinkedQueue.h"

namespace sdp {
namespace ds {

// LinkedQueue + mutex + condition_variable. close() de worker dung em
template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }
        cv_.notify_one();
    }

    // Cho den khi co du lieu. Tra false neu queue da dong va het du lieu
    bool pop(T& out) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return closed_ || !queue_.empty(); });
        if (queue_.empty()) {
            return false;
        }
        out = queue_.pop();
        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        cv_.notify_all();
    }

private:
    LinkedQueue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool closed_ = false;
};

}  // namespace ds
}  // namespace sdp
