#ifndef ASSIGNMENT2_ROOM_H
#define ASSIGNMENT2_ROOM_H

#include "Device.h"

// Room sở hữu mọi Device* trong mảng mDevices.
class Room {
private:
    char* mName;
    Device** mDevices;
    int mCount;
    int mCapacity;

    void ensureCapacity(int requiredCapacity);
    // Dọn các thiết bị và mảng; mName được xử lý riêng.
    void clearDevices();
    void swap(Room& other) noexcept;

public:
    Room();
    explicit Room(const char* name, int initialCapacity = 2);
    // TODO: Copy constructor phải clone từng thiết bị.
    Room(const Room& other);
    // TODO: Copy assignment dùng copy-and-swap.
    Room& operator=(const Room& other);
    // TODO: Destructor phải delete từng thiết bị, mảng và mName.
    ~Room();

    const char* getName() const;
    bool setName(const char* name);
    int deviceCount() const;

    // Lưu bản sao qua clone(); Room sở hữu bản sao đó.
    bool addDevice(const Device& device);
    bool removeDevice(std::uint32_t id);

    // Trả con trỏ mượn; người gọi không được delete.
    Device* find(std::uint32_t id);
    const Device* find(std::uint32_t id) const;

    double totalPower() const;
    void listDevices(std::ostream& out) const;
    Room& operator+=(const Device& device);
};

#endif
