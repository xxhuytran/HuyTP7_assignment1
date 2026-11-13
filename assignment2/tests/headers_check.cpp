#include "Device.h"
#include "Light.h"
#include "Thermostat.h"
#include "SecurityCam.h"
#include "Room.h"
#include "ConsoleApp.h"
#include <type_traits>

static_assert(std::is_abstract<Device>::value, "Device must be abstract");
static_assert(std::has_virtual_destructor<Device>::value, "Device destructor must be virtual");
static_assert(std::is_base_of<Device, Light>::value, "Light must derive from Device");
static_assert(std::is_base_of<Device, Thermostat>::value, "Thermostat must derive from Device");
static_assert(std::is_base_of<Device, SecurityCam>::value, "SecurityCam must derive from Device");
