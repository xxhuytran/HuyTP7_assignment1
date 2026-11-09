# Yêu cầu assignment

## Mô hình

`Student` có ID dương duy nhất, tên ASCII không rỗng dài tối đa 31 byte, ba điểm C++/MCU/English trong [0, 10], average là trung bình ba điểm. Rank A nếu average >= 8.5, B nếu >= 7, C nếu >= 5, còn lại F. Kiểm tra mọi input và lỗi file.

## Đọc, lưu và bộ nhớ

- Đọc CSV từ command-line argument; mặc định `data/students.csv` (file students.csv trong data). Bỏ dòng trống và comment `#`; báo dòng sai kèm số dòng. CSV rỗng hoặc chỉ comment là hợp lệ.
- Mảng dữ liệu gốc cấp phát động bằng `new[]`, luôn tăng theo ID. Capacity khởi đầu 4 và tăng gấp đôi 4 → 8 → 16 → 32 khi đầy. Thêm đúng vị trí, từ chối ID trùng.
- Tìm ID bằng binary search đệ quy. Xóa đầu, giữa, cuối và cập nhật điểm phải giữ bất biến dữ liệu.
- Sort bằng heap array chứa `const Student*`, chỉ hoán đổi con trỏ, không thay đổi thứ tự mảng gốc. Hai khóa: average giảm dần, tên tăng dần.
- Thống kê average cả lớp, cao nhất, thấp nhất, số lượng từng rank.
- CSV dùng `fprintf`; binary dùng `fwrite`/`fread`. Dữ liệu reload phải giống trước khi lưu. Kiểm tra lỗi đọc/ghi và định dạng.
- Mọi `new[]` có `delete[]` tương ứng; mọi file đã mở đều được đóng.

## Menu

1 List all; 2 Add; 3 Update score; 4 Delete; 5 Search by ID; 6 Statistics; 7 Sort; 8 Save CSV; 9 Save/Load binary backup; 0 Exit. Xử lý EOF và lựa chọn sai an toàn.

## Debug Report

Báo cáo 1–2 trang cần bằng chứng debugger thực tế: breakpoint, conditional breakpoint, Variables/Watch, Call Stack, nguyên nhân, sửa lỗi và giải thích vùng nhớ. Không tự tạo `buggy_average.cpp` rồi coi đó là file trainer cung cấp. Không tạo ảnh debug, kết quả test hoặc bằng chứng giả.
