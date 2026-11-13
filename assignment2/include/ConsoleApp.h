#ifndef ASSIGNMENT2_CONSOLE_APP_H
#define ASSIGNMENT2_CONSOLE_APP_H

#include "Room.h"

// ConsoleApp sở hữu các Room*; menu và quản lý phòng còn TODO.
class ConsoleApp {
private:
    Room** mRooms;
    int mRoomCount;
    int mRoomCapacity;

    ConsoleApp(const ConsoleApp&) = delete;
    ConsoleApp& operator=(const ConsoleApp&) = delete;

    void ensureRoomCapacity(int requiredCapacity);
    void showMenu() const;
    void createRoom();
    void addDevice();
    void listDevices() const;
    void toggleDevice();
    void changeSetting();
    void showTotalPower() const;
    void copyRoom();
    void deleteDevice();
    void deleteRoom();
    Room* selectRoom();
    Device* selectDevice(Room& room);

public:
    ConsoleApp();
    ~ConsoleApp();
    void run();
};

#endif
