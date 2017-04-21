#pragma once

#include <Arduino.h>

#include "IoNamespace.h"
#include "input/IoInputDefs.h"
#include "ic/IoIcDefs.h"
#include "muxer/IoMuxerDefs.h"
#include "event/IoEventDefs.h"
#include "pid/IoPidDefs.h"
#include "rotary/RotaryDefs.h"

BEGIN_IO_NAMESPACE

#ifndef ANALOG_MAX
#define ANALOG_MAX 255
#endif

#define MAX_VAL(a, b) (a > b ? b : a)
#define MIN_VAL(a, b) (a < b ? b : a)

typedef uint8_t ioPin;

static const ioPin NoCurrentPin = 255;
static const uint16_t AnalogResolution = 1024;

END_IO_NAMESPACE
