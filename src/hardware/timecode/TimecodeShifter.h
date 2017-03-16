#include <inttypes.h>
#include <muxer/IoDemuxer595.hpp>

static const uint8_t DP_START_INDEX = io::Ic595::NUM_CHANNELS;
static const uint8_t ENABLE_START_INDEX = DP_START_INDEX * 2;

static const uint8_t LED_CHAR_LEN = 4;
typedef uint8_t ledChar[LED_CHAR_LEN];

static const uint8_t CHAR_MAP_LEN = 20;
ledChar CHAR_MAP[CHAR_MAP_LEN] = {
    {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 1, 0, 0},
    {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1}, {1, 0, 0, 0}, {1, 0, 0, 1},
    // these maps to digits and dots
    {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 1, 0, 0},
    {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1}, {1, 0, 0, 0}, {1, 0, 0, 1}
};
