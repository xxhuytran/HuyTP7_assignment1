#include "Device.h"
#include <cstdio>
#include <cstring>
#include <ostream>
#include <stdexcept>

// Class dùng riêng trong test để có thể tạo Device abstract.
class ProbeDevice : public Device {
public:
    ProbeDevice(std::uint32_t id, const char* name) : Device(id, name) {}
    ProbeDevice(const ProbeDevice& other) : Device(other) {}
    ProbeDevice& operator=(const ProbeDevice& other) {
        Device::operator=(other);
        return *this;
    }

    double powerUsage() const override { return isOn() ? 1.0 : 0.0; }
    void status(std::ostream& out) const override { out << getName(); }
    int settingCount() const override { return 0; }
    const char* settingName(int) const override { return nullptr; }
    bool applySetting(int, int) override { return false; }
    Device* clone() const override { return new ProbeDevice(*this); }
};

static int failures = 0;
#define CHECK(condition) do { \
    if (!(condition)) { std::printf("FAIL line %d: %s\n", __LINE__, #condition); failures++; } \
} while (0)

// Kiểm tra các hành vi đã triển khai của Device; trả 0 nếu đạt.
int main() {
    CHECK(Device::count() == 0);
    try {
        ProbeDevice invalid(5, "");
        CHECK(false);
    } catch (const std::invalid_argument&) {
        CHECK(Device::count() == 0);
    }
    try {
        ProbeDevice invalid(5, nullptr);
        CHECK(false);
    } catch (const std::invalid_argument&) {
        CHECK(Device::count() == 0);
    }

    {
        ProbeDevice original(1, "Lamp");
        CHECK(Device::count() == 1);
        CHECK(original.getId() == 1);
        CHECK(std::strcmp(original.getName(), "Lamp") == 0);
        CHECK(!original.isOn());
        original.turnOn();
        CHECK(original.isOn());
        CHECK(original.powerUsage() == 1.0);
        CHECK(original.setName(original.getName()));
        CHECK(!original.setName(nullptr));
        CHECK(!original.setName(""));
        CHECK(std::strcmp(original.getName(), "Lamp") == 0);

        ProbeDevice copy(original);
        CHECK(Device::count() == 2);
        CHECK(copy.getName() != original.getName());
        CHECK(copy.isOn());
        CHECK(copy.setName("Copy"));
        CHECK(std::strcmp(original.getName(), "Lamp") == 0);

        ProbeDevice assigned(3, "Other");
        CHECK(Device::count() == 3);
        assigned = original;
        CHECK(Device::count() == 3);
        CHECK(assigned.getId() == 1);
        CHECK(assigned.isOn());
        CHECK(assigned.getName() != original.getName());
        ProbeDevice& sameObject = assigned;
        assigned = sameObject;
        CHECK(std::strcmp(assigned.getName(), "Lamp") == 0);

        Device* cloned = original.clone();
        CHECK(Device::count() == 4);
        CHECK(cloned->getName() != original.getName());
        cloned->turnOff();
        CHECK(original.isOn() && !cloned->isOn());
        delete cloned; // Xóa qua Device*: cần virtual destructor.
        CHECK(Device::count() == 3);
        original.turnOff();
        CHECK(!original.isOn());
    }
    CHECK(Device::count() == 0);
    std::puts(failures == 0 ? "Device tests: PASS" : "Device tests: FAIL");
    return failures == 0 ? 0 : 1;
}
