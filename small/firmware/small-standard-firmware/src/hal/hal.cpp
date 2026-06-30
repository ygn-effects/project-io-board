#include "hal/hal.h"

namespace hal {

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
