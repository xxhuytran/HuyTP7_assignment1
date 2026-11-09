#ifndef CONFIG_H
#define CONFIG_H

#include <cstdio>

#define NUM_SUBJECTS 3
#define INITIAL_CAPACITY 4

#ifdef DEBUG_LOG
#define TRACE(...) do { std::fprintf(stderr, "[TRACE %s:%d] ", __func__, __LINE__); std::fprintf(stderr, __VA_ARGS__); std::fputc('\n', stderr); } while (0)
#else
#define TRACE(...) do { } while (0)
#endif

// Return true when a subject index identifies one of the three scores.
inline bool validSubject(int subject) { return subject >= 0 && subject < NUM_SUBJECTS; }

#endif
