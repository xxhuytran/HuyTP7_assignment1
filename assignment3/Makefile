CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2 -pthread -Iinclude -MMD -MP
LDFLAGS = -pthread

LIB_SRCS = src/Parser.cpp src/Sensor.cpp src/Pipeline.cpp src/Report.cpp
LIB_OBJS = $(LIB_SRCS:%.cpp=build/%.o)

.PHONY: all test run clean
all: build/sensor_processor

build/sensor_processor: $(LIB_OBJS) build/src/main.o
	$(CXX) $^ -o $@ $(LDFLAGS)

build/test_main: $(LIB_OBJS) build/tests/test_main.o
	$(CXX) $^ -o $@ $(LDFLAGS)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: build/test_main
	./build/test_main

run: build/sensor_processor
	./build/sensor_processor data/sensor_log.txt -n 4 < /dev/null

clean:
	rm -rf build report.csv

-include $(LIB_OBJS:.o=.d) build/src/main.d build/tests/test_main.d
