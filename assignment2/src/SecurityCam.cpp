#include "SecurityCam.h"
#include <cstdio>
#include <ostream>
#include <stdexcept>

SecurityCam::SecurityCam()
    : Device(0, "SecurityCam"), mResolution(1080), mIsRecording(false) {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "SecurityCam constructed: id=%u\n", getId());
#endif
}

SecurityCam::SecurityCam(std::uint32_t id, const char* name, int resolution, bool recording)
    : Device(id, name), mResolution(1080), mIsRecording(recording) {
    if (id == 0 || !setResolution(resolution)) {
        throw std::invalid_argument("SecurityCam ID or resolution is invalid");
    }
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "SecurityCam constructed: id=%u\n", getId());
#endif
}

SecurityCam::SecurityCam(const SecurityCam& other)
    : Device(other), mResolution(other.mResolution), mIsRecording(other.mIsRecording) {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "SecurityCam copy constructed: id=%u\n", getId());
#endif
}

SecurityCam& SecurityCam::operator=(const SecurityCam& other) {
    if (this != &other) {
        Device::operator=(other);
        mResolution = other.mResolution;
        mIsRecording = other.mIsRecording;
    }
    return *this;
}

SecurityCam::~SecurityCam() {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "SecurityCam destroyed: id=%u\n", getId());
#endif
}

int SecurityCam::getResolution() const {
    return mResolution;
}

bool SecurityCam::isRecording() const {
    return mIsRecording;
}

bool SecurityCam::setResolution(int resolution) {
    if (resolution != 720 && resolution != 1080 && resolution != 2160) {
        return false;
    }
    mResolution = resolution;
    return true;
}

bool SecurityCam::setRecording(bool recording) {
    mIsRecording = recording;
    return true;
}

double SecurityCam::powerUsage() const {
    if (!isOn()) {
        return 0.0;
    }
    if (mIsRecording) {
        return 8.0;
    }
    return 5.0;
}

void SecurityCam::status(std::ostream& out) const {
    out << "SecurityCam [ID " << getId() << "] " << getName();
    if (isOn()) {
        out << " - on";
    } else {
        out << " - off";
    }
    out << ", resolution " << mResolution << "p";
    if (mIsRecording) {
        out << ", recording on";
    } else {
        out << ", recording off";
    }
    out << ", power " << powerUsage() << " W";
}

int SecurityCam::settingCount() const {
    return 2;
}

const char* SecurityCam::settingName(int index) const {
    if (index == 0) {
        return "resolution";
    }
    if (index == 1) {
        return "recording";
    }
    return nullptr;
}

bool SecurityCam::applySetting(int index, int value) {
    if (index == 0) {
        return setResolution(value);
    }
    if (index == 1) {
        if (value != 0 && value != 1) {
            return false;
        }
        if (value == 1) {
            return setRecording(true);
        }
        return setRecording(false);
    }
    return false;
}

Device* SecurityCam::clone() const {
    return new SecurityCam(*this);
}
