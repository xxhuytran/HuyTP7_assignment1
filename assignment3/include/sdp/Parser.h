#pragma once
#include <string>

#include "Sample.h"

namespace sdp {
namespace io {

// Parse "timestamp_ms;sensor_id;type;value". Sai thi throw ParseError
Sample parseLine(const std::string& line, int lineNo);

}  // namespace io
}  // namespace sdp
