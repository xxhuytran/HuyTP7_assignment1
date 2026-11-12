#include "fileio.h"
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <cstdio>

// Bỏ khoảng trắng ở hai đầu text; trả con trỏ tới ký tự đầu tiên còn lại.
static char* trim(char* text) {
    while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n') text++;
    int length = static_cast<int>(std::strlen(text));
    while (length > 0 && (text[length - 1] == ' ' || text[length - 1] == '\t' ||
                           text[length - 1] == '\r' || text[length - 1] == '\n')) {
        text[--length] = '\0';
    }
    return text;
}

// Đọc số nguyên dương uint32 từ text; trả true nếu toàn bộ text hợp lệ.
static bool parseId(const char* text, uint32_t& id) {
    if (*text < '0' || *text > '9') return false;
    char* end;
    errno = 0;
    unsigned long value = std::strtoul(text, &end, 10);
    if (errno != 0 || *end != '\0' || value == 0 || value > UINT32_MAX) return false;
    id = static_cast<uint32_t>(value);
    return true;
}

// Đọc điểm từ text; trả true nếu điểm trong 0..10 và không còn ký tự thừa.
static bool parseScore(const char* text, float& score) {
    if (*text == '\0') return false;
    char* end;
    errno = 0;
    float value = std::strtof(text, &end);
    if (errno != 0 || end == text || *end != '\0' || !(value >= 0.0f && value <= 10.0f)) return false;
    score = value;
    return true;
}

// Tách một dòng CSV thành Student; trả false nếu thiếu/thừa trường hoặc dữ liệu sai.
static bool parseCsvRow(char* line, Student& student) {
    char* fields[5];
    fields[0] = line;
    for (int i = 1; i < 5; i++) {
        char* comma = std::strchr(fields[i - 1], ',');
        if (comma == nullptr) return false;
        *comma = '\0';
        fields[i] = comma + 1;
    }
    if (std::strchr(fields[4], ',') != nullptr) return false;
    for (int i = 0; i < 5; i++) fields[i] = trim(fields[i]);

    if (!parseId(fields[0], student.id)) return false;
    int nameLength = static_cast<int>(std::strlen(fields[1]));
    if (nameLength == 0 || nameLength >= 32) return false;
    std::strcpy(student.name, fields[1]);
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        if (!parseScore(fields[i + 2], student.scores[i])) return false;
    }
    return prepareStudent(student);
}

// Kiểm tra một bản ghi trước khi lưu; trả true nếu dữ liệu nhất quán.
static bool validSavedStudent(const Student& student) {
    Student copy = student;
    if (!prepareStudent(copy)) return false;
    if (copy.average != student.average || copy.rank != student.rank) return false;
    for (int i = 0; i < 32 && student.name[i] != '\0'; i++) {
        if (student.name[i] == ',' || student.name[i] == '\n' || student.name[i] == '\r') return false;
    }
    return true;
}

// Kiểm tra mảng trước khi lưu; trả true nếu ID tăng dần và các bản ghi hợp lệ.
static bool validList(const Student* arr, int count) {
    if (count < 0 || (count > 0 && arr == nullptr)) return false;
    for (int i = 0; i < count; i++) {
        if (!validSavedStudent(arr[i])) return false;
        if (i > 0 && arr[i - 1].id >= arr[i].id) return false;
    }
    return true;
}

int loadCsv(const char* path, Student*& arr, int& count, int& capacity) {
    if (path == nullptr || *path == '\0') return -1;
    FILE* file = std::fopen(path, "r");
    if (file == nullptr) {
        std::perror(path);
        return -1;
    }

    Student* loaded = nullptr;
    int loadedCount = 0;
    int loadedCapacity = 0;
    int lineNumber = 0;
    bool firstDataLine = true;
    bool failed = false;
    char line[512];
    while (std::fgets(line, sizeof(line), file) != nullptr) {
        lineNumber++;
        int length = static_cast<int>(std::strlen(line));
        if (length > 0 && line[length - 1] != '\n' && !std::feof(file)) {
            int ch;
            while ((ch = std::fgetc(file)) != '\n' && ch != EOF) {}
            std::fprintf(stderr, "CSV line %d: too long\n", lineNumber);
            continue;
        }
        char* data = trim(line);
        if (*data == '\0' || *data == '#') continue;
        if (firstDataLine && std::strcmp(data, "id,name,cpp,mcu,english") == 0) {
            firstDataLine = false;
            continue;
        }
        firstDataLine = false;
        Student student{};
        if (!parseCsvRow(data, student) ||
            findById(loaded, 0, loadedCount - 1, student.id) >= 0) {
            std::fprintf(stderr, "CSV line %d: invalid record\n", lineNumber);
            continue;
        }
        if (!addStudent(loaded, loadedCount, loadedCapacity, student)) {
            failed = true;
            break;
        }
    }
    if (std::ferror(file)) failed = true;
    if (std::fclose(file) != 0) failed = true;
    if (failed) {
        delete[] loaded;
        std::fprintf(stderr, "Could not read CSV: %s\n", path);
        return -1;
    }
    delete[] arr;
    arr = loaded;
    count = loadedCount;
    capacity = loadedCapacity;
    TRACE("Loaded %d CSV students from %s", count, path);
    return loadedCount;
}

