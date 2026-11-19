# Checklist Assignment 2

Room, ConsoleApp, demo và test TC-01..TC-10 đã triển khai; `make test` báo PASS. Còn thiếu: PNG/PDF class diagram, bằng chứng debugger, minh họa lỗi biên dịch khi tạo Device trực tiếp.

## Functional requirements

- [x] FR-01 Device abstract, thông tin ID/tên/trạng thái và virtual destructor vận hành đúng.
- [x] FR-02 Light validation, setting, status, powerUsage và clone.
- [x] FR-03 Thermostat validation, setting, status, powerUsage và clone.
- [x] FR-04 SecurityCam validation, setting, status, powerUsage và clone.
- [x] FR-05 Room sở hữu thiết bị, thêm/xóa/tìm và tăng mảng động.
- [x] FR-06 Rule of Three và deep copy cho Device/Room; copy assignment giữa hai phòng khác nhau.
- [x] FR-07 Room totalPower, listDevices và operator+=.
- [x] FR-08 ConsoleApp quản lý phòng và menu nhập liệu.
- [x] FR-09 Device::count, virtual destructor và TRACE_LIFECYCLE.
- [x] FR-10 Demo đa hình, deep copy, self-assignment và quản lý bộ nhớ.

## Test cases

- [x] TC-01 Tạo phòng với ba thiết bị.
- [x] TC-02 Tổng công suất 813 W.
- [x] TC-03 Camera ghi hình, tổng công suất 816 W.
- [x] TC-04 Setting không hợp lệ.
- [x] TC-05 Copy phòng bằng copy constructor là deep copy.
- [x] TC-06 Xóa phòng copy không ảnh hưởng phòng gốc.
- [x] TC-07 Self-assignment.
- [x] TC-08 Xóa ID thiết bị không tồn tại.
- [x] TC-09 Mảng thiết bị tăng từ capacity ban đầu 2.
- [x] TC-10 Device::count kết thúc bằng 0.
- [x] Bổ sung: copy assignment giữa hai phòng khác nhau.

## Deliverables

- [x] Header và skeleton source cho sáu class; Device, Light, Thermostat và SecurityCam đã có implementation và test riêng.
- [x] Makefile: app, demo, test, trace, check, clean.
- [x] Mermaid class diagram bản nguồn.
- [ ] Bản PNG/PDF của class diagram.
- [x] Demo và test chức năng thật, có kết quả kiểm tra.
- [ ] Bằng chứng debugger và kiểm tra bộ nhớ thật.
- [ ] Minh họa “không thể tạo Device trực tiếp” cùng compiler error thật trong README khi triển khai.
