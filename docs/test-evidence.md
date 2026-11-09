# Test evidence

Scaffold chỉ có smoke test cho `validSubject`. Ghi kết quả chạy thực tế và ảnh/debugger evidence tại đây sau khi triển khai nghiệp vụ. TC-01 đến TC-14 còn pending trong implementation checklist. Không dùng kết quả giả.

## Kiểm tra scaffold trên macOS (2026-11-09)

- `make all`, `make test`, `make debug-log`, `make asan`: exit 0; compiler không in warning. Smoke test in `Smoke tests: PASS`.
- `printf 'x\\n1\\n0\\n' | ./build/normal/student-score-manager data/students.csv`: in `Invalid choice.`, `Not implemented: menu option 1`, rồi `Goodbye.`.
- `printf '' | ./build/normal/student-score-manager data/students.csv`: in `EOF: exiting.`.
- `printf '0\\n' | ./build/asan/student-score-manager data/students.csv`: thoát bình thường, không có báo cáo ASan.
- `python3 -m json.tool` với ba file `.vscode/*.json`: hợp lệ.
