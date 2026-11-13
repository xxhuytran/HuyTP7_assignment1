#include "assignment1_app.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Chọn bài tập từ menu chung; trả 0 khi người dùng thoát.
int main(int argc, char* argv[]) {
    char line[32];
    while (true) {
        std::puts("\nChoose assignment:");
        std::puts("1 Student Score Manager");
        std::puts("2 Smart Home Device Controller");
        std::puts("0 Exit");
        std::printf("Choice: ");
        std::fflush(stdout);

        if (std::fgets(line, sizeof(line), stdin) == nullptr) break;
        int length = static_cast<int>(std::strlen(line));
        if (length > 0 && line[length - 1] != '\n' && !std::feof(stdin)) {
            int ch;
            while ((ch = std::getchar()) != '\n' && ch != EOF) {}
            std::puts("Invalid choice.");
            continue;
        }
        char* end;
        long choice = std::strtol(line, &end, 10);
        while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') end++;
        if (end == line || *end != '\0' || choice < 0 || choice > 2) {
            std::puts("Invalid choice.");
            continue;
        }
        if (choice == 0) break;
        if (choice == 1) {
            runAssignment1(argc, argv);
            if (std::feof(stdin)) break;
        } else {
            std::puts("Skeleton only: implementation pending.");
        }
    }
    std::puts("Goodbye.");
    return 0;
}
