#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "periphs/gpio.h"
#include "periphs/pollable.h"

namespace hal {

enum class SwitchState : uint8_t {
  kIdle,
  kDebouncingDown,
  kPressed,
  kDebouncingUp,
};

class Switch : public Pollable {
  private:
    DigitalGpio& m_gpio;
    SwitchState m_state;
    uint16_t m_debounceMs;
    uint32_t m_stateMs;
    bool m_isPressed;

  public:
    Switch(DigitalGpio& t_gpio, uint16_t t_debounceMs = 20) :
      m_gpio(t_gpio),
      m_state(SwitchState::kIdle),
      m_debounceMs(t_debounceMs),
      m_stateMs(0),
      m_isPressed(false) {}

    void init() override {
      m_gpio.init();
    }

    void poll() override {
      bool switchEvent = !m_gpio.read();

      uint32_t now = millis();
      uint32_t elapsed = now - m_stateMs;

      switch (m_state) {
        case SwitchState::kIdle:
          if (switchEvent) {
            m_state = SwitchState::kDebouncingDown;
            m_stateMs = now;
          }
          break;

        case SwitchState::kDebouncingDown:
          if (! switchEvent) {
            m_state = SwitchState::kIdle;
            m_stateMs = now;
          }
          else if (elapsed >= m_debounceMs) {
            m_state = SwitchState::kPressed;
            m_stateMs = now;
            m_isPressed = true;
          }
          break;

        case SwitchState::kPressed:
          if (! switchEvent) {
            m_state = SwitchState::kDebouncingUp;
            m_stateMs = now;
          }
          break;

        case SwitchState::kDebouncingUp:
          if (switchEvent) {
            m_state = SwitchState::kPressed;
            m_stateMs = now;
          }
          else if (elapsed >= m_debounceMs) {
            m_state = SwitchState::kIdle;
            m_stateMs = now;
          }
          break;

        default:
          break;
      }
    }

    bool getIsPressed() {
      bool pressed = m_isPressed;
      m_isPressed = false;
      return pressed;
    }
};

}
