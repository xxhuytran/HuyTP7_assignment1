# Assignment 2 — Smart Home Device Controller

Assignment 2 đang được triển khai từng bước. `Device`, `Light`, `Thermostat` và `SecurityCam` đã có implementation và test riêng. `Room`, `ConsoleApp`, demo và menu vẫn là skeleton. Chương trình Assignment 2 hiện chỉ in `Skeleton only: implementation pending.`; `make test` chạy test bốn class đã làm rồi báo `SKIPPED` cho TC-01..TC-10 còn lại.

## Cấu trúc và build

- `include/`: sáu header class với include guard.
- `src/`: sáu file TODO và entry point chỉ báo trạng thái skeleton.
- `demo/`: tên các demo cần tự viết.
- `tests/`: tên các test cần tự viết và kiểm tra header bằng `static_assert`.
- `docs/`: sơ đồ class, kế hoạch và checklist FR/TC.
- `evidence/`: nơi thêm bằng chứng thật sau này.

Từ thư mục `assignment2/`:

```sh
make app
make demo
make test       # Device/Light/Thermostat/SecurityCam PASS; TC-01..TC-10 vẫn SKIPPED
make trace      # build app skeleton với -DTRACE_LIFECYCLE
make device-trace  # build test Device có log constructor/destructor
make device-asan   # chạy test Device với AddressSanitizer
make light-trace   # build test Light có lifecycle log
make light-asan    # chạy test Light với AddressSanitizer
make thermostat-trace # build test Thermostat có lifecycle log
make thermostat-asan  # chạy test Thermostat với AddressSanitizer
make security-cam-trace # build test SecurityCam có lifecycle log
make security-cam-asan  # chạy test SecurityCam với AddressSanitizer
make check      # cú pháp source + include từng header độc lập
make clean
```

Từ thư mục gốc repo có thể dùng `make assignment2-app`, `make assignment2-demo`, `make assignment2-test`, `make assignment2-trace`, `make check`. Compiler mặc định là `clang++`; `make CXX=g++` cũng được hỗ trợ nếu có g++. Trên macOS có thể dùng LLDB khi bắt đầu triển khai; skeleton chưa cần chạy debugger để chứng minh nghiệp vụ.

## Ý tưởng thiết kế

`Room` sở hữu các `Device*` do `clone()` tạo và phải xóa chúng khi xóa/hủy phòng. `ConsoleApp` sở hữu các `Room*`. Rule of Three yêu cầu copy constructor, copy assignment và destructor được viết nhất quán khi class sở hữu bộ nhớ. Virtual destructor cho phép xóa class con qua `Device*`. `clone()` tạo bản sao đúng class con; chỉ sao chép phần `Device` sẽ gây object slicing và mất dữ liệu riêng của Light/Thermostat/SecurityCam. Hiện `Device` đã có quản lý `mName`, Rule of Three, virtual destructor và count; `Light` đã có công suất, setting, status và clone. Quyền sở hữu thiết bị/phòng trong `Room` vẫn chưa được triển khai.

Xem `docs/implementation-plan.md` để học và triển khai lần lượt. Bản PNG/PDF class diagram và minh họa compiler error khi cố tạo `Device` trực tiếp cần được bổ sung sau bằng kết quả thật.

`Device` cho phép ID 0 để các default constructor của class con dùng sau này; constructor có tham số của class con sẽ kiểm tra ID theo quy ước khi được triển khai. `getName()` trả con trỏ chỉ đọc tới tên mà Device sở hữu; người gọi không được `delete[]` con trỏ đó.

## Đọc `Device.cpp` nếu mới học C++

Đọc theo thứ tự: constructor tạo tên và tăng `mCount`; `setName` cấp phát mảng mới, chép tên rồi xóa mảng cũ; copy constructor gọi `setName` để có tên độc lập; copy assignment cũng gọi `setName` nhưng không tạo thêm object; destructor `delete[] mName` và giảm `mCount`. `std::strlen(name) + 1` dành thêm một ô cho ký tự kết thúc `\0`.

`Device` là abstract class nên không thể viết `Device d(...)`. File `tests/device_test.cpp` tạo `ProbeDevice` chỉ để kiểm tra phần chung của `Device`; đó không phải thiết bị thật của assignment. Light, Thermostat và SecurityCam đã được triển khai.

## Đọc `Light.cpp`

`Light` kế thừa `Device`: constructor của Light gọi constructor Device trước. Light chỉ thêm `mBrightness` (0–100). Khi tắt, `powerUsage()` trả 0 W; khi bật, trả `10.0 * brightness / 100`. `clone()` dùng copy constructor để tạo một Light mới; bên gọi phải `delete` bản clone. `status()` là hàm được ghi đè (`override`), nên gọi qua `Device&` vẫn chạy đúng phần của Light.

## Đọc `Thermostat.cpp`

`Thermostat` kế thừa `Device` và thêm `mTargetTemp` (16–30 °C). Khi tắt, công suất bằng 0 W. Khi bật, nhiệt độ cách 25 °C hơn 3 độ dùng 1500 W; các mức còn lại dùng 800 W. `settingCount()` và `applySetting()` cho phép điều chỉnh nhiệt độ qua giao diện chung `Device`. `clone()` tạo một bản sao độc lập; bên gọi chịu trách nhiệm `delete` bản sao.

## Đọc `SecurityCam.cpp`

`SecurityCam` kế thừa `Device`, thêm độ phân giải `mResolution` (720/1080/2160) và cờ ghi hình `mIsRecording`. `applySetting(0, value)` đổi độ phân giải; `applySetting(1, value)` chỉ nhận 0 hoặc 1 để tắt/bật ghi hình. Khi camera tắt, công suất là 0 W; khi bật là 5 W, hoặc 8 W nếu đang ghi hình. `clone()` tạo bản sao độc lập và bên gọi phải `delete` bản sao.
