#pragma once

#include "IoDefs.h"

BEGIN_IO_NAMESPACE

// muxer related

// convert read digital signal for pullup mode
#define DIGITAL_READ_PULLUP(read, pullup) (read ? !pullup : pullup)

// minimum time to wait for a stable channel select, for a demuxer
// this will write the current output on the selected pin for the
// configured time, might only be used for muxers, multiply with channels to
// calculate the total delay
static const uint32_t DefaultBounceDelayMicros_4051 = 0;

// minimum time to write a signal, multiply with channels to
// calculate the total delay, might be higher for analog demuxers
static const uint32_t DefaultDemuxDelayMicros_4051 = 100;

END_IO_NAMESPACE
