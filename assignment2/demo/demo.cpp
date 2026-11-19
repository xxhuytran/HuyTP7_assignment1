#include "Light.h"
#include "Thermostat.h"
#include "SecurityCam.h"
#include "Room.h"
#include <iostream>

// Demo constructor, copy constructor, destructor (chạy make trace để thấy log).
void demoLifecycle() {
    std::cout << "--- Lifecycle ---\n";
    Light light(1, "Den", 50);
    Light copy(light);  // copy constructor
    std::cout << "Device::count() = " << Device::count() << "\n";
}   // ra khỏi scope: destructor chạy

// Gọi hàm ảo qua con trỏ Device*.
void demoPolymorphism() {
    std::cout << "--- Polymorphism ---\n";
    Device* devices[3];
    devices[0] = new Light(1, "Den", 80);
    devices[1] = new Thermostat(2, "May lanh", 25);
    devices[2] = new SecurityCam(3, "Camera", 1080, true);
    for (int i = 0; i < 3; i++) {
        devices[i]->turnOn();
        std::cout << *devices[i] << "\n";  // status() của class con
        delete devices[i];
    }
}

// Copy phòng: thiết bị trong bản copy độc lập với bản gốc.
void demoDeepCopy() {
    std::cout << "--- Deep copy ---\n";
    Room a("Phong A");
    a.addDevice(Light(1, "Den", 100));
    Room b(a);
    b.find(1)->applySetting(0, 20);
    
    a.find(1)->turnOn();
    b.find(1)->turnOn();
    std::cout << "A: " << a.find(1)->powerUsage() << " W, B: " << b.find(1)->powerUsage() << " W\n";
}

// room = room không được làm hỏng dữ liệu.
void demoSelfAssignment() {
    std::cout << "--- Self assignment ---\n";
    Room room("Phong");
    room.addDevice(Light(1, "Den", 50));
    Room& same = room;
    room = same;
    room.listDevices(std::cout);
}

// Chỉ giải thích bằng lời, không tạo lỗi thật.
void demoShallowCopyProblem() {
    std::cout << "--- Shallow copy ---\n";
    std::cout << "Neu chi copy con tro Device*, hai phong cung tro vao mot object.\n";
    std::cout << "Khi ca hai phong bi huy, object bi delete hai lan -> loi bo nho.\n";
    std::cout << "Vi vay Room dung clone() de moi phong co object rieng.\n";
}

// delete qua Device* vẫn gọi đúng destructor class con.
void demoVirtualDestructor() {
    std::cout << "--- Virtual destructor ---\n";
    Device* device = new Light(1, "Den", 50);
    delete device;  // ~Light() -> ~Device()
    std::cout << "Da delete qua Device*, count = " << Device::count() << "\n";
}

void demoDeviceCount() {
    std::cout << "--- Device::count ---\n";
    std::cout << "Truoc scope: " << Device::count() << "\n";
    {
        Room room("Phong");
        room.addDevice(Light(1, "Den", 50));
        room.addDevice(Light(2, "Den 2", 50));
        std::cout << "Trong scope: " << Device::count() << "\n";
    }
    std::cout << "Sau scope: " << Device::count() << "\n";
}

int main() {
    demoLifecycle();
    demoPolymorphism();
    demoDeepCopy();
    demoSelfAssignment();
    demoShallowCopyProblem();
    demoVirtualDestructor();
    demoDeviceCount();
    return 0;
}
