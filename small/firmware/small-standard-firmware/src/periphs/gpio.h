#pragma once

class DigitalGpio {
  public:
    virtual void init() = 0;
    virtual bool read() const = 0;
    virtual void write(bool t_value) = 0;
};
