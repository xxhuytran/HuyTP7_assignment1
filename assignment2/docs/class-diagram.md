# Class diagram — Assignment 2

```mermaid
classDiagram
    class Device {
        <<abstract>>
        #uint32_t mId
        #char* mName
        #bool mIsOn
        -static int mCount
        +powerUsage() double
        +status(ostream&) void
        +clone() Device*
    }
    class Light { -int mBrightness }
    class Thermostat { -int mTargetTemp }
    class SecurityCam {
        -int mResolution
        -bool mIsRecording
    }
    class Room {
        -char* mName
        -Device** mDevices
        -int mCount
        -int mCapacity
    }
    class ConsoleApp {
        -Room** mRooms
        -int mRoomCount
        -int mRoomCapacity
    }
    Device <|-- Light
    Device <|-- Thermostat
    Device <|-- SecurityCam
    Room *-- Device : owns cloned devices
    ConsoleApp *-- Room : owns rooms
```

`Room` sở hữu bản clone của thiết bị; `ConsoleApp` sở hữu các phòng. Khi hoàn thiện assignment, xuất class diagram thành PNG/PDF để nộp cùng mã nguồn. Hiện đây chủ yếu là sơ đồ thiết kế; đã có implementation và test cho Device, Light, Thermostat và SecurityCam.
