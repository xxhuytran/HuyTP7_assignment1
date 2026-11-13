#ifndef ASSIGNMENT2_SECURITY_CAM_H
#define ASSIGNMENT2_SECURITY_CAM_H

#include "Device.h"

class SecurityCam : public Device {
private:
    int mResolution;
    bool mIsRecording;

public:
    SecurityCam();
    SecurityCam(std::uint32_t id, const char* name, int resolution, bool recording);
    SecurityCam(const SecurityCam& other);
    SecurityCam& operator=(const SecurityCam& other);
    ~SecurityCam() override;

    int getResolution() const;
    bool isRecording() const;
    bool setResolution(int resolution);
    bool setRecording(bool recording);

    double powerUsage() const override;
    void status(std::ostream& out) const override;
    int settingCount() const override;
    const char* settingName(int index) const override;
    bool applySetting(int index, int value) override;
    Device* clone() const override;
};

#endif
