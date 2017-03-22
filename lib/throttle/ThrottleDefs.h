#pragma once

#include <Arduino.h>

#include "ThrottleNamespace.h"

BEGIN_THROTTLE_NAMESPACE

static const uint8_t MAX_THROTTLES = 8;
static const unsigned long DEFAULT_THRESHOLD = 5;

struct ThrottleItem {
  unsigned long lastReset;
  uint32_t threshold;
};

typedef ThrottleItem ThrottleItem;

END_THROTTLE_NAMESPACE