bool saveCsv(const char* path, const Student* arr, int count) {
    if (path == nullptr || *path == '\0' || !validList(arr, count)) return false;
    FILE* file = std::fopen(path, "w");
    if (file == nullptr) return false;
    bool ok = std::fprintf(file, "id,name,cpp,mcu,english\n") >= 0;
    for (int i = 0; i < count && ok; i++) {
        const Student& s = arr[i];
        ok = std::fprintf(file, "%u,%s,%.9g,%.9g,%.9g\n",
                          static_cast<unsigned int>(s.id), s.name,
                          s.scores[0], s.scores[1], s.scores[2]) >= 0;
    }
    if (std::fclose(file) != 0) ok = false;
    return ok;
}

bool saveBinary(const char* path, const Student* arr, int count) {
    if (path == nullptr || *path == '\0' || !validList(arr, count)) return false;
    FILE* file = std::fopen(path, "wb");
    if (file == nullptr) return false;
    const char magic[4] = {'S', 'S', 'M', '1'};
    uint32_t size = static_cast<uint32_t>(count);
    bool ok = std::fwrite(magic, 1, 4, file) == 4 &&
              std::fwrite(&size, sizeof(size), 1, file) == 1;
    for (int i = 0; i < count && ok; i++) {
        const Student& s = arr[i];
        ok = std::fwrite(&s.id, sizeof(s.id), 1, file) == 1 &&
             std::fwrite(s.name, 1, sizeof(s.name), file) == sizeof(s.name) &&
             std::fwrite(s.scores, sizeof(float), NUM_SUBJECTS, file) == NUM_SUBJECTS &&
             std::fwrite(&s.average, sizeof(s.average), 1, file) == 1 &&
             std::fwrite(&s.rank, sizeof(s.rank), 1, file) == 1;
    }
    if (std::fclose(file) != 0) ok = false;
    return ok;
}

int loadBinary(const char* path, Student*& arr, int& count, int& capacity) {
    if (path == nullptr || *path == '\0') return -1;
    FILE* file = std::fopen(path, "rb");
    if (file == nullptr) return -1;
    char magic[4];
    uint32_t size = 0;
    bool ok = std::fread(magic, 1, 4, file) == 4 &&
              std::memcmp(magic, "SSM1", 4) == 0 &&
              std::fread(&size, sizeof(size), 1, file) == 1 && size <= INT_MAX;
    Student* loaded = nullptr;
    int loadedCount = 0;
    int loadedCapacity = 0;
    for (uint32_t i = 0; i < size && ok; i++) {
        Student s{};
        ok = std::fread(&s.id, sizeof(s.id), 1, file) == 1 &&
             std::fread(s.name, 1, sizeof(s.name), file) == sizeof(s.name) &&
             std::fread(s.scores, sizeof(float), NUM_SUBJECTS, file) == NUM_SUBJECTS &&
             std::fread(&s.average, sizeof(s.average), 1, file) == 1 &&
             std::fread(&s.rank, sizeof(s.rank), 1, file) == 1;
        if (ok) {
            ok = validSavedStudent(s) &&
                 (loadedCount == 0 || loaded[loadedCount - 1].id < s.id) &&
                 addStudent(loaded, loadedCount, loadedCapacity, s);
        }
    }
    if (ok && std::fgetc(file) != EOF) ok = false;
    if (std::ferror(file)) ok = false;
    if (std::fclose(file) != 0) ok = false;
    if (!ok) {
        delete[] loaded;
        return -1;
    }
    delete[] arr;
    arr = loaded;
    count = loadedCount;
    capacity = loadedCapacity;
    return loadedCount;
}
