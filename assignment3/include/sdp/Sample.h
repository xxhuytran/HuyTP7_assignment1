#pragma once
#include <string>

namespace sdp {

struct Sample {
    long long timestamp = 0;
    int lineNo = 0;
    std::string sensorId;
    std::string type;  // TEMP, VOLT, SPEED
    double value = 0.0;
};

// Gia tri nam trong khoang hop le cua type chua?
bool isInRange(const Sample& s);

}  // namespace sdp
