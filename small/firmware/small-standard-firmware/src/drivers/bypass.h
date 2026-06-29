#pragma once

#include <Arduino.h>
#include "periphs/gpio.h"
#include "periphs/toggleable.h"

namespace hal {

class Bypass : public Toggleable {
  private:
    DigitalGpio& m_relay;
    DigitalGpio& m_optoCoupler;
    DigitalGpio& m_led;
    DigitalGpio& m_startupState;
    bool m_state;

    void setState(bool t_state) {
      m_optoCoupler.write(1);
      delay(10);
      m_relay.write(t_state);
      m_led.write(t_state);
      delay(10);
      m_optoCoupler.write(0);
    }

  public:
    Bypass(DigitalGpio& t_relay, DigitalGpio& t_opto, DigitalGpio& t_led, DigitalGpio& t_startup) :
      m_relay(t_relay),
      m_optoCoupler(t_opto),
      m_led(t_led),
      m_startupState(t_startup),
      m_state(false) {}

    void init() override {
      m_startupState.init();
      m_relay.init();
      m_optoCoupler.init();
      m_led.init();
      m_state = ! m_startupState.read();
      setState(m_state);
    }

    void on() override {
      m_state = true;
      setState(m_state);
    }

    void off() override {
      m_state = false;
      setState(m_state);
    }

    void toggle() override {
      m_state = !m_state;
      setState(m_state);
    }
};

}
