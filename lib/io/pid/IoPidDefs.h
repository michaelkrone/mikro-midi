#pragma once

#include "IoNamespace.h"
#include "IoDefs.h"

BEGIN_IO_NAMESPACE

static const uint16_t DefaultAnalogMax = 4096;
static const uint16_t DefaultFaderMax = 1023;
static const uint16_t DefaultFaderMin = 0;

// good working PID modifiers
static const float DefaultGainP = 5.5;
static const float DefaultGainI = 0.3;
static const float DefaultGainD = 0.00001;

END_IO_NAMESPACE
