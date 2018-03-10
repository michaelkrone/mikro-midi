#pragma once

// #include <avr/pgmspace.h>
#include <inttypes.h>

#include "MackieNamespace.h"
#include "MackieVPotDisplay.h"

BEGIN_MACKIE_NAMESPACE

// internal used constants

static const uint8_t CHANNEL_NB = 8;
static const uint8_t FADER_NB = 8;

static const uint8_t SYSEX_BUF_SIZE = 128;

static const uint8_t CONTROL_CHANNEL = 1;

static const int16_t FADER_SEND_MIN = -8192;
static const int16_t FADER_SEND_MAX = 8176;
static const int16_t FADER_MIN = 130;
static const int16_t FADER_MAX = 1023;

static const uint8_t PITCHBEND_MIN = 0;
static const uint16_t PITCHBEND_MAX = 16383;

static const uint8_t MIDI_SOX = 0xF0;
static const uint8_t MIDI_EOX =	0xF7;

static const uint8_t FW_VERSION_INDEX =	0;
static const uint8_t FW_SUB_VERSION_INDEX =1;
static const uint8_t HW_VERSION_INDEX =	2;
static const uint8_t HW_SUB_VERSION_INDEX =3;
static const uint8_t MANUF_YEAR_INDEX =	4;
static const uint8_t MANUF_MONTH_INDEX =	5;
static const uint8_t SERIAL_4_INDEX =		6;
static const uint8_t SERIAL_3_INDEX =		7;
static const uint8_t SERIAL_2_INDEX =		8;
static const uint8_t SERIAL_1_INDEX =		9;
static const uint8_t SERIAL_0_INDEX =		10;
static const uint8_t IDENTITY_LENGHT =		11;

static const unsigned char IDENTITY[IDENTITY_LENGHT] = {
	'1', '6', '1', '7', '7', '1', '1', '2', '3', '4', '5'
};

static const uint8_t SERIAL_NB_LENGHT = 7; // SS SS SS SS SS SS SS
static const uint8_t FW_VERSION_LENGHT = 5; // VV VV VV VV VV
static const uint8_t CHALLENGE_LENGHT = 4; // LL LL LL LL

static const uint8_t TX_HOST_CONNECT_QUERRY = 0x01; // + SS SS SS SS SS SS SS LL LL LL LL F7 (SS=serial number LL=chalenge code)
static const uint8_t TX_CONFIRM_CONNECT = 0x03; // + SS SS SS SS SS SS SS F7 (SS=serial number)
static const uint8_t TX_ERROR_CONNECT = 0x04; // + SS SS SS SS SS SS SS F7 (SS=serial number)
static const uint8_t TX_FW_VERSION_REPLY = 0x14; // + VV VV VV VV VV F7 (VV=firmware version eg "1.0")
static const uint8_t TX_SERIAL_NB_REPLY_MACKIE = 0x1B; // + SS SS SS SS SS SS SS F7 (SS=serial number) (Mackie only)

static const uint8_t RX_DEVICE_QUERY = 0x00; // + F7
static const uint8_t RX_OFFLINE_STATUS_LOGIC = 0x0F; // + 7F F7 (Logic only, not Mackie) V1.69
static const uint8_t RX_HOST_CONNECT_REPLY = 0x02; // + SS SS SS SS SS SS SS RR RR RR RR F7 (SS=serial number RR=response to chalenge code)
static const uint8_t RX_FW_VERSION_REQUEST = 0x13; // + 00 F7
static const uint8_t RX_SERIAL_NB_REQUEST_MACKIE = 0x1A; // + 00 F7 (Mackie only, not logic) V1.69

static const uint8_t RX_RESET_FADERS = 0x61; // + F7
static const uint8_t RX_RESET_LEDS = 0x62; // + F7
static const uint8_t RX_RESET_UNIT = 0x63; // + F7

// algorithm (l1 to l4 = challenge code bytes 1 to 4, r1 to r4 = response code bytes 1 to 4):
// r1 = 0x7F & (l1 + (l2 ^ 0xa) - l4);
// r2 = 0x7F & ((l3>>4) ^ (l1+l4));
// r3 = 0x7F & ( l4-(l3<<2) ^ (l1|l2));
// r4 = 0x7F & (l2-l3+(0xF0^(l4<<4)));
static const uint8_t RX_WRITE_LCD_TXT = 0x12;	// + OO YY ... F7 (OO=offset 00 thru 37 for upper line, 38 thru 6F for lower line, YY=ASCII data)
												// 6 bytes per channel are used to display texts over each channel
static const uint8_t RX_WRITE_MULTIPLE_MTC = 0x10;	// + YY ... F7 (YY=ASCII data from ASCII table, including dots)
												// eg F0 00 00 66 10 10 36 32 31 71 30 72 30 79 30 31 F7 writes "109.02.01.126"
