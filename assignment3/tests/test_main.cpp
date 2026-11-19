#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "sdp/Algo.h"
#include "sdp/Errors.h"
#include "sdp/LinkedQueue.h"
#include "sdp/Logger.h"
#include "sdp/Parser.h"
#include "sdp/Pipeline.h"
#include "sdp/Report.h"
#include "sdp/RingBuffer.h"
#include "sdp/ThreadSafeQueue.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (cond) {                                                              \
            ++passed;                                                            \
        } else {                                                                 \
            ++failed;                                                            \
            std::cout << "  FAIL " << __FILE__ << ":" << __LINE__ << ": " #cond "\n"; \
        }                                                                        \
    } while (0)

// Chay pipeline, tra ve noi dung CSV
static std::string runCsv(const std::string& path, int workers, int* invalid = nullptr,
                          int* total = nullptr) {
    std::ostringstream logText;
    auto logger = std::make_shared<sdp::Logger>(logText);
    sdp::Pipeline p(logger, workers);
    p.run(path);
    bool ok = false;
    auto sorted = sdp::io::sortByAverage(p.registry(), ok);
    CHECK(ok);
    if (invalid) *invalid = p.invalidLines();
    if (total) *total = p.totalLines();
    return sdp::io::buildCsv(sorted, p);
}

static void testLinkedQueue() {
    std::cout << "LinkedQueue\n";
    sdp::ds::LinkedQueue<int> q;
    CHECK(q.empty());
    q.push(1);
    q.push(2);
    CHECK(q.size() == 2);
    CHECK(q.pop() == 1);
    CHECK(q.pop() == 2);
    bool thrown = false;
    try {
        q.pop();
    } catch (const sdp::EmptyContainerError&) {
        thrown = true;
    }
    CHECK(thrown);
    sdp::ds::LinkedQueue<int> big;  // huy 200000 node khong bi tran stack
    for (int i = 0; i < 200000; ++i) big.push(i);
    CHECK(big.size() == 200000);
}

static void testRingBuffer() {
    std::cout << "RingBuffer\n";
    sdp::ds::RingBuffer<double, 3> r;
    CHECK(r.average() == 0.0);  // rong, khong chia cho 0
    r.push(1);
    r.push(2);
    CHECK(r.average() == 1.5);
    r.push(3);
    r.push(10);  // ghi de 1 -> {2,3,10}
    CHECK(r.full());
    CHECK(r.average() == 5.0);
    sdp::ds::RingBuffer<std::string, 2> s;  // string dung duoc, chi average() bi cam (compile error)
    s.push("a");
    CHECK(s.size() == 1);
}

static void testThreadSafeQueue() {
    std::cout << "ThreadSafeQueue\n";
    sdp::ds::ThreadSafeQueue<int> q;
    long long sum = 0;
    std::thread consumer([&]() {
        int v;
        while (q.pop(v)) sum += v;
    });
    for (int i = 1; i <= 1000; ++i) q.push(i);
    q.close();
    consumer.join();
    CHECK(sum == 500500);
    int v;
    CHECK(!q.pop(v));  // da dong va rong
}

static void testAlgo() {
    std::cout << "algo\n";
    std::vector<int> a = {5, 3, 9, 1, 3, 7, 0};
    std::vector<int> b = a;
    sdp::algo::mergeSort(a, [](int x, int y) { return x > y; });
    std::sort(b.begin(), b.end(), [](int x, int y) { return x > y; });
    CHECK(a == b);
    std::vector<int> sorted = {1, 3, 5, 7, 9};
    auto id = [](int x) { return x; };
    CHECK(sdp::algo::binarySearch(sorted, 7, id) == 3);
    CHECK(sdp::algo::binarySearch(sorted, 4, id) == -1);
    std::vector<int> none;
    CHECK(sdp::algo::binarySearch(none, 1, id) == -1);
}

