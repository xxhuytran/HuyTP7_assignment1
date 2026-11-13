#include "Thermostat.h"
#include <cstdio>
#include <ostream>
#include <stdexcept>

Thermostat::Thermostat() : Device(0, "Thermostat"), mTargetTemp(25) {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Thermostat constructed: id=%u\n", getId());
#endif
}

Thermostat::Thermostat(std::uint32_t id, const char* name, int targetTemp)
    : Device(id, name), mTargetTemp(25) {
    if (id == 0 || !setTargetTemp(targetTemp)) {
        throw std::invalid_argument("Thermostat ID or temperature is invalid");
    }
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Thermostat constructed: id=%u\n", getId());
#endif
}

Thermostat::Thermostat(const Thermostat& other)
    : Device(other), mTargetTemp(other.mTargetTemp) {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Thermostat copy constructed: id=%u\n", getId());
#endif
}

Thermostat& Thermostat::operator=(const Thermostat& other) {
    if (this != &other) {
        Device::operator=(other);
        mTargetTemp = other.mTargetTemp;
    }
    return *this;
}

Thermostat::~Thermostat() {
#ifdef TRACE_LIFECYCLE
    std::fprintf(stderr, "Thermostat destroyed: id=%u\n", getId());
#endif
}

int Thermostat::getTargetTemp() const {
    return mTargetTemp;
}

bool Thermostat::setTargetTemp(int temperature) {
    if (temperature < 16 || temperature > 30) {
        return false;
    }
    mTargetTemp = temperature;
    return true;
}

double Thermostat::powerUsage() const {
    if (!isOn()) {
        return 0.0;
    }

    int difference = mTargetTemp - 25;
    if (difference < 0) {
        difference = -difference;
    }
    if (difference > 3) {
        return 1500.0;
    }
    return 800.0;
}

void Thermostat::status(std::ostream& out) const {
    out << "Thermostat [ID " << getId() << "] " << getName();
    if (isOn()) {
        out << " - on";
    } else {
        out << " - off";
    }
    out << ", target " << mTargetTemp << " C";
    out << ", power " << powerUsage() << " W";
}

int Thermostat::settingCount() const {
    return 1;
}

const char* Thermostat::settingName(int index) const {
    if (index == 0) {
        return "target temperature";
    }
    return nullptr;
}

bool Thermostat::applySetting(int index, int value) {
    if (index != 0) {
        return false;
    }
    return setTargetTemp(value);
}

Device* Thermostat::clone() const {
    return new Thermostat(*this);
}
