#include "sdp/Parser.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "sdp/Errors.h"

namespace sdp {
namespace io {

static std::vector<std::string> split(const std::string& line) {
    std::vector<std::string> parts;
    std::string current;
    for (char c : line) {
        if (c == ';') {
            parts.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    parts.push_back(current);
    return parts;
}

Sample parseLine(const std::string& rawLine, int lineNo) {
    std::string line = rawLine;
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();  // file kieu Windows
    }

    std::vector<std::string> f = split(line);
    if (f.size() != 4) {
        throw ParseError(lineNo, "expected 4 fields, got " + std::to_string(f.size()));
    }

    Sample s;
    s.lineNo = lineNo;

    // timestamp
    char* end = nullptr;
    errno = 0;
    s.timestamp = std::strtoll(f[0].c_str(), &end, 10);
    if (f[0].empty() || *end != '\0' || errno != 0 || s.timestamp < 0) {
        throw ParseError(lineNo, "bad timestamp '" + f[0] + "'");
    }

    // sensor id
    if (f[1].empty()) {
        throw ParseError(lineNo, "empty sensor id");
    }
    s.sensorId = f[1];

    // type
    if (f[2] != "TEMP" && f[2] != "VOLT" && f[2] != "SPEED") {
        throw ParseError(lineNo, "unknown type '" + f[2] + "'");
    }
    s.type = f[2];

    // value
    errno = 0;
    s.value = std::strtod(f[3].c_str(), &end);
    if (f[3].empty() || *end != '\0' || !std::isfinite(s.value)) {
        throw ParseError(lineNo, "bad value '" + f[3] + "'");
    }
    return s;
}

}  // namespace io
}  // namespace sdp
