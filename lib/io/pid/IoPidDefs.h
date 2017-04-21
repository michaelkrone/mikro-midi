#pragma once

#include "IoNamespace.h"
#include "IoDefs.h"

BEGIN_IO_NAMESPACE

static const uint16_t DefaultFaderMax = 1023;
static const uint16_t DefaultFaderMin = 0;

// good working PID modifiers

// 5V
// static const float DefaultGainP = 222.00;
// static const float DefaultGainI = 0.9;
// static const float DefaultGainD = 5.00;

// 9V
// static const float DefaultGainP = 145.00;
// static const float DefaultGainI = 0.09;
// static const float DefaultGainD = 5.00;

static const float DefaultGainP = 5.00;
static const float DefaultGainI = 1.00;
static const float DefaultGainD = 5.00;

END_IO_NAMESPACE
