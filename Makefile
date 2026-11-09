CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0 -Iinc -MMD -MP

APP_SRCS = src/main.cpp src/student.cpp src/fileio.cpp
TEST_SRCS = tests/test_main.cpp src/student.cpp src/fileio.cpp

APP_OBJS = $(patsubst %.cpp,build/normal/%.o,$(APP_SRCS))
DEBUG_OBJS = $(patsubst %.cpp,build/debug-log/%.o,$(APP_SRCS))
TEST_OBJS = $(patsubst %.cpp,build/test/%.o,$(TEST_SRCS))
ASAN_OBJS = $(patsubst %.cpp,build/asan/%.o,$(APP_SRCS))

.PHONY: all run debug-log test asan clean
all: build/normal/student-score-manager

build/normal/student-score-manager: $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/debug-log/student-score-manager: $(DEBUG_OBJS)
	$(CXX) $(CXXFLAGS) -DDEBUG_LOG $^ -o $@

debug-log: build/debug-log/student-score-manager

build/test/test-main: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: build/test/test-main
	./build/test/test-main

build/asan/student-score-manager: $(ASAN_OBJS)
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer $^ -o $@

asan: build/asan/student-score-manager

run: all
	./build/normal/student-score-manager data/students.csv

build/normal/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/debug-log/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -DDEBUG_LOG -c $< -o $@

build/test/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/asan/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer -c $< -o $@

clean:
	rm -rf build

-include $(APP_OBJS:.o=.d) $(DEBUG_OBJS:.o=.d) $(TEST_OBJS:.o=.d) $(ASAN_OBJS:.o=.d)
