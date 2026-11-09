#include "student.h"
#include <cstdio>
#include <limits>

// Report a pending student operation; name identifies the called API.
static void notImplemented(const char* name) {
    std::fprintf(stderr, "Not implemented: %s\n", name);
}

bool addStudent(Student*& arr, int& count, int& capacity, const Student& s) {
    (void)arr; (void)count; (void)capacity; (void)s;
    // TODO: Validate, grow dynamic array, and insert by ascending ID.
    notImplemented(__func__);
    return false;
}

bool removeStudent(Student* arr, int& count, uint32_t id) {
    (void)arr; (void)count; (void)id;
    // TODO: Locate ID and shift later elements left.
    notImplemented(__func__);
    return false;
}

bool updateScore(Student* arr, int count, uint32_t id, int subject, float score) {
    (void)arr; (void)count; (void)id; (void)subject; (void)score;
    // TODO: Validate score and subject, then recompute average and rank.
    notImplemented(__func__);
    return false;
}

int findById(const Student* arr, int low, int high, uint32_t id) {
    (void)arr; (void)low; (void)high; (void)id;
    // TODO: Recursive binary search over ascending IDs.
    notImplemented(__func__);
    return -2;
}

float calcAverage(const float* scores, int n) {
    (void)scores; (void)n;
    // TODO: Validate input and compute arithmetic mean.
    notImplemented(__func__);
    return std::numeric_limits<float>::quiet_NaN();
}

void printStudent(const Student& s) {
    (void)s;
    // TODO: Print one formatted record.
    notImplemented(__func__);
}

void printStudent(const Student* arr, int count) {
    (void)arr; (void)count;
    // TODO: Print a table of records.
    notImplemented(__func__);
}

void swapStudent(Student& a, Student& b) {
    (void)a; (void)b;
    // TODO: Swap two Student values.
    notImplemented(__func__);
}

void sortView(const Student** view, int count, SortKey key) {
    (void)view; (void)count; (void)key;
    // TODO: Sort pointers only, preserving the ascending-ID source array.
    notImplemented(__func__);
}
