# Assignment 3 – Multithreaded Sensor Data Processor

Chương trình C++17 đa luồng đọc log cảm biến ECU (`timestamp_ms;sensor_id;type;value`), loại mẫu lỗi, thống kê theo sensor, báo cáo alarm.

## Build / chạy (MinGW-w64 hoặc g++/clang++)

```
make            # build/sensor_processor
make test       # build + chạy test_main (VS Code: Run Test Task -> "test")
make run
./build/sensor_processor data/large.txt -n 8 -o report.csv -f T1
```
Tham số: `[input] [-n workers] [-o report.csv] [-f sensorId]`. Mặc định `-n` = `hardware_concurrency()` (tối thiểu 1). Không có `-f` thì chương trình hỏi ID từ stdin. Thiếu file -> `File error: ...`, exit code 2.
Build với `-Wall -Wextra -pthread`, 0 warning. Debug: `gdb build/sensor_processor` (đã có `-g`). Sanitizer (Linux/WSL, tùy chọn): thêm `-fsanitize=address,undefined` hoặc `-fsanitize=thread` vào CXXFLAGS/LDFLAGS.

Dữ liệu: `data/sensor_log.txt` (nhỏ, có vài dòng lỗi), `errors.txt`, `alarms.txt`, `empty.txt`, `large.txt` (120000 dòng); sinh lại bằng `python3 tools/gen_data.py`.

## Kiến trúc

```
file --Producer(1 thread, parse)--> ThreadSafeQueue<Sample> --> N Worker (validate) --> SensorRegistry
                                                                 main: join -> finalize -> mergeSort -> report.csv
```
- `sdp::ds`: `LinkedQueue`, `RingBuffer`, `ThreadSafeQueue`; `sdp::algo`: `mergeSort`, `binarySearch`; `sdp::io`: `parseLine`, report.
- `Pipeline` tạo thread, lưu `exception_ptr` cho từng thread, `join` hết rồi mới `rethrow_exception` ở main.
- Producer luôn `close()` queue (kể cả khi lỗi/thiếu file) nên worker không bao giờ bị treo.
- Kết quả không phụ thuộc thứ tự xử lý: mỗi sensor sort mẫu theo (timestamp, số dòng) rồi mới tính; tổng được cộng theo thứ tự đã sort; sort kết quả dùng tie-break theo ID. Số liệu CSV in cố định 2 chữ số thập phân; thời gian chỉ in console.
- Dòng không hợp lệ = lỗi parse (producer) + giá trị ngoài khoảng (worker). Giả định: mỗi sensor ID chỉ có một type.
- `RingBuffer<std::string,N>` tạo được nhưng gọi `average()` sẽ lỗi biên dịch (`static_assert`).

## Độ phức tạp

| Thao tác | Big-O |
|---|---|
| LinkedQueue push / pop / front | O(1) |
| LinkedQueue destructor | O(n) (vòng lặp, tránh đệ quy) |
| RingBuffer push | O(1); average O(N) |
| ThreadSafeQueue push / pop | O(1) + chờ khóa |
| Registry addSample | O(log S) tìm map + O(1) thêm |
| Finalize 1 sensor (m mẫu) | O(m log m) |
| mergeSort S sensor | O(S log S), bộ nhớ O(S) |
| binarySearch | O(log S) |
| Toàn bộ | O(L) parse + O(L log m) finalize (L dòng, S sensor) |

## Chiến lược lock

| Dữ liệu chung | Bảo vệ |
|---|---|
| Queue | 1 mutex + condition_variable trong `ThreadSafeQueue` |
| `SensorRegistry` (map sensor, group) | 1 mutex, chỉ giữ khi tra/tạo sensor |
| `Sensor::samples_` | mutex riêng từng sensor (lock sau khi nhả mutex registry -> không lồng lock, không deadlock) |
| `Group::sensors_` | mutex riêng |
| `Logger` | mutex quanh ghi stream |
| Bộ đếm dòng/invalid | `std::atomic<int>` |
| Slot `exception_ptr` | mỗi thread ghi 1 phần tử riêng, main đọc sau join |

## Smart pointer

- `unique_ptr`: node của LinkedQueue (sở hữu duy nhất, nối chuỗi) và `SensorRegistry` trong Pipeline.
- `shared_ptr`: Sensor (được registry, group và danh sách sort cùng giữ), Logger (nhiều thread dùng chung).
- `weak_ptr`: Sensor -> Group, tránh vòng tham chiếu Group <-> Sensor.

## Test / bằng chứng

`make test` (61 kiểm tra) bao gồm TC-01…TC-07 (N=1/2/4/8 giống nhau, dòng lỗi + số dòng trong log, thiếu file, 3 alarm, file 120000 dòng N=8 chạy 5 lần, tìm sensor, file rỗng). Output lưu ở `evidence/test_output.txt`. Với `large.txt`, MD5 của report.csv giống nhau cho N=1,2,4,8.
