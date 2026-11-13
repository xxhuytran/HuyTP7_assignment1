#include "Light.h"
#include <cstdio>
#include <cstring>
#include <ostream>
#include <stdexcept>
#include <streambuf>

// Giữ chữ mà status() in ra trong một char array để kiểm tra.
class TextBuffer : public std::streambuf {
    char text_[256];
public:
    TextBuffer() { setp(text_, text_ + 255); }
    const char* text() {
        *pptr() = '\0';
        return text_;
    }
};

static int failures = 0;
#define CHECK(condition) do { \
    if (!(condition)) { std::printf("FAIL line %d: %s\n", __LINE__, #condition); failures++; } \
} while (0)

// Kiểm tra hành vi Light qua chính class và con trỏ Device; trả 0 nếu đạt.
int main() {
    CHECK(Device::count() == 0);
    {
        Light light;
        CHECK(Device::count() == 1);
        CHECK(light.getId() == 0);
        CHECK(std::strcmp(light.getName(), "Light") == 0);
        CHECK(light.getBrightness() == 100);
        CHECK(!light.isOn() && light.powerUsage() == 0.0);
    }
    CHECK(Device::count() == 0);

    try {
        Light invalid(0, "Bad", 50);
        CHECK(false);
    } catch (const std::invalid_argument&) {
        CHECK(Device::count() == 0);
    }
    try {
        Light invalid(1, "Bad", 101);
        CHECK(false);
    } catch (const std::invalid_argument&) {
        CHECK(Device::count() == 0);
    }

    {
        Light light(7, "Desk", 80);
        CHECK(Device::count() == 1);
        CHECK(light.settingCount() == 1);
        CHECK(std::strcmp(light.settingName(0), "brightness") == 0);
        CHECK(light.settingName(1) == nullptr);
        CHECK(!light.applySetting(1, 30));
        CHECK(!light.applySetting(0, -1));
        CHECK(!light.applySetting(0, 101));
        CHECK(light.getBrightness() == 80);

        light.turnOn();
        Device& asDevice = light;
        CHECK(asDevice.powerUsage() == 8.0);
        TextBuffer buffer;
        std::ostream out(&buffer);
        out << asDevice; // operator<< của Device gọi Light::status().
        CHECK(std::strstr(buffer.text(), "brightness 80%") != nullptr);
        CHECK(std::strstr(buffer.text(), "power 8 W") != nullptr);

        Device* copied = light.clone();
        CHECK(Device::count() == 2);
        CHECK(copied->getName() != light.getName());
        CHECK(copied->applySetting(0, 20));
        CHECK(copied->powerUsage() == 2.0);
        CHECK(light.getBrightness() == 80);
        delete copied;
        CHECK(Device::count() == 1);

        Light copy(light);
        CHECK(Device::count() == 2);
        CHECK(copy.getName() != light.getName());
        CHECK(copy.getBrightness() == 80 && copy.isOn());
        CHECK(copy.setBrightness(100));
        CHECK(copy.powerUsage() == 10.0);
        CHECK(light.getBrightness() == 80);

        Light assigned(8, "Hall", 10);
        CHECK(Device::count() == 3);
        assigned = light;
        CHECK(Device::count() == 3);
        CHECK(assigned.getId() == 7);
        CHECK(assigned.getBrightness() == 80);
        CHECK(assigned.getName() != light.getName());
        Light& sameObject = assigned;
        assigned = sameObject;
        CHECK(assigned.getBrightness() == 80);
        light.turnOff();
        CHECK(light.powerUsage() == 0.0);
    }
    CHECK(Device::count() == 0);
    std::puts(failures == 0 ? "Light tests: PASS" : "Light tests: FAIL");
    return failures == 0 ? 0 : 1;
}
