#include "assignment1_app.h"
#include "fileio.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Đọc một dòng từ bàn phím; trả false nếu EOF hoặc dòng quá dài.
static bool readLine(const char* prompt, char* text, int size) {
    std::printf("%s", prompt);
    std::fflush(stdout);
    if (std::fgets(text, size, stdin) == nullptr) return false;
    int length = static_cast<int>(std::strlen(text));
    if (length > 0 && text[length - 1] == '\n') {
        text[length - 1] = '\0';
        return true;
    }
    if (std::feof(stdin)) return true;
    int ch;
    while ((ch = std::getchar()) != '\n' && ch != EOF) {}
    std::puts("Input too long.");
    return false;
}

// Đọc số nguyên trong min..max từ text; trả true nếu hợp lệ.
static bool parseInt(const char* text, long min, long max, long& value) {
    if (*text == '\0') return false;
    char* end;
    errno = 0;
    long number = std::strtol(text, &end, 10);
    while (*end == ' ' || *end == '\t') end++;
    if (errno != 0 || end == text || *end != '\0' || number < min || number > max) return false;
    value = number;
    return true;
}

// Đọc một điểm 0..10 từ text; trả true nếu hợp lệ.
static bool parseScore(const char* text, float& score) {
    if (*text == '\0') return false;
    char* end;
    errno = 0;
    float number = std::strtof(text, &end);
    while (*end == ' ' || *end == '\t') end++;
    if (errno != 0 || end == text || *end != '\0' || !(number >= 0.0f && number <= 10.0f)) return false;
    score = number;
    return true;
}

// Hỏi một số nguyên; trả false khi nhập sai hoặc EOF.
static bool askInt(const char* prompt, long min, long max, long& value) {
    char text[128];
    if (!readLine(prompt, text, sizeof(text))) return false;
    if (parseInt(text, min, max, value)) return true;
    std::puts("Invalid number.");
    return false;
}

// Hỏi một điểm; trả false khi nhập sai hoặc EOF.
static bool askScore(const char* prompt, float& score) {
    char text[128];
    if (!readLine(prompt, text, sizeof(text))) return false;
    if (parseScore(text, score)) return true;
    std::puts("Invalid score.");
    return false;
}

// Hỏi dữ liệu sinh viên; trả true nếu đủ và hợp lệ.
static bool askStudent(Student& student) {
    long id;
    if (!askInt("ID: ", 1, 4294967295L, id)) return false;
    student.id = static_cast<uint32_t>(id);
    char name[128];
    if (!readLine("Name: ", name, sizeof(name))) return false;
    int length = static_cast<int>(std::strlen(name));
    if (length == 0 || length >= 32 || std::strchr(name, ',') != nullptr) {
        std::puts("Invalid name (1-31 bytes, no comma).");
        return false;
    }
    std::strcpy(student.name, name);
    if (!askScore("C++ score: ", student.scores[0])) return false;
    if (!askScore("MCU score: ", student.scores[1])) return false;
    if (!askScore("English score: ", student.scores[2])) return false;
    return prepareStudent(student);
}

// In trung bình lớp, cao nhất, thấp nhất và số lượng từng rank.
static void printStatistics(const Student* arr, int count) {
    if (count == 0) {
        std::puts("No students.");
        return;
    }
    float total = 0.0f;
    int highest = 0;
    int lowest = 0;
    int ranks[4] = {0, 0, 0, 0};
    for (int i = 0; i < count; i++) {
        total += arr[i].average;
        if (arr[i].average > arr[highest].average) highest = i;
        if (arr[i].average < arr[lowest].average) lowest = i;
        if (arr[i].rank == 'A') ranks[0]++;
        else if (arr[i].rank == 'B') ranks[1]++;
        else if (arr[i].rank == 'C') ranks[2]++;
        else ranks[3]++;
    }
    std::printf("Class average: %.2f\n", total / count);
    std::printf("Highest: %s (ID %u, %.2f)\n", arr[highest].name,
                static_cast<unsigned int>(arr[highest].id), arr[highest].average);
    std::printf("Lowest: %s (ID %u, %.2f)\n", arr[lowest].name,
                static_cast<unsigned int>(arr[lowest].id), arr[lowest].average);
    std::printf("Ranks: A=%d B=%d C=%d F=%d\n", ranks[0], ranks[1], ranks[2], ranks[3]);
}

