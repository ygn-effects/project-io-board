#include <Arduino.h>
#include "hal/hal.h"

void setup() {
  hal::init();
}

void loop() {
  hal::update();
}
