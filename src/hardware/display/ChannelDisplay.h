#include <inttypes.h>
#include <display/IoDisplayHTDSDI96.hpp>

#define MAX(a, b) (a > b ? b : a)

static const uint8_t DISPLAY_PADDING = 0;

static const uint8_t BORDER_WIDTH = 1;
static const uint16_t DISPLAY_CENTER_X = SSD1306_WIDTH / 2;

static const uint8_t SCALE_HEIGHT = 12;
static const uint16_t SCALE_WIDTH = SSD1306_WIDTH - (BORDER_WIDTH * 2);

static const uint8_t VPOT_BAR_WIDTH = 11;
static const uint16_t VPOT_SEGMENT_LENGTH = SCALE_WIDTH / mackie::VPotRingLength;