// In menu; không có tham số và không trả kết quả.
static void showMenu() {
    std::puts("\nStudent Score Manager");
    std::puts("1 List all\n2 Add\n3 Update score\n4 Delete\n5 Search by ID");
    std::puts("6 Statistics\n7 Sort\n8 Save CSV\n9 Save/Load binary backup\n0 Exit");
}

// Chạy menu với đường dẫn CSV tùy chọn; trả 0 khi thoát bình thường.
int runAssignment1(int argc, char* argv[]) {
    const char* csvPath = argc > 1 ? argv[1] : "data/students.csv";
    Student* students = nullptr;
    int count = 0;
    int capacity = 0;
    int loaded = loadCsv(csvPath, students, count, capacity);
    if (loaded < 0) std::printf("Could not load %s.", csvPath);
    else std::printf("Loaded %d students from %s.\n", loaded, csvPath);

    while (true) {
        showMenu();
        long choice;
        if (!askInt("Choice: ", 0, 9, choice)) {
            if (std::feof(stdin)) break;
            continue;
        }
        if (choice == 0) break;
        if (choice == 1) {
            printStudent(students, count);
        } else if (choice == 2) {
            Student student{};
            if (askStudent(student)) {
                std::puts(addStudent(students, count, capacity, student) ? "Student added." : "Could not add student (duplicate ID or no memory).");
            }
        } else if (choice == 3) {
            long id, subject;
            float score;
            if (askInt("ID: ", 1, 4294967295L, id) &&
                askInt("Subject (0=C++, 1=MCU, 2=English): ", 0, 2, subject) &&
                askScore("New score: ", score)) {
                std::puts(updateScore(students, count, static_cast<uint32_t>(id),
                                      static_cast<int>(subject), score) ? "Score updated." : "Student not found or invalid data.");
            }
        } else if (choice == 4) {
            long id;
            if (askInt("ID: ", 1, 4294967295L, id)) {
                std::puts(removeStudent(students, count, static_cast<uint32_t>(id)) ? "Student removed." : "Student not found.");
            }
        } else if (choice == 5) {
            long id;
            if (askInt("ID: ", 1, 4294967295L, id)) {
                int index = findById(students, 0, count - 1, static_cast<uint32_t>(id));
                if (index < 0) std::puts("Student not found.");
                else printStudent(students[index]);
            }
        } else if (choice == 6) {
            printStatistics(students, count);
        } else if (choice == 7) {
            if (count == 0) {
                std::puts("No students.");
                continue;
            }
            long key;
            if (!askInt("Sort (1=average descending, 2=name ascending): ", 1, 2, key)) continue;
            const Student** view = new (std::nothrow) const Student*[count];
            if (view == nullptr) {
                std::puts("Not enough memory.");
                continue;
            }
            for (int i = 0; i < count; i++) view[i] = &students[i];
            sortView(view, count, key == 1 ? SortKey::AverageDescending : SortKey::NameAscending);
            std::puts("ID    Name                               C++    MCU English Average Rank");
            for (int i = 0; i < count; i++) printStudent(*view[i]);
            delete[] view;
        } else if (choice == 8) {
            const char* path = "data/students_saved.csv";
            std::puts(saveCsv(path, students, count) ? "CSV saved to data/students_saved.csv." : "Could not save CSV.");
        } else if (choice == 9) {
            long action;
            if (!askInt("Binary (1=save, 2=load): ", 1, 2, action)) continue;
            const char* path = "data/students.backup.bin";
            if (action == 1) {
                std::puts(saveBinary(path, students, count) ? "Binary backup saved." : "Could not save binary backup.");
            } else {
                int result = loadBinary(path, students, count, capacity);
                if (result < 0) std::puts("Could not load binary backup.");
                else std::printf("Loaded %d students from binary backup.\n", result);
            }
        }
        if (std::feof(stdin)) break;
    }
    delete[] students;
    std::puts("DONE.");
    return 0;
}