static void testParser() {
    std::cout << "parser\n";
    sdp::Sample s = sdp::io::parseLine("1000;T1;TEMP;25.5", 1);
    CHECK(s.timestamp == 1000 && s.sensorId == "T1" && s.value == 25.5);
    const char* bad[] = {"", "garbage", "1;T1;TEMP", "x;T1;TEMP;1", "1;;TEMP;1", "1;T1;FOO;1",
                         "1;T1;TEMP;abc", "1;T1;TEMP;nan"};
    for (const char* line : bad) {
        bool thrown = false;
        try {
            sdp::io::parseLine(line, 7);
        } catch (const sdp::ParseError& e) {
            thrown = (e.lineNumber() == 7);
        }
        CHECK(thrown);
    }
}

static void testTC01_sameResult() {
    std::cout << "TC-01 N=1 vs N=4 (and 2, 8)\n";
    std::string base = runCsv("data/sensor_log.txt", 1);
    CHECK(runCsv("data/sensor_log.txt", 2) == base);
    CHECK(runCsv("data/sensor_log.txt", 4) == base);
    CHECK(runCsv("data/sensor_log.txt", 8) == base);
}

static void testTC02_invalidLines() {
    std::cout << "TC-02 invalid lines\n";
    int invalid = 0, total = 0;
    std::ostringstream logText;
    auto logger = std::make_shared<sdp::Logger>(logText);
    sdp::Pipeline p(logger, 2);
    p.run("data/errors.txt");
    invalid = p.invalidLines();
    total = p.totalLines();
    CHECK(total == 6);
    CHECK(invalid == 4);  // bad line, abc, 200 (ngoai khoang), FOO
    CHECK(logText.str().find("line 2") != std::string::npos);
    CHECK(logText.str().find("line 3") != std::string::npos);
    CHECK(logText.str().find("line 4") != std::string::npos);
    CHECK(logText.str().find("line 6") != std::string::npos);
}

static void testTC03_missingFile() {
    std::cout << "TC-03 missing file\n";
    bool thrown = false;
    try {
        runCsv("data/does_not_exist.txt", 4);
    } catch (const sdp::FileError&) {
        thrown = true;
    }
    CHECK(thrown);
}

static std::ostringstream sink;  // log cua test bi nuot

static void testTC04_alarms() {
    std::cout << "TC-04 alarms\n";
    auto logger = std::make_shared<sdp::Logger>(sink);
    sdp::Pipeline p(logger, 4);
    p.run("data/alarms.txt");
    int total = 0;
    for (const auto& e : p.registry().sensors()) total += e.second->alarms();
    CHECK(total == 3);
    CHECK(p.registry().sensors().at("T1")->alarms() == 1);
    CHECK(p.registry().sensors().at("V1")->alarms() == 1);
    CHECK(p.registry().sensors().at("S1")->alarms() == 1);
}

static void testTC05_large() {
    std::cout << "TC-05 large file, N=8, 5 runs\n";
    std::string base;
    for (int i = 0; i < 5; ++i) {
        std::string csv = runCsv("data/large.txt", 8);
        if (i == 0) base = csv;
        CHECK(csv == base);
    }
    CHECK(runCsv("data/large.txt", 1) == base);
}

static void testTC06_search() {
    std::cout << "TC-06 search\n";
    auto logger = std::make_shared<sdp::Logger>(sink);
    sdp::Pipeline p(logger, 2);
    p.run("data/sensor_log.txt");
    auto found = sdp::io::findSensor(p.registry(), "T1");
    CHECK(found != nullptr && found->id() == "T1");
    CHECK(found->groupName() == "TEMP");
    CHECK(sdp::io::findSensor(p.registry(), "NOPE") == nullptr);
}

static void testTC07_emptyFile() {
    std::cout << "TC-07 empty file\n";
    std::string csv = runCsv("data/empty.txt", 4);
    CHECK(csv == "id,type,group,count,min,max,avg,moving_avg,alarms\n"
                 "total_lines,valid_lines,invalid_lines\n0,0,0\n");
}

int main() {
    testLinkedQueue();
    testRingBuffer();
    testThreadSafeQueue();
    testAlgo();
    testParser();
    testTC01_sameResult();
    testTC02_invalidLines();
    testTC03_missingFile();
    testTC04_alarms();
    testTC05_large();
    testTC06_search();
    testTC07_emptyFile();
    std::cout << "\nPassed: " << passed << ", failed: " << failed << '\n';
    return failed == 0 ? 0 : 1;
}
