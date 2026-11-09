#include "student.h"
#include <cstdio>

static int failures = 0;
#define CHECK(condition) do { if (!(condition)) { std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); ++failures; } } while (0)

// Exercise the implemented inline subject index helper; return nonzero on failure.
int main() {
    CHECK(validSubject(0));
    CHECK(validSubject(NUM_SUBJECTS - 1));
    CHECK(!validSubject(-1));
    CHECK(!validSubject(NUM_SUBJECTS));
    std::printf("Smoke tests: %s\n", failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
