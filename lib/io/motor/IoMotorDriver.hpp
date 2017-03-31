#pragma once

#include "IoDefs.h"
#include "ic/IoIc.hpp"

BEGIN_IO_NAMESPACE

class MotorDriver : public Ic {
  protected:
    int mSpeed;

  public:
    MotorDriver() {}

    virtual ~MotorDriver() {}

    void setSpeed(int speed) {
      mSpeed = speed;
    }

    virtual void enable(uint8_t index);
    virtual void disable(uint8_t index);
    virtual void setEnabled(uint8_t index, bool enabled);
    virtual bool isEnabled(uint8_t index);

    virtual void up(uint8_t index);
    virtual void up(uint8_t index, int speed);

    virtual void down(uint8_t index);
    virtual void down(uint8_t index, int speed);

    virtual void stop(uint8_t index);
    virtual void halt(uint8_t index);
    virtual void stopAll();
    virtual void haltAll();
  };

END_IO_NAMESPACE
