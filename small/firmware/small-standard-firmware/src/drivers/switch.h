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
    uint16_t m_longPressMs;
    uint32_t m_stateMs;
    bool m_longPressActive;

    bool m_isPressed;
    bool m_isLongPressed;

  public:
    Switch(DigitalGpio& t_gpio, uint16_t t_debounceMs = 20, uint16_t t_longPressMs = 500) :
      m_gpio(t_gpio) {}

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

          m_isPressed = false;
          m_isLongPressed = false;
          break;

        case SwitchState::kDebouncingDown:
          if (! switchEvent) {
            m_state = SwitchState::kIdle;
            m_stateMs = now;
          }
          else if (elapsed >= m_debounceMs) {
            m_state = SwitchState::kPressed;
            m_stateMs = now;
            m_longPressActive = false;
          }
          break;

        case SwitchState::kPressed:
          if (switchEvent && elapsed >= m_longPressMs) {
            m_longPressActive = true;
          }
          else if (! switchEvent) {
            m_state = SwitchState::kDebouncingUp;
            m_stateMs = now;
          }
          break;

        case SwitchState::kDebouncingUp:
          if (switchEvent) {
            m_state = SwitchState::kPressed;
            m_stateMs = now;
            m_longPressActive = false;
          }
          else if (elapsed >= m_debounceMs) {
            if (m_longPressActive) {
              m_isLongPressed = true;
            }
            else {
              m_isPressed = true;
            }

            m_state = SwitchState::kIdle;
            m_stateMs = now;
          }
          break;

        default:
          break;
      }
    }

    bool getIsPressed() {
      return m_isPressed;
    }
};

}
