# Student Score Manager — C++17 scaffold

Đây là bộ khung để tự làm assignment. Menu 0–9 chạy được, nhưng **chưa đọc CSV**, chưa quản lý sinh viên, thống kê, sort hoặc lưu file. Mọi hàm nghiệp vụ còn `TODO` và in `Not implemented` khi gọi; giá trị trả về chưa triển khai được ghi trong header. Không dùng kết quả menu làm bằng chứng đã xử lý dữ liệu.

## Chuẩn bị trên macOS

Cài Apple Command Line Tools bằng `xcode-select --install` nếu máy chưa có `clang++` và `make`. Trong VS Code, cài Microsoft C/C++ (`ms-vscode.cpptools`) và CodeLLDB (`vadimcn.vscode-lldb`). Mở thư mục repository, chọn Run and Debug để dùng cấu hình LLDB. `tasks.json` cung cấp build, debug-log build và test.

```sh
make all               # build/normal/student-score-manager
make run               # chạy với data/students.csv; chưa load dữ liệu
make debug-log         # build/debug-log/student-score-manager
make test              # smoke test phần đã làm
make asan              # build/asan/student-score-manager
./build/asan/student-score-manager data/students.csv
make clean
```

Mặc định compiler là `clang++`; có thể dùng `make CXX=g++`. Build dùng `-std=c++17 -Wall -Wextra -g -O0`. Mỗi cấu hình có object riêng để tránh trộn cờ biên dịch. `DEBUG_LOG` bật macro `TRACE` in tên hàm và số dòng; thêm lệnh TRACE khi triển khai nghiệp vụ. Test hiện chỉ kiểm tra inline helper, không chứng minh chức năng assignment.

## Thư mục

- `inc/`: cấu hình, mô hình dữ liệu và khai báo API.
- `src/`: stub nghiệp vụ, file I/O và menu.
- `tests/`: smoke test và nơi bổ sung test sau này.
- `data/`: CSV hợp lệ, CSV lỗi và file rỗng.
- `docs/`: yêu cầu, checklist, mẫu báo cáo debug và bằng chứng test.
- `.vscode/`: tác vụ và cấu hình LLDB dùng chung.

## Thứ tự tự triển khai

1. Validate dữ liệu, `calcAverage`, rank và format in.
2. Mảng động tăng capacity, chèn theo ID, binary search, xóa và cập nhật.
3. Đọc CSV và kiểm tra dữ liệu đầu vào; nối vào menu sau khi load thực sự hoạt động.
4. Thống kê và pointer view để sort.
5. Lưu CSV/binary, reload, rồi hoàn tất TC-01–TC-14 và Debug Report bằng kết quả thật.

## Bộ nhớ và công cụ

Biến cục bộ thông thường và lời gọi hàm gắn với stack; `new[]` cấp phát trên heap và cần `delete[]`. Biến static/global có thời gian sống suốt chương trình, thường nằm trong vùng dữ liệu; literal và một số dữ liệu bất biến có thể ở vùng chỉ đọc. Từ khóa `const` một mình không đủ để khẳng định địa chỉ vật lý hay segment của mọi biến. Quan sát debugger và trình biên dịch để kết luận trường hợp cụ thể.

Trên macOS dùng Apple Clang, LLDB, `make` và AddressSanitizer. Windows thường cần MSVC hoặc MinGW cùng debugger tương ứng; cấu hình `type: "lldb"` và đường dẫn ở đây dành cho macOS, không tự dùng nguyên trạng trên Windows.
