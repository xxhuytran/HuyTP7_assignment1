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

// Add s to arr, updating count/capacity; return false for failure or pending implementation.
bool addStudent(Student*& arr, int& count, int& capacity, const Student& s);
// Remove id from arr and update count; return false for failure or pending implementation.
bool removeStudent(Student* arr, int& count, uint32_t id);
// Set one score by id (subject 0..2); return false for failure or pending implementation.
bool updateScore(Student* arr, int count, uint32_t id, int subject, float score);
// Search sorted arr within inclusive low..high; return index, -1 if absent, -2 while pending.
int findById(const Student* arr, int low, int high, uint32_t id);
// Average n scores; return NaN for invalid input or pending implementation.
float calcAverage(const float* scores, int n = NUM_SUBJECTS);
// Print one student; currently reports pending implementation.
void printStudent(const Student& s);
// Print count students from arr; currently reports pending implementation.
void printStudent(const Student* arr, int count);
// Swap a and b; currently reports pending implementation.
void swapStudent(Student& a, Student& b);
// Sort count pointers in view by key, leaving original array untouched; currently pending.
void sortView(const Student** view, int count, SortKey key);

#endif
