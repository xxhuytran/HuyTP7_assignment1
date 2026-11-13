#include "SecurityCam.h"
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
        SecurityCam camera;
        CHECK(camera.getId() == 0);
        CHECK(std::strcmp(camera.getName(), "SecurityCam") == 0);
        CHECK(camera.getResolution() == 1080);
        CHECK(!camera.isRecording());
        CHECK(!camera.isOn() && camera.powerUsage() == 0.0);
    }
    CHECK(Device::count() == 0);

    for (int resolution : {0, 1081, 4320}) {
        try {
            SecurityCam invalid(1, "Bad", resolution, false);
            CHECK(false);
        } catch (const std::invalid_argument&) {
            CHECK(Device::count() == 0);
        }
    }
    try {
        SecurityCam invalid(0, "Bad", 720, false);
        CHECK(false);
    } catch (const std::invalid_argument&) {
        CHECK(Device::count() == 0);
    }

    {
        SecurityCam camera(7, "Door", 720, false);
        CHECK(camera.settingCount() == 2);
        CHECK(std::strcmp(camera.settingName(0), "resolution") == 0);
        CHECK(std::strcmp(camera.settingName(1), "recording") == 0);
        CHECK(camera.settingName(2) == nullptr);
        CHECK(!camera.applySetting(2, 1));
        CHECK(!camera.applySetting(0, 800));
        CHECK(!camera.applySetting(1, 2));
        CHECK(camera.getResolution() == 720 && !camera.isRecording());

        camera.turnOn();
        Device& device = camera;
        CHECK(device.powerUsage() == 5.0);
        CHECK(device.applySetting(0, 2160));
        CHECK(device.applySetting(1, 1));
        CHECK(camera.getResolution() == 2160 && camera.isRecording());
        CHECK(device.powerUsage() == 8.0);

        TextBuffer buffer;
        std::ostream out(&buffer);
        out << device;
        CHECK(std::strstr(buffer.text(), "resolution 2160p") != nullptr);
        CHECK(std::strstr(buffer.text(), "recording on") != nullptr);
        CHECK(std::strstr(buffer.text(), "power 8 W") != nullptr);

        Device* clone = device.clone();
        CHECK(Device::count() == 2);
        CHECK(clone->getName() != camera.getName());
        CHECK(clone->applySetting(1, 0));
        CHECK(clone->powerUsage() == 5.0);
        CHECK(camera.isRecording());
        delete clone;
        CHECK(Device::count() == 1);

        SecurityCam copy(camera);
        CHECK(copy.getName() != camera.getName());
        CHECK(copy.getResolution() == 2160 && copy.isRecording());
        SecurityCam assigned(8, "Garage", 1080, false);
        assigned = camera;
        CHECK(assigned.getId() == 7);
        CHECK(assigned.getName() != camera.getName());
        CHECK(assigned.getResolution() == 2160 && assigned.isRecording());
        SecurityCam& same = assigned;
        assigned = same;
        CHECK(assigned.isRecording());
        camera.turnOff();
        CHECK(camera.powerUsage() == 0.0);
    }
    CHECK(Device::count() == 0);
    std::puts(failures == 0 ? "SecurityCam tests: PASS" : "SecurityCam tests: FAIL");
    return failures == 0 ? 0 : 1;
}
