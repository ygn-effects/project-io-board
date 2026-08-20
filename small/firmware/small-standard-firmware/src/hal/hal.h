#pragma once

#include "drivers/bypass.h"
#include "drivers/gpio.h"
#include "drivers/switch.h"

namespace hal {
  namespace pinsDefinitions {
    constexpr uint8_t c_startupStatePin = 4;
    constexpr uint8_t c_footSwitchPin = 0;
    constexpr uint8_t c_ledPin = 3;
    constexpr uint8_t c_relayPin = 1;
    constexpr uint8_t c_optocouplerPin = 2;
  };

extern DigitalGpioDriver startupStateGpio;
extern DigitalGpioDriver footSwitchGpio;
extern DigitalGpioDriver ledGpio;
extern DigitalGpioDriver relayGpio;
extern DigitalGpioDriver optocouplerGpio;

extern Bypass bypass;
extern Switch footSwitch;

void init();
void update();

}
