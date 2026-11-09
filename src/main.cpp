#include "student.h"
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cctype>

// Parse a complete menu line; return 0..9, or -1 for invalid input.
static int parseChoice(const char* line) {
    if (line == nullptr) return -1;
    while (std::isspace(static_cast<unsigned char>(*line))) ++line;
    if (*line == '\0') return -1;
    char* end = nullptr;
    errno = 0;
    const long value = std::strtol(line, &end, 10);
    if (errno != 0 || end == line) return -1;
    while (std::isspace(static_cast<unsigned char>(*end))) ++end;
    return (*end == '\0' && value >= 0 && value <= 9) ? static_cast<int>(value) : -1;
}

// Show the assignment menu; no parameters or return value.
static void showMenu() {
    std::puts("\nStudent Score Manager (scaffold)");
    std::puts("1 List all\n2 Add\n3 Update score\n4 Delete\n5 Search by ID");
    std::puts("6 Statistics\n7 Sort\n8 Save CSV\n9 Save/Load binary backup\n0 Exit");
    std::fputs("Choice: ", stdout);
    std::fflush(stdout);
}

// Run the menu with an optional CSV path; return 0 on normal exit.
int main(int argc, char* argv[]) {
    const char* csvPath = argc > 1 ? argv[1] : "data/students.csv";
    std::printf("CSV source: %s (loading not implemented)\n", csvPath);
    char line[128];
    for (;;) {
        showMenu();
        if (std::fgets(line, sizeof(line), stdin) == nullptr) {
            std::puts("\nEOF: exiting.");
            break;
        }
        bool complete = false;
        for (int i = 0; line[i] != '\0'; ++i) {
            if (line[i] == '\n') { complete = true; break; }
        }
        if (!complete && !std::feof(stdin)) {
            int ch;
            while ((ch = std::getchar()) != '\n' && ch != EOF) {}
            std::puts("Invalid choice.");
            continue;
        }
        const int choice = parseChoice(line);
        if (choice == 0) { std::puts("Goodbye."); break; }
        if (choice < 0) { std::puts("Invalid choice."); continue; }
        std::printf("Not implemented: menu option %d\n", choice);
    }
    return 0;
}
