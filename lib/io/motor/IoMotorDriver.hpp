#pragma once

#include "IoDefs.h"
#include "ic/IoIc.hpp"

BEGIN_IO_NAMESPACE

class MotorDriver : public Ic {
  protected:

  public:
    MotorDriver() {}

    virtual ~MotorDriver() {}

    virtual void enable();
    virtual void disable();

    virtual void up(int speed);
    virtual void down(int speed);

    virtual void stop();
    virtual void halt();
  };

END_IO_NAMESPACE
