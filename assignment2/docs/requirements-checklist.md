# Checklist Assignment 2

Device, Light, Thermostat và SecurityCam đã triển khai và có test riêng. `make check` chỉ kiểm tra cú pháp/header; `make test` báo PASS cho bốn class này và SKIPPED cho TC-01..TC-10.

## Functional requirements

- [x] FR-01 Device abstract, thông tin ID/tên/trạng thái và virtual destructor vận hành đúng.
- [x] FR-02 Light validation, setting, status, powerUsage và clone.
- [x] FR-03 Thermostat validation, setting, status, powerUsage và clone.
- [x] FR-04 SecurityCam validation, setting, status, powerUsage và clone.
- [ ] FR-05 Room sở hữu thiết bị, thêm/xóa/tìm và tăng mảng động.
- [ ] FR-06 Rule of Three và deep copy cho Device/Room; copy assignment giữa hai phòng khác nhau.
- [ ] FR-07 Room totalPower, listDevices và operator+=.
- [ ] FR-08 ConsoleApp quản lý phòng và menu nhập liệu.
- [ ] FR-09 Device::count, virtual destructor và TRACE_LIFECYCLE.
- [ ] FR-10 Demo đa hình, deep copy, self-assignment và quản lý bộ nhớ.

## Test cases

- [ ] TC-01 Tạo phòng với ba thiết bị.
- [ ] TC-02 Tổng công suất 813 W.
- [ ] TC-03 Camera ghi hình, tổng công suất 816 W.
- [ ] TC-04 Setting không hợp lệ.
- [ ] TC-05 Copy phòng bằng copy constructor là deep copy.
- [ ] TC-06 Xóa phòng copy không ảnh hưởng phòng gốc.
- [ ] TC-07 Self-assignment.
- [ ] TC-08 Xóa ID thiết bị không tồn tại.
- [ ] TC-09 Mảng thiết bị tăng từ capacity ban đầu 2.
- [ ] TC-10 Device::count kết thúc bằng 0.
- [ ] Bổ sung: copy assignment giữa hai phòng khác nhau.

## Deliverables

- [x] Header và skeleton source cho sáu class; Device, Light, Thermostat và SecurityCam đã có implementation và test riêng.
- [x] Makefile: app, demo, test, trace, check, clean.
- [x] Mermaid class diagram bản nguồn.
- [ ] Bản PNG/PDF của class diagram.
- [ ] Demo và test chức năng thật, có kết quả kiểm tra.
- [ ] Bằng chứng debugger và kiểm tra bộ nhớ thật.
- [ ] Minh họa “không thể tạo Device trực tiếp” cùng compiler error thật trong README khi triển khai.
