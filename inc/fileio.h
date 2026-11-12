#ifndef FILEIO_H
#define FILEIO_H

#include "student.h"

// Đọc CSV path vào mảng mới, bỏ dòng sai và báo số dòng; trả số bản ghi hoặc -1 nếu lỗi file.
int loadCsv(const char* path, Student*& arr, int& count, int& capacity);
// Ghi count sinh viên vào CSV path bằng fprintf; trả true nếu thành công.
bool saveCsv(const char* path, const Student* arr, int count);
// Ghi count sinh viên vào binary path bằng fwrite; trả true nếu thành công.
bool saveBinary(const char* path, const Student* arr, int count);
// Đọc binary path bằng fread, thay mảng cũ khi hợp lệ; trả số bản ghi hoặc -1 nếu lỗi.
int loadBinary(const char* path, Student*& arr, int& count, int& capacity);

#endif
