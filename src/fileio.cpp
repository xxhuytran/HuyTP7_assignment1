#include "fileio.h"
#include <cstdio>

// Report a pending file operation; name identifies the called API.
static void notImplemented(const char* name) {
    std::fprintf(stderr, "Not implemented: %s\n", name);
}

int loadCsv(const char* path, Student*& arr, int& count, int& capacity) {
    (void)path; (void)arr; (void)count; (void)capacity;
    // TODO: Validate CSV rows, report line numbers, then insert valid students.
    notImplemented(__func__);
    return -2;
}

bool saveCsv(const char* path, const Student* arr, int count) {
    (void)path; (void)arr; (void)count;
    // TODO: Write records with fprintf and check all I/O errors.
    notImplemented(__func__);
    return false;
}

bool saveBinary(const char* path, const Student* arr, int count) {
    (void)path; (void)arr; (void)count;
    // TODO: Write a documented binary format with fwrite.
    notImplemented(__func__);
    return false;
}

int loadBinary(const char* path, Student*& arr, int& count, int& capacity) {
    (void)path; (void)arr; (void)count; (void)capacity;
    // TODO: Validate and reload a binary backup with fread.
    notImplemented(__func__);
    return -2;
}
