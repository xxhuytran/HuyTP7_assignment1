#include "Device.h"
#include <cstdio>
#include <cstring>
#include <ostream>
#include <stdexcept>

int Device::mCount = 0;

Device::Device(std::uint32_t id, const char* name)
    : mId(id), mName(nullptr), mIsOn(false) {
    if (!setName(name)) {
        throw std::invalid_argument("Device name must not be empty");
    }
    ++mCount;
}

Device::Device(const Device& other)
    : mId(other.mId), mName(nullptr), mIsOn(other.mIsOn) {
    // setName tạo mảng mới, không dùng chung mName với other.
    setName(other.mName);
    ++mCount;
}

Device& Device::operator=(const Device& other) {
    if (this != &other) {
        // Dùng lại setName để sao chép tên an toàn.
        setName(other.mName);
        mId = other.mId;
        mIsOn = other.mIsOn;
    }
    return *this;
}

Device::~Device() {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Device destroyed: id=%u, count=%d\n", mId, mCount - 1);
#endif
    delete[] mName;
    --mCount;
}

std::uint32_t Device::getId() const {
    return mId;
}

const char* Device::getName() const {
    return mName;
}

bool Device::isOn() const {
    return mIsOn;
}

bool Device::setName(const char* name) {
    if (name == nullptr || name[0] == '\0') {
        return false;
    }

    // +1 là chỗ cho ký tự kết thúc chuỗi '\0'.
    char* newName = new char[std::strlen(name) + 1];
    std::strcpy(newName, name);

    // Chỉ xóa tên cũ sau khi đã tạo xong tên mới.
    delete[] mName;
    mName = newName;
    return true;
}

void Device::turnOn() {
    mIsOn = true;
}

void Device::turnOff() {
    mIsOn = false;
}

int Device::count() {
    return mCount;
}

std::ostream& operator<<(std::ostream& out, const Device& device) {
    device.status(out);
    return out;
}
