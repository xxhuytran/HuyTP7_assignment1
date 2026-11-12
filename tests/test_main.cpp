#include "fileio.h"
#include <cstdio>
#include <cstring>

int failures = 0;

#define CHECK(condition) do {                                         \
    if (!(condition)) {                                               \
        std::printf("FAIL at line %d: %s\n", __LINE__, #condition); \
        failures++;                                                    \
    }                                                                 \
} while (0)

// Kiểm tra helper đã làm; trả 0 nếu pass, 1 nếu có lỗi.
int main() {
    CHECK(validSubject(0));
    CHECK(validSubject(2));
    CHECK(!validSubject(-1));
    CHECK(!validSubject(3));

    float scores[NUM_SUBJECTS] = {8.0f, 9.0f, 8.5f};
    CHECK(calcAverage(scores) == 8.5f);
    CHECK(calcRank(calcAverage(scores)) == 'A');

    float otherScores[NUM_SUBJECTS] = {6.0f, 7.0f, 8.0f};
    CHECK(calcAverage(otherScores) == 7.0f);
    CHECK(calcRank(calcAverage(otherScores)) == 'B');

    CHECK(calcRank(10.0f) == 'A');
    CHECK(calcRank(8.5f) == 'A');
    CHECK(calcRank(8.49f) == 'B');
    CHECK(calcRank(7.0f) == 'B');
    CHECK(calcRank(6.99f) == 'C');
    CHECK(calcRank(5.0f) == 'C');
    CHECK(calcRank(4.99f) == 'F');
    CHECK(calcRank(0.0f) == 'F');

    float badScores[NUM_SUBJECTS] = {5.0f, -1.0f, 8.0f};
    CHECK(calcAverage(badScores) == -1.0f);
    badScores[1] = 10.5f;
    CHECK(calcAverage(badScores) == -1.0f);
    CHECK(calcAverage(nullptr) == -1.0f);
    CHECK(calcAverage(scores, 0) == -1.0f);
    CHECK(calcRank(-1.0f) == '?');
    CHECK(calcRank(10.5f) == '?');

    Student student = {1, "An", {8.0f, 9.0f, 8.5f}, 0.0f, '?'};
    CHECK(prepareStudent(student));
    CHECK(student.average == 8.5f);
    CHECK(student.rank == 'A');

    student.id = 0;
    CHECK(!prepareStudent(student));
    CHECK(student.average == 8.5f); // Dữ liệu sai không sửa average/rank.
    student.id = 1;

    student.name[0] = '\0';
    CHECK(!prepareStudent(student));
    student.name[0] = 'A';

    for (int i = 0; i < 32; i++) {
        student.name[i] = 'A';
    }
    CHECK(!prepareStudent(student)); // Tên không có ký tự kết thúc.
    student.name[31] = '\0';
    CHECK(prepareStudent(student)); // 31 byte nội dung là hợp lệ.

    student.scores[0] = -1.0f;
    CHECK(!prepareStudent(student));
    CHECK(student.average == 8.5f);
    student.scores[0] = 10.5f;
    CHECK(!prepareStudent(student));

    Student* list = nullptr;
    int count = 0;
    int capacity = 0;
    for (int id = 20; id >= 1; id--) {
        Student item = {static_cast<uint32_t>(id), "An", {8.0f, 9.0f, 8.5f}, 0.0f, '?'};
        CHECK(addStudent(list, count, capacity, item));
        if (id == 20) CHECK(capacity == 4);
        if (id == 16) CHECK(capacity == 8);
        if (id == 12) CHECK(capacity == 16);
        if (id == 4) CHECK(capacity == 32);
    }
    CHECK(count == 20);
    CHECK(capacity == 32);
    for (int i = 0; i < count; i++) {
        CHECK(list[i].id == static_cast<uint32_t>(i + 1));
        CHECK(list[i].average == 8.5f);
        CHECK(list[i].rank == 'A');
    }

    CHECK(findById(list, 0, count - 1, 1) == 0);
    CHECK(findById(list, 0, count - 1, 10) == 9);
    CHECK(findById(list, 0, count - 1, 20) == 19);
    CHECK(findById(list, 0, count - 1, 21) == -1);
    CHECK(findById(list, 5, 10, 6) == 5);
    CHECK(findById(list, 5, 10, 5) == -1);
    CHECK(findById(list, 0, -1, 1) == -1);
    CHECK(findById(nullptr, 0, 0, 1) == -1);
    CHECK(findById(list, -1, count - 1, 1) == -1);

    Student duplicate = {10, "Binh", {5.0f, 5.0f, 5.0f}, 0.0f, '?'};
    CHECK(!addStudent(list, count, capacity, duplicate));
    CHECK(count == 20);
    CHECK(list[9].id == 10);

    Student invalid = {0, "An", {8.0f, 8.0f, 8.0f}, 0.0f, '?'};
    CHECK(!addStudent(list, count, capacity, invalid));
    CHECK(count == 20);

    CHECK(removeStudent(list, count, 1));   // Xóa đầu.
    CHECK(count == 19);
    CHECK(list[0].id == 2);
    CHECK(removeStudent(list, count, 10));  // Xóa giữa.
    CHECK(count == 18);
    CHECK(list[8].id == 11);
    CHECK(removeStudent(list, count, 20));  // Xóa cuối.
    CHECK(count == 17);
    CHECK(list[count - 1].id == 19);
    CHECK(!removeStudent(list, count, 10)); // ID đã xóa.
    CHECK(!removeStudent(list, count, 0));
    CHECK(capacity == 32);
    for (int i = 0; i < count; i++) {
        uint32_t expected = static_cast<uint32_t>(i + 2);
        if (expected >= 10) expected++;
        CHECK(list[i].id == expected);
    }
    CHECK(!removeStudent(nullptr, count, 2));
    CHECK(count == 17);
    delete[] list;

    Student one[1] = {{7, "Ha", {7.0f, 7.0f, 7.0f}, 7.0f, 'B'}};
    int oneCount = 1;
    CHECK(removeStudent(one, oneCount, 7));
    CHECK(oneCount == 0);
    CHECK(!removeStudent(one, oneCount, 7));

    Student updateList[1] = {{2, "Binh", {7.0f, 7.0f, 7.0f}, 7.0f, 'B'}};
    CHECK(updateScore(updateList, 1, 2, 0, 1.0f));
    CHECK(updateList[0].scores[0] == 1.0f);
    CHECK(updateList[0].average == 5.0f);
    CHECK(updateList[0].rank == 'C');
    CHECK(updateScore(updateList, 1, 2, 0, 10.0f));
    CHECK(updateList[0].average == 8.0f);
    CHECK(updateList[0].rank == 'B');
    CHECK(updateScore(updateList, 1, 2, 1, 10.0f));
    CHECK(updateList[0].average == 9.0f);
    CHECK(updateList[0].rank == 'A');

    Student swapped[2] = {
        {1, "Zed", {9.0f, 9.0f, 9.0f}, 9.0f, 'A'},
        {2, "Amy", {5.0f, 5.0f, 5.0f}, 5.0f, 'C'}
    };
    swapStudent(swapped[0], swapped[1]);
    CHECK(swapped[0].id == 2 && swapped[1].id == 1);
    const Student* view[2] = {&swapped[0], &swapped[1]};
    sortView(view, 2, SortKey::AverageDescending);
    CHECK(view[0]->id == 1 && view[1]->id == 2);
    sortView(view, 2, SortKey::NameAscending);
    CHECK(view[0]->id == 2 && view[1]->id == 1);
    CHECK(swapped[0].id == 2 && swapped[1].id == 1); // Mảng gốc không đổi.

    Student* csv = nullptr;
    int csvCount = 0;
    int csvCapacity = 0;
    CHECK(loadCsv("data/students.csv", csv, csvCount, csvCapacity) == 10);
    CHECK(csvCount == 10 && csv[0].id == 1 && csv[9].id == 10);
    CHECK(saveCsv("build/test/roundtrip.csv", csv, csvCount));

    Student* reloaded = nullptr;
    int reloadedCount = 0;
    int reloadedCapacity = 0;
    CHECK(loadCsv("build/test/roundtrip.csv", reloaded, reloadedCount, reloadedCapacity) == 10);
    CHECK(reloadedCount == csvCount);
    for (int i = 0; i < csvCount; i++) {
        CHECK(reloaded[i].id == csv[i].id);
        CHECK(std::strcmp(reloaded[i].name, csv[i].name) == 0);
        CHECK(reloaded[i].scores[0] == csv[i].scores[0]);
        CHECK(reloaded[i].scores[1] == csv[i].scores[1]);
        CHECK(reloaded[i].scores[2] == csv[i].scores[2]);
        CHECK(reloaded[i].average == csv[i].average);
        CHECK(reloaded[i].rank == csv[i].rank);
    }
    CHECK(saveBinary("build/test/roundtrip.bin", csv, csvCount));
    CHECK(loadBinary("build/test/roundtrip.bin", reloaded, reloadedCount, reloadedCapacity) == 10);
    for (int i = 0; i < csvCount; i++) {
        CHECK(reloaded[i].id == csv[i].id);
        CHECK(std::strcmp(reloaded[i].name, csv[i].name) == 0);
        CHECK(reloaded[i].scores[0] == csv[i].scores[0]);
        CHECK(reloaded[i].scores[1] == csv[i].scores[1]);
        CHECK(reloaded[i].scores[2] == csv[i].scores[2]);
        CHECK(reloaded[i].average == csv[i].average);
        CHECK(reloaded[i].rank == csv[i].rank);
    }
    FILE* badBinary = std::fopen("build/test/bad.bin", "wb");
    CHECK(badBinary != nullptr);
    if (badBinary != nullptr) {
        std::fwrite("BAD", 1, 3, badBinary);
        std::fclose(badBinary);
    }
    CHECK(loadBinary("build/test/bad.bin", reloaded, reloadedCount, reloadedCapacity) == -1);
    CHECK(reloadedCount == 10 && reloaded[0].id == 1);
    delete[] reloaded;
    CHECK(loadCsv("data/students_invalid.csv", csv, csvCount, csvCapacity) == 2);
    CHECK(csvCount == 2 && csv[0].id == 1 && csv[1].id == 5);
    CHECK(loadCsv("data/empty.csv", csv, csvCount, csvCapacity) == 0);
    CHECK(csv == nullptr && csvCount == 0);
    FILE* comments = std::fopen("build/test/comments.csv", "w");
    CHECK(comments != nullptr);
    if (comments != nullptr) {
        std::fputs("# comment\n\n", comments);
        std::fclose(comments);
    }
    CHECK(loadCsv("build/test/comments.csv", csv, csvCount, csvCapacity) == 0);
    CHECK(saveBinary("build/test/empty.bin", csv, csvCount));
    CHECK(loadBinary("build/test/empty.bin", csv, csvCount, csvCapacity) == 0);
    CHECK(!saveCsv("build/test/no_dir/file.csv", csv, csvCount));
    CHECK(loadCsv("data/does_not_exist.csv", csv, csvCount, csvCapacity) == -1);
    CHECK(csv == nullptr && csvCount == 0);
    delete[] csv;
    std::remove("build/test/roundtrip.csv");
    std::remove("build/test/roundtrip.bin");
    std::remove("build/test/bad.bin");
    std::remove("build/test/comments.csv");
    std::remove("build/test/empty.bin");

    CHECK(!updateScore(updateList, 1, 2, -1, 5.0f));
    CHECK(!updateScore(updateList, 1, 2, 3, 5.0f));
    CHECK(!updateScore(updateList, 1, 2, 0, -1.0f));
    CHECK(!updateScore(updateList, 1, 2, 0, 10.5f));
    CHECK(!updateScore(updateList, 1, 99, 0, 5.0f));
    CHECK(!updateScore(nullptr, 1, 2, 0, 5.0f));
    CHECK(updateList[0].scores[0] == 10.0f);
    CHECK(updateList[0].average == 9.0f);
    CHECK(updateList[0].rank == 'A');

    if (failures > 0) {
        std::puts("Smoke tests: FAIL");
        return 1;
    }
    std::puts("Smoke tests: PASS");
    return 0;
}
