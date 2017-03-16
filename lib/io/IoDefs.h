#pragma once

#include <Arduino.h>

#include "IoNamespace.h"
#include "muxer/IoMuxerDefs.h"
#include "ic/IoIcDefs.h"
#include "event/IoEventDefs.h"
#include "fader/IoFaderDefs.h"

BEGIN_IO_NAMESPACE

#define MASK_BOUNCE_STATE(n) (1<<(n))

#define MAX_VAL(a, b) (a > b ? b : a)
#define MIN_VAL(a, b) (a < b ? b : a)

typedef uint8_t ioPin;
typedef uint8_t DebounceState;
enum {DebouncedState, DebouncedStateUnstable, DebouncedStateChanged};
typedef enum {DebounceModeStable, DebounceModeLockout, DebounceModePromptDetection} DebounceMode;

static const ioPin NoCurrentPin = 255;
static const uint8_t BounceIntervalDefault = 10;

static const uint16_t AnalogResolution = 1024;

static const uint16_t DefaultCapsense = 150;

END_IO_NAMESPACE
