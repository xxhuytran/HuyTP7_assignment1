#include "ConsoleApp.h"
#include "Light.h"
#include "SecurityCam.h"
#include "Thermostat.h"
#include <iostream>
#include <stdexcept>
#include <string>

ConsoleApp::ConsoleApp() : mRooms(nullptr), mRoomCount(0), mRoomCapacity(2) {
    mRooms = new Room*[mRoomCapacity];
}

ConsoleApp::~ConsoleApp() {
    for (int i = 0; i < mRoomCount; i++) {
        delete mRooms[i];
    }
    delete[] mRooms;
}

// Mảng đầy thì tạo mảng mới to gấp đôi rồi chép con trỏ sang.
void ConsoleApp::ensureRoomCapacity(int requiredCapacity) {
    if (requiredCapacity <= mRoomCapacity) {
        return;
    }
    int newCapacity = mRoomCapacity * 2;
    Room** newRooms = new Room*[newCapacity];
    for (int i = 0; i < mRoomCount; i++) {
        newRooms[i] = mRooms[i];
    }
    delete[] mRooms;
    mRooms = newRooms;
    mRoomCapacity = newCapacity;
}

void ConsoleApp::showMenu() const {
    std::cout << "\n===== Smart Home =====\n";
    std::cout << "1. Tao phong\n";
    std::cout << "2. Them thiet bi\n";
    std::cout << "3. Xem thiet bi\n";
    std::cout << "4. Bat/tat thiet bi\n";
    std::cout << "5. Doi setting\n";
    std::cout << "6. Xem tong cong suat\n";
    std::cout << "7. Copy phong\n";
    std::cout << "8. Xoa thiet bi\n";
    std::cout << "9. Xoa phong\n";
    std::cout << "0. Thoat\n";
}

// Chọn phòng theo số thứ tự. Không chọn được thì trả nullptr.
Room* ConsoleApp::selectRoom() {
    if (mRoomCount == 0) {
        std::cout << "Chua co phong nao.\n";
        return nullptr;
    }
    for (int i = 0; i < mRoomCount; i++) {
        std::cout << i + 1 << ". " << mRooms[i]->getName() << "\n";
    }
    int choice;
    std::cout << "Chon phong: ";
    std::cin >> choice;
    if (choice < 1 || choice > mRoomCount) {
        std::cout << "Phong khong ton tai.\n";
        return nullptr;
    }
    return mRooms[choice - 1];
}

// Chọn thiết bị theo ID. Không thấy thì trả nullptr.
Device* ConsoleApp::selectDevice(Room& room) {
    int id;
    std::cout << "Nhap ID thiet bi: ";
    std::cin >> id;
    if (id < 0) {
        std::cout << "ID khong hop le.\n";
        return nullptr;
    }
    Device* device = room.find(id);
    if (device == nullptr) {
        std::cout << "Khong tim thay thiet bi.\n";
    }
    return device;
}

void ConsoleApp::createRoom() {
    std::string name;
    std::cout << "Ten phong: ";
    std::cin >> name;  // chi nhap 1 tu, khong co dau cach
    try {
        Room* room = new Room(name.c_str());
        ensureRoomCapacity(mRoomCount + 1);
        mRooms[mRoomCount] = room;
        mRoomCount++;
        std::cout << "Da tao phong.\n";
    } catch (const std::invalid_argument& error) {
        std::cout << "Loi: " << error.what() << "\n";
    }
}

void ConsoleApp::addDevice() {
    Room* room = selectRoom();
    if (room == nullptr) {
        return;
    }
    int type, id, value;
    std::string name;
    std::cout << "Loai (1=Light, 2=Thermostat, 3=SecurityCam): ";
    std::cin >> type;
    std::cout << "ID (so duong): ";
    std::cin >> id;
    std::cout << "Ten thiet bi: ";
    std::cin >> name;

    try {
        // Tạo thiết bị tạm, Room sẽ clone một bản riêng.
        if (type == 1) {
            std::cout << "Do sang (0-100): ";
            std::cin >> value;
            Light light(id, name.c_str(), value);
            *room += light;
        } else if (type == 2) {
            std::cout << "Nhiet do (16-30): ";
            std::cin >> value;
            Thermostat thermostat(id, name.c_str(), value);
            *room += thermostat;
        } else if (type == 3) {
            std::cout << "Do phan giai (720/1080/2160): ";
            std::cin >> value;
            SecurityCam cam(id, name.c_str(), value, false);
            *room += cam;
        } else {
            std::cout << "Loai khong hop le.\n";
            return;
        }
        std::cout << "Da them thiet bi.\n";
    } catch (const std::invalid_argument& error) {
        std::cout << "Loi: " << error.what() << "\n";
    }
}

