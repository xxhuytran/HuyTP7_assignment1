#include "sdp/Pipeline.h"

#include <exception>
#include <fstream>
#include <thread>
#include <vector>

#include "sdp/Errors.h"
#include "sdp/Parser.h"
#include "sdp/ThreadSafeQueue.h"

namespace sdp {

Pipeline::Pipeline(std::shared_ptr<Logger> logger, int workers)
    : logger_(std::move(logger)), workers_(workers < 1 ? 1 : workers) {}

void Pipeline::run(const std::string& path) {
    registry_ = std::make_unique<SensorRegistry>();
    totalLines_ = 0;
    invalidLines_ = 0;

    ds::ThreadSafeQueue<Sample> queue;
    std::vector<std::exception_ptr> errors(workers_ + 1);  // moi thread 1 o rieng

    // Producer: doc file, parse, day vao queue
    std::thread producer([&]() {
        try {
            std::ifstream in(path);
            if (!in) {
                throw FileError("cannot open input file: " + path);
            }
            std::string line;
            int lineNo = 0;
            while (std::getline(in, line)) {
                ++lineNo;
                ++totalLines_;
                try {
                    queue.push(io::parseLine(line, lineNo));
                } catch (const ParseError& e) {
                    ++invalidLines_;
                    logger_->log(std::string("invalid ") + e.what());
                }
            }
        } catch (...) {
            errors[0] = std::current_exception();
        }
        queue.close();  // luon dong queue de worker khong bi treo
    });

    // Workers: validate va cap nhat registry
    std::vector<std::thread> workers;
    for (int i = 0; i < workers_; ++i) {
        workers.emplace_back([&, i]() {
            try {
                Sample s;
                while (queue.pop(s)) {
                    if (!isInRange(s)) {
                        ++invalidLines_;
                        logger_->log("invalid line " + std::to_string(s.lineNo) +
                                     ": value out of range for " + s.type);
                        continue;
                    }
                    registry_->addSample(s);
                }
            } catch (...) {
                errors[i + 1] = std::current_exception();
            }
        });
    }

    producer.join();
    for (std::thread& t : workers) {
        t.join();
    }

    // Nem lai exception tu thread (sau khi da join het)
    for (std::exception_ptr& e : errors) {
        if (e) {
            std::rethrow_exception(e);
        }
    }

    registry_->finalizeAll();  // main thread
}

}  // namespace sdp
