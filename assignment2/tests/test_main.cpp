#include <cstdio>

// TODO TC-01 Arrange/Act/Assert: tạo phòng với 3 thiết bị.
void testCreateRoomWithThreeDevices();
// TODO TC-02 Arrange/Act/Assert: tổng công suất 813 W.
void testTotalPower();
// TODO TC-03 Arrange/Act/Assert: camera ghi hình, tổng 816 W.
void testCameraRecordingPower();
// TODO TC-04 Arrange/Act/Assert: setting ngoài phạm vi.
void testInvalidSettings();
// TODO TC-05 Arrange/Act/Assert: copy constructor clone thiết bị.
void testRoomDeepCopy();
// TODO TC-06 Arrange/Act/Assert: xóa phòng copy không ảnh hưởng phòng gốc.
void testDeleteCopiedRoom();
// TODO TC-07 Arrange/Act/Assert: tự gán room = room.
void testSelfAssignment();
// TODO TC-08 Arrange/Act/Assert: xóa ID không tồn tại.
void testRemoveUnknownDevice();
// TODO TC-09 Arrange/Act/Assert: mảng thiết bị tăng từ capacity 2.
void testDeviceArrayGrowth();
// TODO TC-10 Arrange/Act/Assert: Device::count() kết thúc bằng 0.
void testFinalDeviceCount();
// TODO bổ sung: copy assignment giữa hai phòng khác nhau.
void testRoomCopyAssignment();

// Entry point test; chưa chạy test chức năng, trả 0 vì chỉ báo SKIPPED.
int main() {
    std::puts("SKIPPED: Assignment 2 TC-01..TC-10 not implemented.");
    return 0;
}
