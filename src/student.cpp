#include "student.h"
#include <cstdio>
#include <climits>
#include <cstring>
#include <new>

bool addStudent(Student*& arr, int& count, int& capacity, const Student& s) {
    // arr trỏ tới mảng sinh viên
    // count Số sinh viên hiện có
    // capacity Số chỗ mảng đã cấp phát
    // s Sinh viên cần add

    if (count < 0 || capacity < 0 || count > capacity) {
        return false;
    }
    if ((arr == nullptr && (count != 0 || capacity != 0)) ||
        (arr != nullptr && capacity == 0)) {
        return false;
    }

    Student newStudent = s;
    if (!prepareStudent(newStudent)) {
        return false;
    }

    // Tìm vị trí chèn và kiểm tra ID trùng.
    int position = 0;
    while (position < count && arr[position].id < newStudent.id) {
        position++;
    }
    if (position < count && arr[position].id == newStudent.id) {
        return false;
    }

    if (count == capacity) {
        if (capacity > INT_MAX / 2) {
            return false;
        }
        int newCapacity = capacity == 0 ? INITIAL_CAPACITY : capacity * 2;
        Student* larger = new (std::nothrow) Student[newCapacity];
        if (larger == nullptr) {
            return false;
        }
        for (int i = 0; i < count; i++) {
            larger[i] = arr[i];
        }
        delete[] arr;
        arr = larger;
        capacity = newCapacity;
    }

    // Dịch sang phải để chừa chỗ cho sinh viên mới.
    for (int i = count; i > position; i--) {
        arr[i] = arr[i - 1];
    }
    arr[position] = newStudent;
    count++;
    TRACE("Added ID %u, count=%d", static_cast<unsigned int>(newStudent.id), count);
    return true;
}

bool removeStudent(Student* arr, int& count, uint32_t id) {
    if (arr == nullptr || count <= 0 || id == 0) {
        return false;
    }

    int position = findById(arr, 0, count - 1, id);
    if (position == -1) {
        return false;
    }

    // Ghi đè phần tử cần xóa bằng các phần tử phía sau.
    for (int i = position; i < count - 1; i++) {
        arr[i] = arr[i + 1];
    }
    count--;
    return true;
}

bool updateScore(Student* arr, int count, uint32_t id, int subject, float score) {
    if (arr == nullptr || count <= 0 || id == 0 || !validSubject(subject)) {
        return false;
    }
    if (!(score >= 0.0f && score <= 10.0f)) {
        return false;
    }

    int position = findById(arr, 0, count - 1, id);
    if (position == -1) {
        return false;
    }

    // Kiểm tra trên bản sao trước, tránh sửa dở dữ liệu trong mảng.
    Student updated = arr[position];
    updated.scores[subject] = score;
    if (!prepareStudent(updated)) {
        return false;
    }
    arr[position] = updated;
    return true;
}

int findById(const Student* arr, int low, int high, uint32_t id) {
    //arr: mảng sinh viên
    // low, high: vị trí đầu và cuối của đoạn đang tìm, có tính cả hai đầu
    // id: sinh viên ID cần tìm

    if (arr == nullptr || low < 0 || id == 0 || low > high) {
        // return -1 nếu không tìm thấy
        return -1;
    }

    int middle = low + (high - low) / 2;
    if (arr[middle].id == id) {
        return middle;
    }
    if (id < arr[middle].id) {
        return findById(arr, low, middle - 1, id);
    }
    return findById(arr, middle + 1, high, id);
}

float calcAverage(const float* scores, int n) {
    if (scores == nullptr || n <= 0) {
        return -1.0f;
    }

    float total = 0.0f;
    for (int i = 0; i < n; i++) {
        if (!(scores[i] >= 0.0f && scores[i] <= 10.0f)) {
            return -1.0f;
        }
        total += scores[i];
    }
    return total / n;
}

char calcRank(float average) {
    if (!(average >= 0.0f && average <= 10.0f)) {
        return '?';
    }
    if (average >= 8.5f) return 'A';
    if (average >= 7.0f) return 'B';
    if (average >= 5.0f) return 'C';
    return 'F';
}

bool prepareStudent(Student& s) {
    if (s.id == 0 || s.name[0] == '\0') {
        return false;
    }

    // Trong 32 ô của name phải có ký tự kết thúc chuỗi '\0'.
    int length = 0;
    while (length < 32 && s.name[length] != '\0') {
        length++;
    }
    if (length == 32) {
        return false;
    }

    float average = calcAverage(s.scores);
    if (average < 0.0f) {
        return false;
    }

    s.average = average;
    s.rank = calcRank(average);
    return true;
}

void printStudent(const Student& s) {
    std::printf("%-5u %-31.31s %6.2f %6.2f %7.2f %7.2f %4c\n",
                static_cast<unsigned int>(s.id), s.name,
                s.scores[0], s.scores[1], s.scores[2],
                s.average, s.rank);
}

void printStudent(const Student* arr, int count) {
    if (count < 0 || (arr == nullptr && count > 0)) {
        std::puts("Invalid student list.");
        return;
    }
    if (count == 0) {
        std::puts("No students.");
        return;
    }

    std::puts("ID    Name                               C++    MCU English Average Rank");
    for (int i = 0; i < count; i++) {
        printStudent(arr[i]);
    }
}

void swapStudent(Student& a, Student& b) {
    Student temp = a;
    a = b;
    b = temp;
}

void sortView(const Student** view, int count, SortKey key) {
    if (view == nullptr || count <= 1) return;
    for (int i = 0; i < count; i++) {
        if (view[i] == nullptr) return;
    }

    // Selection sort: tìm phần tử đứng trước rồi đổi con trỏ.
    for (int i = 0; i < count - 1; i++) {
        int best = i;
        for (int j = i + 1; j < count; j++) {
            bool before;
            if (key == SortKey::AverageDescending) {
                before = view[j]->average > view[best]->average;
                if (view[j]->average == view[best]->average) {
                    before = view[j]->id < view[best]->id;
                }
            } else {
                int names = std::strcmp(view[j]->name, view[best]->name);
                before = names < 0 || (names == 0 && view[j]->id < view[best]->id);
            }
            if (before) best = j;
        }
        const Student* temp = view[i];
        view[i] = view[best];
        view[best] = temp;
    }
}
