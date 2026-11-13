CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0 -Iinclude -MMD -MP

COMMON_SRCS = src/Device.cpp src/Light.cpp src/Thermostat.cpp src/SecurityCam.cpp src/Room.cpp src/ConsoleApp.cpp
COMMON_OBJS = $(patsubst %.cpp,build/normal/%.o,$(COMMON_SRCS))
TRACE_OBJS = $(patsubst %.cpp,build/trace/%.o,$(COMMON_SRCS))
APP_OBJ = build/normal/src/main.o
DEMO_OBJ = build/normal/demo/demo.o
TEST_OBJ = build/normal/tests/test_main.o
DEVICE_TEST_OBJ = build/normal/tests/device_test.o
LIGHT_TEST_OBJ = build/normal/tests/light_test.o
THERMOSTAT_TEST_OBJ = build/normal/tests/thermostat_test.o
SECURITY_CAM_TEST_OBJ = build/normal/tests/security_cam_test.o
TRACE_APP_OBJ = build/trace/src/main.o
TRACE_DEVICE_TEST_OBJ = build/trace/tests/device_test.o
TRACE_LIGHT_TEST_OBJ = build/trace/tests/light_test.o
TRACE_THERMOSTAT_TEST_OBJ = build/trace/tests/thermostat_test.o
TRACE_SECURITY_CAM_TEST_OBJ = build/trace/tests/security_cam_test.o
ASAN_DEVICE_OBJ = build/asan/src/Device.o
ASAN_DEVICE_TEST_OBJ = build/asan/tests/device_test.o
ASAN_LIGHT_OBJ = build/asan/src/Light.o
ASAN_LIGHT_TEST_OBJ = build/asan/tests/light_test.o
ASAN_THERMOSTAT_OBJ = build/asan/src/Thermostat.o
ASAN_THERMOSTAT_TEST_OBJ = build/asan/tests/thermostat_test.o
ASAN_SECURITY_CAM_OBJ = build/asan/src/SecurityCam.o
ASAN_SECURITY_CAM_TEST_OBJ = build/asan/tests/security_cam_test.o

.PHONY: app demo test trace device-trace device-asan light-trace light-asan thermostat-trace thermostat-asan security-cam-trace security-cam-asan check clean
app: build/normal/smart-home

demo: build/normal/smart-home-demo

test: build/normal/device-test build/normal/light-test build/normal/thermostat-test build/normal/security-cam-test build/normal/test-main
	./build/normal/device-test
	./build/normal/light-test
	./build/normal/thermostat-test
	./build/normal/security-cam-test
	./build/normal/test-main

trace: build/trace/smart-home
device-trace: build/trace/device-test
device-asan: build/asan/device-test
	./build/asan/device-test
light-trace: build/trace/light-test
light-asan: build/asan/light-test
	./build/asan/light-test
thermostat-trace: build/trace/thermostat-test
thermostat-asan: build/asan/thermostat-test
	./build/asan/thermostat-test
security-cam-trace: build/trace/security-cam-test
security-cam-asan: build/asan/security-cam-test
	./build/asan/security-cam-test

build/normal/smart-home: $(COMMON_OBJS) $(APP_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/normal/smart-home-demo: $(COMMON_OBJS) $(DEMO_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/normal/test-main: $(COMMON_OBJS) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/normal/device-test: build/normal/src/Device.o $(DEVICE_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/normal/light-test: build/normal/src/Device.o build/normal/src/Light.o $(LIGHT_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/normal/thermostat-test: build/normal/src/Device.o build/normal/src/Thermostat.o $(THERMOSTAT_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/normal/security-cam-test: build/normal/src/Device.o build/normal/src/SecurityCam.o $(SECURITY_CAM_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/trace/smart-home: $(TRACE_OBJS) $(TRACE_APP_OBJ)
	$(CXX) $(CXXFLAGS) -DTRACE_LIFECYCLE $^ -o $@

build/trace/device-test: build/trace/src/Device.o $(TRACE_DEVICE_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -DTRACE_LIFECYCLE $^ -o $@

build/trace/light-test: build/trace/src/Device.o build/trace/src/Light.o $(TRACE_LIGHT_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -DTRACE_LIFECYCLE $^ -o $@

build/trace/thermostat-test: build/trace/src/Device.o build/trace/src/Thermostat.o $(TRACE_THERMOSTAT_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -DTRACE_LIFECYCLE $^ -o $@

build/trace/security-cam-test: build/trace/src/Device.o build/trace/src/SecurityCam.o $(TRACE_SECURITY_CAM_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -DTRACE_LIFECYCLE $^ -o $@

build/asan/device-test: $(ASAN_DEVICE_OBJ) $(ASAN_DEVICE_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer $^ -o $@

build/asan/light-test: $(ASAN_DEVICE_OBJ) $(ASAN_LIGHT_OBJ) $(ASAN_LIGHT_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer $^ -o $@

build/asan/thermostat-test: $(ASAN_DEVICE_OBJ) $(ASAN_THERMOSTAT_OBJ) $(ASAN_THERMOSTAT_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer $^ -o $@

build/asan/security-cam-test: $(ASAN_DEVICE_OBJ) $(ASAN_SECURITY_CAM_OBJ) $(ASAN_SECURITY_CAM_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer $^ -o $@

build/normal/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/trace/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -DTRACE_LIFECYCLE -c $< -o $@

build/asan/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer -c $< -o $@

check:
	@for header in include/*.h; do \
	  printf '#include "%s"\n' "$$(basename "$$header")" | $(CXX) -std=c++17 -Wall -Wextra -Iinclude -x c++ -fsyntax-only - || exit 1; \
	done
	$(CXX) -std=c++17 -Wall -Wextra -Iinclude -fsyntax-only $(COMMON_SRCS) src/main.cpp demo/demo.cpp tests/test_main.cpp tests/device_test.cpp tests/light_test.cpp tests/thermostat_test.cpp tests/security_cam_test.cpp tests/headers_check.cpp

clean:
	rm -rf build

-include $(COMMON_OBJS:.o=.d) $(TRACE_OBJS:.o=.d) $(APP_OBJ:.o=.d) $(DEMO_OBJ:.o=.d) $(TEST_OBJ:.o=.d) $(DEVICE_TEST_OBJ:.o=.d) $(LIGHT_TEST_OBJ:.o=.d) $(THERMOSTAT_TEST_OBJ:.o=.d) $(SECURITY_CAM_TEST_OBJ:.o=.d) $(TRACE_APP_OBJ:.o=.d) $(TRACE_DEVICE_TEST_OBJ:.o=.d) $(TRACE_LIGHT_TEST_OBJ:.o=.d) $(TRACE_THERMOSTAT_TEST_OBJ:.o=.d) $(TRACE_SECURITY_CAM_TEST_OBJ:.o=.d) $(ASAN_DEVICE_OBJ:.o=.d) $(ASAN_DEVICE_TEST_OBJ:.o=.d) $(ASAN_LIGHT_OBJ:.o=.d) $(ASAN_LIGHT_TEST_OBJ:.o=.d) $(ASAN_THERMOSTAT_OBJ:.o=.d) $(ASAN_THERMOSTAT_TEST_OBJ:.o=.d) $(ASAN_SECURITY_CAM_OBJ:.o=.d) $(ASAN_SECURITY_CAM_TEST_OBJ:.o=.d)