void ConsoleApp::listDevices() const {
    if (mRoomCount == 0) {
        std::cout << "Chua co phong nao.\n";
    }
    for (int i = 0; i < mRoomCount; i++) {
        mRooms[i]->listDevices(std::cout);
    }
}

void ConsoleApp::toggleDevice() {
    Room* room = selectRoom();
    if (room == nullptr) {
        return;
    }
    Device* device = selectDevice(*room);
    if (device == nullptr) {
        return;
    }
    if (device->isOn()) {
        device->turnOff();
    } else {
        device->turnOn();
    }
    std::cout << *device << "\n";
}

void ConsoleApp::changeSetting() {
    Room* room = selectRoom();
    if (room == nullptr) {
        return;
    }
    Device* device = selectDevice(*room);
    if (device == nullptr) {
        return;
    }
    for (int i = 0; i < device->settingCount(); i++) {
        std::cout << i << ". " << device->settingName(i) << "\n";
    }
    int index, value;
    std::cout << "Chon setting: ";
    std::cin >> index;
    std::cout << "Gia tri moi: ";
    std::cin >> value;
    if (device->applySetting(index, value)) {
        std::cout << *device << "\n";
    } else {
        std::cout << "Setting hoac gia tri khong hop le.\n";
    }
}

void ConsoleApp::showTotalPower() const {
    if (mRoomCount == 0) {
        std::cout << "Chua co phong nao.\n";
    }
    for (int i = 0; i < mRoomCount; i++) {
        std::cout << mRooms[i]->getName() << ": " << mRooms[i]->totalPower() << " W\n";
    }
}

void ConsoleApp::copyRoom() {
    Room* room = selectRoom();
    if (room == nullptr) {
        return;
    }
    std::string name;
    std::cout << "Ten phong moi: ";
    std::cin >> name;
    Room* copy = new Room(*room);  // copy constructor clone từng thiết bị
    if (!copy->setName(name.c_str())) {
        std::cout << "Ten khong hop le.\n";
        delete copy;
        return;
    }
    ensureRoomCapacity(mRoomCount + 1);
    mRooms[mRoomCount] = copy;
    mRoomCount++;
    std::cout << "Da copy phong.\n";
}

void ConsoleApp::deleteDevice() {
    Room* room = selectRoom();
    if (room == nullptr) {
        return;
    }
    int id;
    std::cout << "Nhap ID thiet bi can xoa: ";
    std::cin >> id;
    if (id >= 0 && room->removeDevice(id)) {
        std::cout << "Da xoa thiet bi.\n";
    } else {
        std::cout << "Khong tim thay thiet bi.\n";
    }
}

void ConsoleApp::deleteRoom() {
    Room* room = selectRoom();
    if (room == nullptr) {
        return;
    }
    for (int i = 0; i < mRoomCount; i++) {
        if (mRooms[i] == room) {
            delete mRooms[i];
            // dồn các phòng phía sau lên một ô
            for (int j = i; j < mRoomCount - 1; j++) {
                mRooms[j] = mRooms[j + 1];
            }
            mRoomCount--;
            std::cout << "Da xoa phong.\n";
            return;
        }
    }
}

void ConsoleApp::run() {
    int choice = -1;
    while (choice != 0) {
        showMenu();
        std::cout << "Chon: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            // nhập chữ thay vì số, hoặc hết dữ liệu: thoát cho an toàn
            std::cout << "Nhap khong hop le, thoat chuong trinh.\n";
            return;
        }

        if (choice == 1) {
            createRoom();
        } else if (choice == 2) {
            addDevice();
        } else if (choice == 3) {
            listDevices();
        } else if (choice == 4) {
            toggleDevice();
        } else if (choice == 5) {
            changeSetting();
        } else if (choice == 6) {
            showTotalPower();
        } else if (choice == 7) {
            copyRoom();
        } else if (choice == 8) {
            deleteDevice();
        } else if (choice == 9) {
            deleteRoom();
        } else if (choice != 0) {
            std::cout << "Lua chon khong hop le.\n";
        }
    }
}
