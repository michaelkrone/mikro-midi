#pragma once

#include "IoNamespace.h"
#include "IoDefs.h"

BEGIN_IO_NAMESPACE

static const uint16_t DefaultFaderMax = 1023;
static const uint16_t DefaultFaderMin = 0;

// good working PID modifiers
static const float DefaultGainP = 222.00;
static const float DefaultGainI = 0.9;
static const float DefaultGainD = 5.00;

END_IO_NAMESPACE
