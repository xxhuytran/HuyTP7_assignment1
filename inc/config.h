#ifndef CONFIG_H
#define CONFIG_H

#include <cstdio>

#define NUM_SUBJECTS 3
#define INITIAL_CAPACITY 4

// DEBUG_LOG bật TRACE; do/while giúp dùng an toàn trong if/else.
#ifdef DEBUG_LOG
#define TRACE(...) do {                                              \
    std::fprintf(stderr, "[TRACE %s:%d] ", __func__, __LINE__);     \
    std::fprintf(stderr, __VA_ARGS__);                               \
    std::fputc('\n', stderr);                                         \
} while (0)
#else
#define TRACE(...) do { } while (0)
#endif

// Kiểm tra subject có thuộc 0..2 không; trả true nếu hợp lệ.
inline bool validSubject(int subject) {
    return subject >= 0 && subject < NUM_SUBJECTS;
}

#endif
