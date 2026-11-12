# Checklist triển khai

- [x] Cấu trúc, build C++17, menu mẫu và smoke test cho helper đã làm.
- [x] CSV: argument/default, bỏ dòng trống/comment, báo số dòng lỗi, dữ liệu rỗng.
- [x] Tính average, rank và kiểm tra điểm trong hàm `calcAverage`.
- [x] Hàm `prepareStudent` kiểm tra ID, tên, điểm rồi điền average/rank.
- [x] `addStudent` gọi kiểm tra dữ liệu, dùng `new[]`/`delete[]`, gấp đôi capacity, chèn theo ID, từ chối ID trùng.
- [x] Gọi kiểm tra dữ liệu ở các luồng CSV/menu.
- [x] Tìm ID bằng binary search đệ quy.
- [x] Xóa đầu, giữa, cuối bằng `removeStudent`.
- [x] Cập nhật điểm và tính lại average/rank bằng `updateScore`.
- [x] In một sinh viên và danh sách bằng `printStudent`.
- [x] Thống kê lớp/rank.
- [x] Sort pointer view trên heap theo average giảm/tên tăng; giữ nguyên mảng gốc.
- [x] CSV `fprintf` và binary `fwrite`/`fread`; round-trip giống hệt.
- [x] Kiểm tra input, lỗi file, đóng file và giải phóng toàn bộ bộ nhớ.
- [ ] Debug Report 1–2 trang với bằng chứng debugger thật; chờ trainer cung cấp `buggy_average.cpp` nếu bài yêu cầu.

## Trạng thái test case

| ID | Kiểm tra |
|---|---|
| TC-01 | Load CSV hợp lệ và hiển thị (đã kiểm tra) |
| TC-02 | File không tồn tại (đã kiểm tra) |
| TC-03 | CSV có đúng 3 dòng sai (đã kiểm tra) |
| TC-04 | Thêm 20 sinh viên, capacity 4 → 8 → 16 → 32 (unit test; menu đã thử thêm 1) |
| TC-05 | ID trùng (unit test; menu có xử lý) |
| TC-06 | Điểm -1, 10.5 hoặc `abc` (đã kiểm tra trong menu) |
| TC-07 | Tìm ID có và không tồn tại (unit test; menu đã thử ID có) |
| TC-08 | Xóa đầu, giữa, cuối (unit test; menu đã thử xóa 1 ID) |
| TC-09 | Thống kê (đã kiểm tra thủ công) |
| TC-10 | Sort điểm giảm dần và tên tăng dần (đã có unit test và kiểm tra menu) |
| TC-11 | CSV round-trip (đã có unit test) |
| TC-12 | Binary round-trip (đã có unit test) |
| TC-13 | CSV rỗng hoặc chỉ comment (đã có unit test) |
| TC-14 | Thoát và giải phóng tài nguyên (đã kiểm tra với ASan) |
