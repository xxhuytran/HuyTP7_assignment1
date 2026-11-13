#ifndef ASSIGNMENT2_LIGHT_H
#define ASSIGNMENT2_LIGHT_H

#include "Device.h"

class Light : public Device {
private:
    int mBrightness;

public:
    Light();
    Light(std::uint32_t id, const char* name, int brightness);
    Light(const Light& other);
    Light& operator=(const Light& other);
    ~Light() override;

    int getBrightness() const;
    bool setBrightness(int brightness);

    double powerUsage() const override;
    void status(std::ostream& out) const override;
    int settingCount() const override;
    const char* settingName(int index) const override;
    bool applySetting(int index, int value) override;
    Device* clone() const override;
};

#endif
