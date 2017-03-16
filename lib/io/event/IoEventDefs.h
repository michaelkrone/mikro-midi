#pragma once

#include "IoNamespace.h"

BEGIN_IO_NAMESPACE

static const uint8_t MaxSignalEventListeners = 3;
static const uint8_t MaxAdditionalEventListenerArgs = 2;

static const uint8_t SignalEventTypeLength = 5;
typedef enum {EventUninitialised, EventUp, EventDown, EventHold, EventHoldRepeat, EventCatchAll} SignalEventType;

typedef void (*SignalChangeHandler)(int, SignalEventType);

END_IO_NAMESPACE
