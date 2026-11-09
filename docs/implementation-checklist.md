# Checklist triển khai

- [x] Cấu trúc, build C++17, menu mẫu và smoke test cho helper đã làm.
- [ ] CSV: argument/default, bỏ dòng trống/comment, báo số dòng lỗi, dữ liệu rỗng.
- [ ] Validate ID, tên, điểm; tính average và rank.
- [ ] `new[]`/`delete[]`, capacity gấp đôi, chèn theo ID, từ chối ID trùng.
- [ ] Binary search đệ quy; tìm, xóa và cập nhật điểm.
- [ ] In danh sách và thống kê lớp/rank.
- [ ] Sort pointer view trên heap theo average giảm/tên tăng; giữ nguyên mảng gốc.
- [ ] CSV `fprintf` và binary `fwrite`/`fread`; round-trip giống hệt.
- [ ] Kiểm tra input, lỗi file, đóng file và giải phóng toàn bộ bộ nhớ.
- [ ] Debug Report 1–2 trang với bằng chứng debugger thật; chờ trainer cung cấp `buggy_average.cpp` nếu bài yêu cầu.

## Test case pending

| ID | Kiểm tra |
|---|---|
| TC-01 | Load CSV hợp lệ và hiển thị |
| TC-02 | File không tồn tại |
| TC-03 | CSV có đúng 3 dòng sai |
| TC-04 | Thêm 20 sinh viên, capacity 4 → 8 → 16 → 32 |
| TC-05 | ID trùng |
| TC-06 | Điểm -1, 10.5 hoặc `abc` |
| TC-07 | Tìm ID có và không tồn tại |
| TC-08 | Xóa đầu, giữa, cuối |
| TC-09 | Thống kê |
| TC-10 | Sort điểm giảm dần và tên tăng dần |
| TC-11 | CSV round-trip |
| TC-12 | Binary round-trip |
| TC-13 | CSV rỗng hoặc chỉ comment |
| TC-14 | Thoát và giải phóng tài nguyên |
