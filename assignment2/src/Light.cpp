#include "Light.h"
#include <cstdio>
#include <ostream>
#include <stdexcept>

Light::Light() : Device(0, "Light"), mBrightness(100) {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Light constructed: id=%u\n", getId());
#endif
}

Light::Light(std::uint32_t id, const char* name, int brightness)
    : Device(id, name), mBrightness(0) {
    if (id == 0 || !setBrightness(brightness)) {
        throw std::invalid_argument("Light ID or brightness is invalid");
    }
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Light constructed: id=%u\n", getId());
#endif
}

Light::Light(const Light& other)
    : Device(other), mBrightness(other.mBrightness) {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Light copy constructed: id=%u\n", getId());
#endif
}

Light& Light::operator=(const Light& other) {
    if (this != &other) {
        Device::operator=(other);
        mBrightness = other.mBrightness;
    }
    return *this;
}

Light::~Light() {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Light destroyed: id=%u\n", getId());
#endif
}

int Light::getBrightness() const {
    return mBrightness;
}

bool Light::setBrightness(int brightness) {
    if (brightness < 0 || brightness > 100) {
        return false;
    }
    mBrightness = brightness;
    return true;
}

double Light::powerUsage() const {
    if (!isOn()) {
        return 0.0;
    }
    return 10.0 * mBrightness / 100.0;
}

void Light::status(std::ostream& out) const {
    out << "Light [ID " << getId() << "] " << getName();
    if (isOn()) {
        out << " - on";
    } else {
        out << " - off";
    }
    out << ", brightness " << mBrightness << "%";
    out << ", power " << powerUsage() << " W";
}

int Light::settingCount() const {
    return 1;
}

const char* Light::settingName(int index) const {
    if (index == 0) {
        return "brightness";
    }
    return nullptr;
}

bool Light::applySetting(int index, int value) {
    if (index != 0) {
        return false;
    }
    return setBrightness(value);
}

Device* Light::clone() const {
    return new Light(*this);
}
