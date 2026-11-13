#ifndef ASSIGNMENT2_DEVICE_H
#define ASSIGNMENT2_DEVICE_H

#include <cstdint>
#include <iosfwd>

class Device {
protected:
    std::uint32_t mId;
    char* mName;  // Device sở hữu mảng ký tự này.
    bool mIsOn;

    // Chỉ class con được gọi copy; hai hàm deep-copy mName.
    Device(const Device& other);
    Device& operator=(const Device& other);

private:
    static int mCount;

public:
    Device(std::uint32_t id, const char* name);
    virtual ~Device();

    std::uint32_t getId() const;
    const char* getName() const;
    bool isOn() const;
    bool setName(const char* name);

    void turnOn();
    void turnOff();
    static int count();

    virtual double powerUsage() const = 0;
    virtual void status(std::ostream& out) const = 0;
    virtual int settingCount() const = 0;
    virtual const char* settingName(int index) const = 0;
    virtual bool applySetting(int index, int value) = 0;
    virtual Device* clone() const = 0;
};

// In trạng thái thiết bị qua hàm ảo status().
std::ostream& operator<<(std::ostream& out, const Device& device);

#endif
