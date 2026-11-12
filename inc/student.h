#ifndef STUDENT_H
#define STUDENT_H

#include <cstdint>
#include "config.h"

struct Student {
    uint32_t id;
    char name[32];
    float scores[NUM_SUBJECTS];
    float average;
    char rank;
};

enum class SortKey { AverageDescending, NameAscending };

// Kiểm tra và chèn s theo ID tăng dần; cập nhật arr/count/capacity.
// Trả false nếu dữ liệu sai, ID trùng hoặc không cấp phát được bộ nhớ.
bool addStudent(Student*& arr, int& count, int& capacity, const Student& s);
// Xóa ID khỏi arr đã sắp xếp và giảm count; không đổi vùng nhớ đã cấp phát.
// Trả false nếu arr/count/id sai hoặc không tìm thấy ID.
bool removeStudent(Student* arr, int& count, uint32_t id);
// Sửa điểm môn subject (0..2) của ID, tính lại average/rank.
// Trả false nếu dữ liệu sai hoặc không tìm thấy ID; khi đó không sửa arr.
bool updateScore(Student* arr, int count, uint32_t id, int subject, float score);
// Tìm ID bằng đệ quy trong arr tăng dần, từ low đến high (bao gồm hai đầu).
// Trả vị trí nếu tìm thấy, -1 nếu không có hoặc tham số sai; high phải là chỉ số hợp lệ.
int findById(const Student* arr, int low, int high, uint32_t id);
// Tính trung bình n điểm; trả -1 nếu con trỏ, n hoặc điểm không hợp lệ.
float calcAverage(const float* scores, int n = NUM_SUBJECTS);
// Xếp loại theo điểm trung bình; trả '?' nếu average ngoài khoảng 0..10.
char calcRank(float average);
// Kiểm tra id, name, scores của s; nếu đúng thì điền average/rank và trả true.
// Trả false nếu dữ liệu sai; khi đó s không bị thay đổi.
bool prepareStudent(Student& s);
// In một sinh viên s thành một dòng; không trả kết quả.
void printStudent(const Student& s);
// In count sinh viên trong arr; báo mảng rỗng hoặc tham số sai; không trả kết quả.
void printStudent(const Student* arr, int count);
// Đổi chỗ hai sinh viên a và b; không trả kết quả.
void swapStudent(Student& a, Student& b);
// Sắp xếp count con trỏ trong view theo key; không đổi mảng gốc.
void sortView(const Student** view, int count, SortKey key);

#endif
