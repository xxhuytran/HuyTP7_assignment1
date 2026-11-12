# Kết quả kiểm tra thực tế

Ngày 2026-11-11, trên macOS với Apple Clang:

- `make all`, `make test`, `make debug-log`, `make asan`: exit 0, không có compiler warning. `make test` in `Smoke tests: PASS`.
- Biên dịch `tests/test_main.cpp` cùng `src/student.cpp`, `src/fileio.cpp` với `-fsanitize=address` và chạy: `Smoke tests: PASS`, không có báo cáo ASan.
- Test C++ kiểm tra điểm/rank, validate sinh viên, thêm 20 bản ghi và capacity 4 → 8 → 16 → 32, ID trùng, tìm/xóa/sửa, sort hai khóa và mảng gốc không đổi.
- Test C++ đọc 10 bản ghi từ `students.csv`; `students_invalid.csv` báo đúng dòng 3, 4, 5 và đọc 2 bản ghi hợp lệ; CSV rỗng/comment đọc 0 bản ghi; file không tồn tại trả lỗi.
- Test C++ lưu/đọc lại CSV và binary, so ID/tên/ba điểm/average/rank; file binary hỏng bị từ chối và danh sách cũ giữ nguyên.
- Chạy menu thực tế: load 10 sinh viên, liệt kê, thống kê, sort, thêm, tìm, sửa, xóa, lưu CSV và save/load binary. Thống kê dữ liệu mẫu in trung bình lớp `7.15`, rank `A=2 B=4 C=3 F=1`.
- Chạy thêm menu với input điểm `abc`, `-1`, `10.5`: in `Invalid score.` và không thêm/sửa bản ghi. EOF thoát bình thường. Chạy app ASan với CSV rỗng: thoát bình thường, không có báo cáo ASan.
- `.vscode/tasks.json` và `.vscode/launch.json` được kiểm tra bằng `python3 -m json.tool`: JSON hợp lệ.

Debug Report vẫn cần bằng chứng debugger do người học tự thu thập. Không có ảnh debug được tạo sẵn.
