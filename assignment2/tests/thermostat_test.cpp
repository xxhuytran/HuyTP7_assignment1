#include "Thermostat.h"
#include <cstdio>
#include <cstring>
#include <ostream>
#include <stdexcept>
#include <streambuf>

class TextBuffer : public std::streambuf {
    char mText[256];
public:
    TextBuffer() { setp(mText, mText + 255); }
    const char* text() {
        *pptr() = '\0';
        return mText;
    }
};

static int failures = 0;
#define CHECK(condition) do { \
    if (!(condition)) { std::printf("FAIL line %d: %s\n", __LINE__, #condition); failures++; } \
} while (0)

int main() {
    CHECK(Device::count() == 0);
    {
        Thermostat thermostat;
        CHECK(thermostat.getId() == 0);
        CHECK(std::strcmp(thermostat.getName(), "Thermostat") == 0);
        CHECK(thermostat.getTargetTemp() == 25);
        CHECK(!thermostat.isOn() && thermostat.powerUsage() == 0.0);
    }
    CHECK(Device::count() == 0);

    for (int temperature : {15, 31}) {
        try {
            Thermostat invalid(1, "Bad", temperature);
            CHECK(false);
        } catch (const std::invalid_argument&) {
            CHECK(Device::count() == 0);
        }
    }
    try {
        Thermostat invalid(0, "Bad", 25);
        CHECK(false);
    } catch (const std::invalid_argument&) {
        CHECK(Device::count() == 0);
    }

    {
        Thermostat thermostat(7, "Living room", 22);
        CHECK(Device::count() == 1);
        CHECK(thermostat.settingCount() == 1);
        CHECK(std::strcmp(thermostat.settingName(0), "target temperature") == 0);
        CHECK(thermostat.settingName(1) == nullptr);
        CHECK(!thermostat.applySetting(1, 25));
        CHECK(!thermostat.applySetting(0, 15));
        CHECK(thermostat.getTargetTemp() == 22);

        thermostat.turnOn();
        Device& device = thermostat;
        CHECK(device.powerUsage() == 800.0);
        CHECK(device.applySetting(0, 21));
        CHECK(device.powerUsage() == 1500.0);
        CHECK(device.applySetting(0, 28));
        CHECK(device.powerUsage() == 800.0);
        CHECK(device.applySetting(0, 29));
        CHECK(device.powerUsage() == 1500.0);

        TextBuffer buffer;
        std::ostream out(&buffer);
        out << device;
        CHECK(std::strstr(buffer.text(), "target 29 C") != nullptr);
        CHECK(std::strstr(buffer.text(), "power 1500 W") != nullptr);

        Device* clone = device.clone();
        CHECK(Device::count() == 2);
        CHECK(clone->getName() != thermostat.getName());
        CHECK(clone->applySetting(0, 16));
        CHECK(thermostat.getTargetTemp() == 29);
        delete clone;
        CHECK(Device::count() == 1);

        Thermostat copy(thermostat);
        CHECK(copy.getName() != thermostat.getName());
        CHECK(copy.getTargetTemp() == 29 && copy.isOn());
        Thermostat assigned(8, "Bedroom", 25);
        assigned = thermostat;
        CHECK(assigned.getId() == 7);
        CHECK(assigned.getName() != thermostat.getName());
        CHECK(assigned.getTargetTemp() == 29);
        Thermostat& same = assigned;
        assigned = same;
        CHECK(assigned.getTargetTemp() == 29);
        thermostat.turnOff();
        CHECK(thermostat.powerUsage() == 0.0);
    }
    CHECK(Device::count() == 0);
    std::puts(failures == 0 ? "Thermostat tests: PASS" : "Thermostat tests: FAIL");
    return failures == 0 ? 0 : 1;
}
