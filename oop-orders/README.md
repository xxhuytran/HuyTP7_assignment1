# Học OOP C++: quản lý đơn hàng vận chuyển

Đây là ví dụ học riêng, không thuộc Assignment 1 hoặc Assignment 2. Mỗi lần chỉ sửa `main.cpp` để học một bước. Chưa cần tách thành nhiều file `.h` và `.cpp`.

## Chạy chương trình

Từ thư mục gốc repo:

```sh
make -C oop-orders run
```

Hai dòng đầu của kết quả hiện tại:

```text
An - 10 kg
Binh - 5.5 kg
```

Muốn xóa file chương trình đã biên dịch: `make -C oop-orders clean`.

## `public`, `protected`, `private` trong ví dụ hiện tại

| Mức truy cập | Bên trong `Order` | Class con như `ExpressOrder` | `main()` |
| --- | --- | --- | --- |
| `public` | Được | Được | Được |
| `protected` | Được | Được | Không |
| `private` | Được | Không | Không |

- `weightKg` và `customerName` là **private**: chỉ hàm của `Order` đọc/sửa trực tiếp. Class con dùng `getWeight()` và `getCustomerName()`.
- `getCustomerName()` là **protected**: `ExpressOrder::printDetails()` gọi được, nhưng `main()` không gọi được. Đây là ví dụ học cú pháp; khi không cần cho class con dùng, nên để hàm ở `private`.
- `getWeight()`, `printInfo()` và `calcFee()` của các class con là **public**: `main()` gọi được.
- `mPriorityFee` là **private của ExpressOrder**: `Order` và `main()` không truy cập trực tiếp được; `getPriorityFee()` cung cấp cách đọc.

Ở gần lời gọi `expressOrder.printDetails()` trong `main.cpp` có ba dòng bị comment. Bỏ dấu `//` **từng dòng một** rồi chạy `make -B -C oop-orders` để xem lỗi truy cập do compiler báo; sau đó thêm lại `//` để chương trình build được.

`class ExpressOrder : public Order, public Trackable` dùng **public inheritance**. Chữ `public` ở đây mô tả cách kế thừa; nó khác với nhãn `public:` bên trong class. Nhờ public inheritance, các hàm public của cha vẫn public ở class con. Thành viên private của cha vẫn private; chúng không tự trở thành public.

## Lộ trình học

1. **Class và object — đã làm:** `Order` là class; `firstOrder` là object.
2. **Đóng gói:** chuyển tên và khối lượng thành `private`; thêm constructor, `setWeight()` và getter. Thử nhập khối lượng âm để thấy quy tắc kiểm tra hoạt động.
3. **Kế thừa — đã làm:** `StandardOrder` và `ExpressOrder` có công thức riêng; `ExpressOrder` còn kế thừa `Trackable`.
4. **Đa hình:** thêm `virtual calcFee()` trong `Order`, `override` trong hai class con; gọi qua `Order&` trước khi học con trỏ và danh sách.
5. **Danh sách đơn hàng:** dùng `std::vector` và `std::unique_ptr` sau khi đã hiểu tham chiếu, con trỏ và destructor ảo.
6. **Composition:** thêm `Address` rồi đặt một `Address` bên trong mỗi `Order`.

Các bước này là **những phiên bản tiếp nối** của cùng ví dụ; chỉ triển khai bước kế tiếp sau khi bạn đã chạy và hiểu bước trước.

## Thử diamond problem

Ở cuối `main.cpp`, `InternationalOrder` kế thừa `TrackedOrder` và `InsuredOrder`. Vì cả hai nhánh đều kế thừa `Order` thông thường, một object có **hai phần Order**. Chương trình đổi khối lượng ở nhánh tracked thành 12 kg; nhánh insured vẫn là 10 kg.

`VirtualInternationalOrder` dùng hai nhánh kế thừa `virtual public Order`, nên hai nhánh cùng dùng **một phần Order**. Đổi khối lượng qua một nhánh thì nhánh kia cũng đọc được 12 kg. Constructor của class cuối gọi `Order(name, weight)` để khởi tạo phần chung này.

Chạy bản bình thường:

```sh
make -B -C oop-orders run
```

Xem lỗi `ambiguous` có chủ đích mà không sửa file:

```sh
clang++ -std=c++17 -Wall -Wextra -DSHOW_DIAMOND_ERROR -fsyntax-only oop-orders/main.cpp
```

Lệnh thứ hai **phải báo lỗi biên dịch** ở lời gọi `diamond.getWeight()`: compiler tìm thấy `Order::getWeight()` qua hai đường khác nhau. Bỏ `-DSHOW_DIAMOND_ERROR` để kiểm tra bản chạy được.
