#pragma once

#include <stdint.h>

class Pollable {
  public:
    virtual void init() = 0;
    virtual void poll() = 0;
};
