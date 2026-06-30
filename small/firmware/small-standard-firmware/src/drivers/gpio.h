#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "periphs/gpio.h"

namespace hal {

enum class GpioConfig : uint8_t {
  kInput,
  kInputPullup,
  kOutput
};

class DigitalGpioDriver : public DigitalGpio {
  private:
      uint8_t m_pin;
      GpioConfig m_config;

  public:
    DigitalGpioDriver(uint8_t t_pin, GpioConfig t_config) :
      m_pin(t_pin),
      m_config(t_config) {}

    void init() override {
      switch (m_config) {
        case GpioConfig::kInput:
          pinMode(m_pin, INPUT);
          break;

        case GpioConfig::kInputPullup:
          pinMode(m_pin, INPUT_PULLUP);
          break;

        case GpioConfig::kOutput:
          pinMode(m_pin, OUTPUT);
          break;

        default:
          break;
      }
    }

    bool read() const override {
      return digitalRead(m_pin);
    }

    void write(bool t_value) override {
      digitalWrite(m_pin, t_value);
    }
};

}