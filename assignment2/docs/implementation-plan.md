# Kế hoạch triển khai Assignment 2

Device, Light, Thermostat và SecurityCam đã được triển khai, mỗi class có test riêng. Room, ConsoleApp, demo và TC-01..TC-10 vẫn là skeleton.

1. **Device — đã làm:** cấp phát `mName` bằng `new[]`, Rule of Three, `setName`, bật/tắt, `mCount`, `operator<<`. Xong khi copy độc lập, count đúng qua scope và không rò bộ nhớ.
2. **Các class con — Light, Thermostat và SecurityCam đã làm:** constructor, setter/getter, các hàm ảo, `clone`, `status`, công suất và setting. Validation, công suất và đa hình đều có test riêng.
3. **Room:** sở hữu `Device**`, tăng capacity, clone khi thêm/copy, xóa đúng, tìm kiếm và tổng công suất. Xong khi copy một phòng rồi sửa/xóa phòng copy không ảnh hưởng phòng gốc.
4. **Operators và lifecycle trace:** `operator+=`, copy assignment giữa hai phòng, self-assignment và `TRACE_LIFECYCLE` cho mọi constructor/destructor kể cả copy constructor. Xong khi trace và Device::count phù hợp.
5. **Demo và test:** hoàn thành các TODO trong `demo/` và `tests/`; kiểm tra TC-01 đến TC-10. Xong khi kết quả thực là PASS, không còn SKIPPED.
6. **ConsoleApp:** quản lý `Room**`, nhập liệu, menu và thoát an toàn. Xong khi tạo/xóa phòng và thiết bị qua giao diện.
7. **Kiểm tra cuối:** build `-Wall -Wextra` không warning, chạy kiểm tra bộ nhớ, ghi log/debug evidence và xuất class diagram PNG/PDF.

## Quy ước thiết kế đề xuất (không phải yêu cầu bắt buộc)

- ID thiết bị duy nhất **trong một phòng**; thêm trùng ID trả `false`. ID giữa hai phòng có thể trùng. Copy phòng giữ ID nhưng tạo object độc lập.
- Default constructor dùng ID 0 và giá trị mặc định hợp lệ. Thiết bị mặc định tắt; camera mặc định không ghi hình.
- Tên null/rỗng bị từ chối. Parameterized constructor nhận dữ liệu sai dự kiến ném `std::invalid_argument`; setter trả `false` và giữ giá trị cũ.
- `settingName(index)` trả `nullptr` nếu index sai; `applySetting(index, value)` trả `false` nếu index hoặc value sai.
- `clone()` tạo object mới và bên nhận sở hữu. `Room::operator+=` gọi `addDevice`; trùng ID dự kiến ném `std::invalid_argument`. Lỗi cấp phát được phép lan truyền.
- Light: setting 0 = brightness 0–100. Thermostat: setting 0 = target temperature 16–30. SecurityCam: setting 0 = resolution 720/1080/2160; setting 1 = recording 0/1.
- Khi tắt, công suất 0 W. Khi bật: Light = `10.0 * brightness / 100`; Thermostat = 1500 W nếu `abs(targetTemp - 25) > 3`, ngược lại 800 W; camera = 5 W và thêm 3 W nếu ghi hình.
- `Device::count()` đếm object Device đang sống, gồm bản clone: constructor/copy constructor tăng, destructor giảm, assignment không đổi count.
- `TRACE_LIFECYCLE`: mỗi constructor/destructor in một dòng khi macro bật; copy constructor cũng phải trace. Đã làm cho destructor Device và constructor/destructor Light/Thermostat/SecurityCam; constructor/copy constructor Device và Room còn TODO.
