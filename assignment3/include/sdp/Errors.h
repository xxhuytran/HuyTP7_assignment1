#pragma once
#include <stdexcept>
#include <string>

namespace sdp {

// Pop tu container rong
class EmptyContainerError : public std::runtime_error {
public:
    explicit EmptyContainerError(const std::string& msg) : std::runtime_error(msg) {}
};

// Loi parse 1 dong log, kem so dong
class ParseError : public std::runtime_error {
public:
    ParseError(int lineNo, const std::string& msg)
        : std::runtime_error("line " + std::to_string(lineNo) + ": " + msg), lineNo_(lineNo) {}
    int lineNumber() const { return lineNo_; }

private:
    int lineNo_;
};

// Khong mo duoc file
class FileError : public std::runtime_error {
public:
    explicit FileError(const std::string& msg) : std::runtime_error(msg) {}
};

}  // namespace sdp
