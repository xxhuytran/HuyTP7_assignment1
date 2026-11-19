#pragma once
#include <array>
#include <cstddef>
#include <type_traits>

namespace sdp {
namespace ds {

// Buffer vong co dinh N phan tu, day thi ghi de phan tu cu nhat
template <typename T, std::size_t N>
class RingBuffer {
    static_assert(N > 0, "RingBuffer size must be > 0");

public:
    void push(const T& value) {
        data_[next_] = value;
        next_ = (next_ + 1) % N;
        if (count_ < N) {
            ++count_;
        }
    }

    std::size_t size() const { return count_; }
    bool empty() const { return count_ == 0; }
    bool full() const { return count_ == N; }

    // Trung binh cac phan tu dang co. Chi dung duoc voi kieu so (khong dung voi std::string)
    double average() const {
        static_assert(std::is_arithmetic<T>::value, "RingBuffer::average needs a numeric type");
        if (count_ == 0) {
            return 0.0;  // tranh chia cho 0
        }
        double sum = 0.0;
        for (std::size_t i = 0; i < count_; ++i) {
            sum += static_cast<double>(data_[i]);
        }
        return sum / static_cast<double>(count_);
    }

private:
    std::array<T, N> data_{};
    std::size_t next_ = 0;
    std::size_t count_ = 0;
};

}  // namespace ds
}  // namespace sdp