static const uint8_t RX_WRITE_SINGLE_MTC = 0x40;	// B0 4i YY ... F7 (YY=ASCII data from ASCII table, i=digit id)
static const uint8_t RX_WRITE_MULTIPLE_7SEG = 0x11;	// + YY YY F7 (YY=ASCII data from ASCII table, including dots)
static const uint8_t RX_WRITE_VPOT_RING = 0x30;

static const uint8_t LCD_LINE_LENGHT = 56; // V1.7
static const uint8_t LCD_LINE_NB = 2;
static const uint8_t LCD_CHAR_NB = LCD_LINE_LENGHT * LCD_LINE_NB; // V1.7

//enum {MACKIE_MTC_HOUR, MTC_MIN, MTC_SEC, MTC_MILSEC, MTC_NB};
static const uint8_t MTC_UPDATE_ALL_DIGITS = 0xFF;
static const uint8_t MTC_DIGIT_NB = 10;		// 000.00.00.000
static const uint8_t SEG_DIGIT_NB = 2;		// 00

static const uint8_t VPOT_RING_NB = 11;

// Messages sent to the host which are not available in the library
static const uint8_t SYX_NO_MODE = 0x00;
static const uint8_t SYX_ID0 = 0x00;
static const uint8_t SYX_ID1 = 0x00;
static const uint8_t SYX_ID2 = 0x66;
static const uint8_t SYX_ID3 = 0x14; // V1.68 0x10 (mackie control is 14 while logic control is 10)
static const uint8_t SYX_ID3_XT = 0x15; // V1.68 0x11 (mackie control xt is 15 while logic control xt is 11)
static const uint8_t SYX_ID3_LOGIC = 0x10; // V1.68
static const uint8_t SYX_ID3_LOGIC_XT = 0x11; // V1.68
static const uint8_t SYX_HEADER_SIZE = 4;	// F0 00 00 66  V1.69
static const uint8_t SYSEX_HEADER[SYX_HEADER_SIZE] = {MIDI_SOX, SYX_ID0, SYX_ID1, SYX_ID2/* V1.69, SYX_ID3 */ };

///////////////////////////////////////////////////////////
// MACKIE CONTROL IDs
//////////////////////////////////////////////////////////

static const uint8_t FADER_BNK_LEFT_ID = 0x2E;
static const uint8_t FADER_BNK_RIGHT_ID = 0x2F;
static const uint8_t FADER_CHANNEL_LEFT_ID = 0x30;	// V1.7
static const uint8_t FADER_CHANNEL_RIGHT_ID = 0x31;	// V1.7

static const uint8_t SHIFT_ID = 0x46;
static const uint8_t JOG_WHEEL_ID = 0x3C;

static const uint8_t JOG_WHEEL_LEFT = 0x41;
static const uint8_t JOG_WHEEL_RIGHT = 0x01;

const static uint8_t LCD_CHANNEL_DATA_LEN = 20;
const static uint8_t LCD_ADDRESS_LEN = 4;
typedef uint8_t lcdAddress[LCD_ADDRESS_LEN];
static const lcdAddress LCD_INDEX_MAP[CHANNEL_NB] = {
  {0, 7, 56, 63}, {0, 9, 56, 64}, {0, 9, 56, 64}, {0, 9, 56, 64},
  {0, 9, 56, 64}, {0, 9, 56, 64}, {0, 9, 56, 64}, {0, 9, 56, 64}
};
// static const lcdAddress LCD_INDEX_MAP[CHANNEL_NB] = {
//   {0, 56}, {0, 56}, {0, 56}, {0, 56},
//   {0, 56}, {0, 56}, {0, 56}, {0, 56}
// };

// "public" usable constants

static const uint8_t Off = 0;
static const uint8_t On = 127;

enum OperationModes {
	OperationMode_MackieControl = SYX_ID3,
	OperationMode_MackieControlXt = SYX_ID3_XT,
	OperationMode_LogicControl = SYX_ID3_LOGIC,
	OperationMode_LogicControlXt = SYX_ID3_LOGIC_XT
};

enum Commands {
	Channel0_Button_Rec = 0,
	Channel1_Button_Rec = 1,
	Channel2_Button_Rec = 2,
	Channel3_Button_Rec = 3,
	Channel4_Button_Rec = 4,
	Channel5_Button_Rec = 5,
	Channel6_Button_Rec = 6,
	Channel7_Button_Rec = 7,

	Channel0_Button_Solo = 8,
	Channel1_Button_Solo = 9,
	Channel2_Button_Solo = 10,
	Channel3_Button_Solo = 11,
	Channel4_Button_Solo = 12,
	Channel5_Button_Solo = 13,
	Channel6_Button_Solo = 14,
	Channel7_Button_Solo = 15,

