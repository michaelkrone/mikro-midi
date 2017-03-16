
#include <MackieDefs.h>
#include <muxer/IoDemuxer595.hpp>
#include <IoOutput.hpp>
#include <IoBouncedInput.hpp>

#include "Pins.h"

struct CommandOutputMap {
  uint8_t cmd;
  io::ioPin pin;
};

template <uint8_t N>
struct Commands {
  static const uint8_t length = N;
  io::Demuxer595 output;
  CommandOutputMap commands[N];
};

// struct CommandInput {
//   uint8_t cmd;
//   io::BouncedInput input;
// };
// typedef CommandInput CommandInput;
