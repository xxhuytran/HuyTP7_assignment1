#ifndef FILEIO_H
#define FILEIO_H

#include "student.h"

// Read path into arr and update count/capacity; return records loaded, -1 on error, -2 while pending.
int loadCsv(const char* path, Student*& arr, int& count, int& capacity);
// Write count students to CSV path; return false on error or while pending.
bool saveCsv(const char* path, const Student* arr, int count);
// Write count students to binary path; return false on error or while pending.
bool saveBinary(const char* path, const Student* arr, int count);
// Read binary path into arr and update count/capacity; return records, -1 on error, -2 while pending.
int loadBinary(const char* path, Student*& arr, int& count, int& capacity);

#endif
