#include "Room.h"
#include <cstdio>
#include <cstring>
#include <ostream>
#include <stdexcept>

// Room sở hữu: mName (new[]) và từng Device* (do clone() tạo) trong mDevices.

Room::Room() : mName(nullptr), mDevices(nullptr), mCount(0), mCapacity(2) {
    setName("Room");
    mDevices = new Device*[mCapacity];
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Room constructed: %s\n", mName);
#endif
}

Room::Room(const char* name, int initialCapacity)
    : mName(nullptr), mDevices(nullptr), mCount(0), mCapacity(initialCapacity) {
    if (initialCapacity < 1 || !setName(name)) {
        throw std::invalid_argument("Room name or capacity is invalid");
    }
    mDevices = new Device*[mCapacity];
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Room constructed: %s\n", mName);
#endif
}

Room::Room(const Room& other)
    : mName(nullptr), mDevices(nullptr), mCount(0), mCapacity(other.mCapacity) {
    setName(other.mName);
    mDevices = new Device*[mCapacity];
    // clone từng thiết bị để hai phòng không dùng chung object.
    for (int i = 0; i < other.mCount; i++) {
        mDevices[i] = other.mDevices[i]->clone();
        mCount++;
    }
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Room copy constructed: %s\n", mName);
#endif
}

Room& Room::operator=(const Room& other) {
    if (this == &other) {
        return *this;  // tự gán thì không làm gì
    }
    // xóa dữ liệu cũ trước
    clearDevices();
    setName(other.mName);
    // rồi copy dữ liệu của other
    mCapacity = other.mCapacity;
    mDevices = new Device*[mCapacity];
    for (int i = 0; i < other.mCount; i++) {
        mDevices[i] = other.mDevices[i]->clone();
        mCount++;
    }
    return *this;
}

Room::~Room() {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Room destroyed: %s\n", mName);
#endif
    clearDevices();
    delete[] mName;
}

void Room::clearDevices() {
    for (int i = 0; i < mCount; i++) {
        delete mDevices[i];  // virtual destructor nên xóa đúng class con
    }
    delete[] mDevices;
    mDevices = nullptr;
    mCount = 0;
    mCapacity = 0;
}

void Room::ensureCapacity(int requiredCapacity) {
    if (requiredCapacity <= mCapacity) {
        return;
    }
    int newCapacity = mCapacity * 2;
    if (newCapacity < requiredCapacity) {
        newCapacity = requiredCapacity;
    }
    Device** newDevices = new Device*[newCapacity];
    for (int i = 0; i < mCount; i++) {
        newDevices[i] = mDevices[i];
    }
    delete[] mDevices;
    mDevices = newDevices;
    mCapacity = newCapacity;
}

const char* Room::getName() const {
    return mName;
}

bool Room::setName(const char* name) {
    if (name == nullptr || name[0] == '\0') {
        return false;
    }
    char* newName = new char[std::strlen(name) + 1];
    std::strcpy(newName, name);
    delete[] mName;
    mName = newName;
    return true;
}

int Room::deviceCount() const {
    return mCount;
}

bool Room::addDevice(const Device& device) {
    if (find(device.getId()) != nullptr) {
        return false;  // trùng ID trong phòng
    }
    ensureCapacity(mCount + 1);
    mDevices[mCount] = device.clone();
    mCount++;
    return true;
}

bool Room::removeDevice(std::uint32_t id) {
    for (int i = 0; i < mCount; i++) {
        if (mDevices[i]->getId() == id) {
            delete mDevices[i];
            // dồn các thiết bị phía sau lên một ô
            for (int j = i; j < mCount - 1; j++) {
                mDevices[j] = mDevices[j + 1];
            }
            mCount--;
            return true;
        }
    }
    return false;
}

Device* Room::find(std::uint32_t id) {
    for (int i = 0; i < mCount; i++) {
        if (mDevices[i]->getId() == id) {
            return mDevices[i];
        }
    }
    return nullptr;
}

const Device* Room::find(std::uint32_t id) const {
    for (int i = 0; i < mCount; i++) {
        if (mDevices[i]->getId() == id) {
            return mDevices[i];
        }
    }
    return nullptr;
}

double Room::totalPower() const {
    double total = 0;
    for (int i = 0; i < mCount; i++) {
        total += mDevices[i]->powerUsage();
    }
    return total;
}

void Room::listDevices(std::ostream& out) const {
    out << "Room " << mName << " (" << mCount << " devices)\n";
    for (int i = 0; i < mCount; i++) {
        out << "  " << *mDevices[i] << "\n";
    }
}

Room& Room::operator+=(const Device& device) {
    if (!addDevice(device)) {
        throw std::invalid_argument("Device ID already exists in room");
    }
    return *this;
}
