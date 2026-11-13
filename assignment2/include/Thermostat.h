#ifndef ASSIGNMENT2_THERMOSTAT_H
#define ASSIGNMENT2_THERMOSTAT_H

#include "Device.h"

class Thermostat : public Device {
private:
    int mTargetTemp;

public:
    Thermostat();
    Thermostat(std::uint32_t id, const char* name, int targetTemp);
    Thermostat(const Thermostat& other);
    Thermostat& operator=(const Thermostat& other);
    ~Thermostat() override;

    int getTargetTemp() const;
    bool setTargetTemp(int temperature);

    double powerUsage() const override;
    void status(std::ostream& out) const override;
    int settingCount() const override;
    const char* settingName(int index) const override;
    bool applySetting(int index, int value) override;
    Device* clone() const override;
};

#endif
