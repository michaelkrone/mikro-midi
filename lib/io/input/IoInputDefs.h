#pragma once

#include <inttypes.h>

#include "IoNamespace.h"

BEGIN_IO_NAMESPACE

#define MASK_BOUNCE_STATE(n) (1<<(n))

static const uint16_t DefaultCapsense = 150;
static const uint8_t BounceIntervalDefault = 10;

typedef uint8_t DebounceState;

enum {DebouncedState, DebouncedStateUnstable, DebouncedStateChanged};

typedef enum {DebounceModeStable, DebounceModeLockout, DebounceModePromptDetection} DebounceMode;

END_IO_NAMESPACE
