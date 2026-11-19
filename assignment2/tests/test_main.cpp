#include "Light.h"
#include "Thermostat.h"
#include "SecurityCam.h"
#include "Room.h"
#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(condition) do { \
    if (!(condition)) { std::printf("FAIL line %d: %s\n", __LINE__, #condition); failures++; } \
} while (0)

// Tạo phòng mẫu: Light 80% (8 W) + Thermostat 25 °C (800 W) + Camera (5 W), tất cả bật.
static void fillSampleRoom(Room& room) {
    Light light(1, "Den", 80);
    Thermostat thermostat(2, "May lanh", 25);
    SecurityCam cam(3, "Camera", 1080, false);
    light.turnOn();
    thermostat.turnOn();
    cam.turnOn();
    room.addDevice(light);
    room.addDevice(thermostat);
    room.addDevice(cam);
}

// TC-01
void testCreateRoomWithThreeDevices() {
    Room room("Phong khach");
    fillSampleRoom(room);
    CHECK(room.deviceCount() == 3);
    CHECK(room.find(1) != nullptr);
    CHECK(room.find(2) != nullptr);
    CHECK(room.find(3) != nullptr);
    CHECK(!room.addDevice(Light(1, "Trung ID", 10)));  // trùng ID bị từ chối
    CHECK(room.deviceCount() == 3);
}

// TC-02
void testTotalPower() {
    Room room("Phong khach");
    fillSampleRoom(room);
    CHECK(room.totalPower() == 813.0);
}

// TC-03
void testCameraRecordingPower() {
    Room room("Phong khach");
    fillSampleRoom(room);
    Device* cam = room.find(3);
    CHECK(cam != nullptr);
    CHECK(cam->applySetting(1, 1));  // bật ghi hình
    CHECK(room.totalPower() == 816.0);
}

// TC-04
void testInvalidSettings() {
    Room room("Phong khach");
    fillSampleRoom(room);
    Device* light = room.find(1);
    CHECK(!light->applySetting(0, 101));  // sáng quá mức
    CHECK(!light->applySetting(5, 50));   // index sai
    CHECK(room.find(2)->applySetting(0, 40) == false);  // nhiệt độ quá cao
    CHECK(room.find(3)->applySetting(0, 500) == false); // độ phân giải sai
    CHECK(room.totalPower() == 813.0);    // giá trị cũ không đổi
}

// TC-05
void testRoomDeepCopy() {
    Room original("Goc");
    fillSampleRoom(original);
    Room copy(original);
    CHECK(copy.deviceCount() == 3);
    CHECK(copy.find(1) != original.find(1));  // khác địa chỉ = object độc lập
    copy.find(1)->applySetting(0, 10);        // sửa bản copy
    CHECK(original.totalPower() == 813.0);    // bản gốc không đổi
    CHECK(std::strcmp(copy.getName(), "Goc") == 0);
}

// TC-06
void testDeleteCopiedRoom() {
    Room original("Goc");
    fillSampleRoom(original);
    {
        Room copy(original);
        CHECK(Device::count() == 6);
    }
    // copy đã bị hủy, phòng gốc vẫn dùng được
    CHECK(Device::count() == 3);
    CHECK(original.totalPower() == 813.0);
    CHECK(std::strcmp(original.find(2)->getName(), "May lanh") == 0);
}

// TC-07
void testSelfAssignment() {
    Room room("Phong");
    fillSampleRoom(room);
    Room& same = room;
    room = same;
    CHECK(room.deviceCount() == 3);
    CHECK(room.totalPower() == 813.0);
    CHECK(Device::count() == 3);
}

// TC-08
void testRemoveUnknownDevice() {
    Room room("Phong");
    fillSampleRoom(room);
    CHECK(!room.removeDevice(99));
    CHECK(room.deviceCount() == 3);
    CHECK(room.removeDevice(2));
    CHECK(room.deviceCount() == 2);
    CHECK(room.find(2) == nullptr);
    CHECK(room.find(3) != nullptr);
}

// TC-09
void testDeviceArrayGrowth() {
    Room room("Phong", 2);  // capacity ban đầu 2
    for (std::uint32_t id = 1; id <= 5; id++) {
        Light light(id, "Den", 50);
        CHECK(room.addDevice(light));
    }
    CHECK(room.deviceCount() == 5);
    CHECK(room.find(1) != nullptr);
    CHECK(room.find(5) != nullptr);
}

// bổ sung: copy assignment giữa hai phòng khác nhau
void testRoomCopyAssignment() {
    Room a("A");
    fillSampleRoom(a);
    Room b("B");
    Light light(7, "Den B", 10);
    b.addDevice(light);
    b = a;
    CHECK(b.deviceCount() == 3);
    CHECK(b.find(7) == nullptr);  // thiết bị cũ của B đã bị xóa
    CHECK(b.find(1) != a.find(1));
    CHECK(std::strcmp(b.getName(), "A") == 0);
    CHECK(Device::count() == 7);
}

// TC-10: gọi cuối cùng, mọi Room đã bị hủy
void testFinalDeviceCount() {
    CHECK(Device::count() == 0);
}

int main() {
    CHECK(Device::count() == 0);
    testCreateRoomWithThreeDevices();
    testTotalPower();
    testCameraRecordingPower();
    testInvalidSettings();
    testRoomDeepCopy();
    testDeleteCopiedRoom();
    testSelfAssignment();
    testRemoveUnknownDevice();
    testDeviceArrayGrowth();
    testRoomCopyAssignment();
    testFinalDeviceCount();

    if (failures == 0) {
        std::puts("PASS: Assignment 2 TC-01..TC-10");
    }
    return failures;
}
