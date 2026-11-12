# Student Score Manager

Chương trình C++17 quản lý điểm sinh viên bằng mảng động. Khi khởi động, chương trình đọc `data/students.csv` hoặc đường dẫn CSV truyền qua command line. Menu 0–9 cho phép xem, thêm, sửa, xóa, tìm, thống kê, sắp xếp và lưu dữ liệu.

## Build và chạy trên macOS

Nếu chưa có công cụ biên dịch, cài Apple Command Line Tools bằng `xcode-select --install`. Trong VS Code, cài Microsoft C/C++ và CodeLLDB, rồi mở cả thư mục project. Dùng task `build` hoặc Terminal:

```sh
make all
make run
make test
make debug-log
make asan
./build/asan/student-score-manager data/students.csv
make clean
```

`make run` dùng file mẫu `data/students.csv`. Để đọc file khác:

```sh
./build/normal/student-score-manager data/students_invalid.csv
```

Nếu `make all` báo `Nothing to be done for 'all'`, bản build đã cập nhật. `make clean` rồi `make all` sẽ biên dịch lại toàn bộ. Mặc định dùng `clang++`; có thể chạy `make CXX=g++`. Makefile tạo object riêng cho bản thường, `DEBUG_LOG`, test và AddressSanitizer, dùng `-std=c++17 -Wall -Wextra -g -O0`.

## Dữ liệu và menu

CSV có header `id,name,cpp,mcu,english`. Dòng trống và dòng bắt đầu bằng `#` được bỏ qua; dòng sai được báo kèm số dòng và các dòng hợp lệ vẫn được đọc. ID phải dương và duy nhất. Tên dài 1–31 byte, không có dấu phẩy. Điểm từng môn từ 0 đến 10. Average và rank được tính từ ba điểm.

Menu `8` ghi `data/students_saved.csv`; menu `9` ghi hoặc đọc `data/students.backup.bin`. Các file này được bỏ qua bởi Git. File binary có mã `SSM1`, số bản ghi, rồi ID, tên, ba điểm, average và rank của từng sinh viên. File binary dùng định dạng số của máy đang chạy, nên phù hợp để backup trên cùng môi trường; CSV phù hợp hơn để trao đổi với máy khác. Load chỉ thay danh sách hiện tại sau khi đọc xong và dữ liệu hợp lệ.

## Cấu trúc project

- `inc/`: `Student`, cấu hình và khai báo hàm.
- `src/student.cpp`: kiểm tra, quản lý mảng, tìm kiếm, in và sort.
- `src/fileio.cpp`: đọc/ghi CSV và binary bằng C-style file I/O.
- `src/main.cpp`: menu, nhập liệu và thống kê.
- `tests/`: các phép kiểm tra tự động; chạy bằng `make test`.
- `data/`: CSV mẫu hợp lệ, có lỗi và rỗng.
- `docs/`: yêu cầu, checklist, mẫu Debug Report và kết quả kiểm tra.
- `.vscode/`: task build và cấu hình LLDB.

## Bộ nhớ và debug

Biến cục bộ thông thường và lời gọi hàm gắn với stack; `new[]` cấp phát trên heap và cần `delete[]`. Biến static/global có thời gian sống suốt chương trình, thường nằm trong vùng dữ liệu; literal và một số dữ liệu bất biến có thể ở vùng chỉ đọc. Từ khóa `const` một mình không đủ để khẳng định địa chỉ vật lý hay segment của mọi biến. Quan sát debugger để kết luận trường hợp cụ thể.

Trên macOS dùng Apple Clang, LLDB, `make` và AddressSanitizer. Windows thường cần MSVC hoặc MinGW cùng debugger tương ứng; cấu hình LLDB ở đây dành cho macOS. Debug Report vẫn cần bạn tự thu thập breakpoint, Variables/Watch, Call Stack và bằng chứng thực tế theo mẫu trong `docs/`.