	Channel0_Button_Mute = 16,
	Channel1_Button_Mute = 17,
	Channel2_Button_Mute = 18,
	Channel3_Button_Mute = 19,
	Channel4_Button_Mute = 20,
	Channel5_Button_Mute = 21,
	Channel6_Button_Mute = 22,
	Channel7_Button_Mute = 23,

	Channel0_Button_Select = 24,
	Channel1_Button_Select = 25,
	Channel2_Button_Select = 26,
	Channel3_Button_Select = 27,
	Channel4_Button_Select = 28,
	Channel5_Button_Select = 29,
	Channel6_Button_Select = 30,
	Channel7_Button_Select = 31,

	Channel0_Vpot_Switch = 32,
	Channel1_Vpot_Switch = 33,
	Channel2_Vpot_Switch = 34,
	Channel3_Vpot_Switch = 35,
	Channel4_Vpot_Switch = 36,
	Channel5_Vpot_Switch = 37,
	Channel6_Vpot_Switch = 38,
	Channel7_Vpot_Switch = 39,

	VpotAssign_Track = 40,
	VpotAssign_Send = 41,
	VpotAssign_Pan = 42,
	VpotAssign_PlugIn = 43,
	VpotAssign_Eq = 44,
	VpotAssign_Instrument = 45,

	General_Fader_BankUp = 46,
	General_Fader_BankDown = 47,
	General_Channel_Up = 48,
	General_Channel_Down = 49,
	General_Flip = 50,
	General_Edit = 51,

	Display_ToggleVuMeter = 52,
	Display_ToggleTempoOrTicks = 53,

	Function_F1 = 54,
	Function_F2 = 55,
	Function_F3 = 56,
	Function_F4 = 57,
	Function_F5 = 58,
	Function_F6 = 59,
	Function_F7 = 60,
	Function_F8 = 61,
	Function_F9 = 62,
	Function_F10 = 63,
	Function_F11 = 64,
	Function_F12 = 65,
	Function_F13 = 66,
	Function_F14 = 67,
	Function_F15 = 68,
	Function_F16 = 69,

	Modifier_Shift = 70,
	Modifier_Option = 71,
	Modifier_Control = 72,
	Modifier_Alt = 73,

	Automation_ReadOff = 74,
	Automation_Touch = 75,
	Automation_Trim = 76,
	Automation_Overwrite = 77,
	Automation_Latch = 78,
	Automation_TrimLatch = 79,

	Edit_Save = 80,
	Edit_AudibleMode = 81,
	Edit_Undo = 82,
	Edit_Redo = 83,

	Daw_RTZ = 84,
	Daw_Marker = 85,
	Daw_EditGrid = 86,
	Daw_Cycle = 87,
	Daw_Punch = 88,
	Daw_Select = 89,

	Transport_Solo = 90,
	Transport_Rewind = 91,
	Transport_FastForward = 92,
	Transport_Stop = 93,
	Transport_Play = 94,
	Transport_Record = 95,

	Navigation_Up = 96,
	Navigation_Down = 97,
	Navigation_Scrub = 98,
	Navigation_Zoom = 99,

	Navigation_Left = 100,
	Navigation_Right = 101,

	// Events, not considered in the command state

	Channel0_FaderTouch = 104,
	Channel1_FaderTouch = 105,
	Channel2_FaderTouch = 106,
	Channel3_FaderTouch = 107,
	Channel4_FaderTouch = 108,
	Channel5_FaderTouch = 109,
	Channel6_FaderTouch = 110,
	Channel7_FaderTouch = 111,

	JogWheelMove = 160,
	Channel_VpotMove = 170, // starts with control change 16 up to 23
	Channel_FaderMove = 180,
};

// do not consider Events
static const uint8_t COMMANDS_NB = 101;

enum States {
	Error,
	Command,
	Lcd,
	VPotRings,
	Faders,
	VuMeters,
	Timecode,
  SegmentLcd
};
static const uint8_t STATES_NB = 8;

enum State {
	Unchanged,
	Changed
};

enum Errors {
	Ok,
	Offline
};
static const uint8_t ERRORS_NB = 2;

#define MIN_VAL(a, b) (a > b ? b : a)

#define TO_SYSBYTE(b) (b + '0')

#define TO_PITCHBEND_SCALE(value) ((float) (value - FADER_MIN) / (FADER_MAX - FADER_MIN)) * PITCHBEND_MAX

#define FROM_PITCHBEND_SCALE(x) MIN_VAL(\
	((float) (((mackie::FADER_MAX - mackie::FADER_MIN) * (x - mackie::PITCHBEND_MIN))\
    / (mackie::PITCHBEND_MAX - mackie::PITCHBEND_MIN)) + mackie::FADER_MIN)\
	, 1023)

#define DISPLAY_TIMECODE_DOT(index) (index >= 10 && index <= 19)

END_MACKIE_NAMESPACE
