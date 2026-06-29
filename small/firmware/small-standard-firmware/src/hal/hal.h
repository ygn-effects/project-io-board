#pragma once

#include "drivers/bypass.h"
#include "drivers/gpio.h"
#include "drivers/switch.h"

namespace hal {
  namespace pinsDefinitions {
    constexpr uint8_t c_startupStatePin = 4;
    constexpr uint8_t c_footSwitchPin = 3;
    constexpr uint8_t c_ledPin = 0;
    constexpr uint8_t c_relayPin = 2;
    constexpr uint8_t c_optocouplerPin = 1;
  };

DigitalGpioDriver startupStateGpio(pinsDefinitions::c_startupStatePin, GpioConfig::kInputPullup);
DigitalGpioDriver footSwitchGpio(pinsDefinitions::c_footSwitchPin, GpioConfig::kInputPullup);
DigitalGpioDriver ledGpio(pinsDefinitions::c_ledPin, GpioConfig::kOutput);
DigitalGpioDriver relayGpio(pinsDefinitions::c_relayPin, GpioConfig::kOutput);
DigitalGpioDriver optocouplerGpio(pinsDefinitions::c_optocouplerPin, GpioConfig::kOutput);

Bypass bypass(relayGpio, optocouplerGpio, ledGpio, startupStateGpio);
Switch footSwitch(footSwitchGpio);

void init() {
  bypass.init();
  footSwitch.init();
}

void update() {
  footSwitch.poll();

  if (footSwitch.getIsPressed()) {
    bypass.toggle();
  }
}

}
