#pragma once

#include <inttypes.h>


// ic's

// 595 demuxers

static const uint8_t IC_595_1_SHCP = 28;
static const uint8_t IC_595_1_STCP = 27;
static const uint8_t IC_595_1_DS = 26;

// 4051 muxers

static const uint8_t IC_4051_1_S0 = 31;
static const uint8_t IC_4051_1_S1 = 32;
static const uint8_t IC_4051_1_S2 = 33;

static const uint8_t IC_4051_1_MUX = 14;
static const uint8_t IC_4051_2_MUX = 15;

// displays

static const uint8_t DISPLAY_CHANNEL1_SDA_PIN = 30;
static const uint8_t DISPLAY_CHANNEL1_SCL_PIN = 29;

// command button and led pins

// channel 0

// channel 0 - leds - ledDemuxer1
static const uint8_t PinLed_Channel0_Button_Rec = 0;
static const uint8_t PinLed_Channel0_Button_Solo = 1;
static const uint8_t PinLed_Channel0_Button_Mute = 2;
static const uint8_t PinLed_Channel0_Button_Select = 3;

// channel 0 - buttons - buttonMuxer1
static const uint8_t PinButton_Channel0_Button_Rec = 0;
static const uint8_t PinButton_Channel0_Button_Solo = 1;
static const uint8_t PinButton_Channel0_Button_Mute = 2;
static const uint8_t PinButton_Channel0_Button_Select = 3;
static const uint8_t PinButton_Channel0_Vpot_Switch = 4;

// channel 1 - leds - ledDemuxer1
static const uint8_t PinLed_Channel1_Button_Rec = 4;
static const uint8_t PinLed_Channel1_Button_Solo = 5;
static const uint8_t PinLed_Channel1_Button_Mute = 6;
static const uint8_t PinLed_Channel1_Button_Select = 7;

// channel 1 - buttons - demuxer2
static const uint8_t PinButton_Channel1_Button_Rec = 1;
static const uint8_t PinButton_Channel1_Button_Solo = 9;
static const uint8_t PinButton_Channel1_Button_Mute = 17;
static const uint8_t PinButton_Channel1_Button_Select = 25;
static const uint8_t PinButton_Channel1_Vpot_Switch = 33;

// channel 2
static const uint8_t PinButton_Channel2_Button_Rec = 2;
static const uint8_t PinButton_Channel3_Button_Rec = 3;
static const uint8_t PinButton_Channel4_Button_Rec = 4;
static const uint8_t PinButton_Channel5_Button_Rec = 5;
static const uint8_t PinButton_Channel6_Button_Rec = 6;
static const uint8_t PinButton_Channel7_Button_Rec = 7;

static const uint8_t PinLed_Channel2_Button_Rec = 2;
static const uint8_t PinLed_Channel3_Button_Rec = 3;
static const uint8_t PinLed_Channel4_Button_Rec = 4;
static const uint8_t PinLed_Channel5_Button_Rec = 5;
static const uint8_t PinLed_Channel6_Button_Rec = 6;
static const uint8_t PinLed_Channel7_Button_Rec = 7;

//
static const uint8_t PinButton_Channel2_Button_Solo = 10;
static const uint8_t PinButton_Channel3_Button_Solo = 11;
static const uint8_t PinButton_Channel4_Button_Solo = 12;
static const uint8_t PinButton_Channel5_Button_Solo = 13;
static const uint8_t PinButton_Channel6_Button_Solo = 14;
static const uint8_t PinButton_Channel7_Button_Solo = 15;

static const uint8_t PinLed_Channel2_Button_Solo = 10;
static const uint8_t PinLed_Channel3_Button_Solo = 11;
static const uint8_t PinLed_Channel4_Button_Solo = 12;
static const uint8_t PinLed_Channel5_Button_Solo = 13;
static const uint8_t PinLed_Channel6_Button_Solo = 14;
static const uint8_t PinLed_Channel7_Button_Solo = 15;

//
static const uint8_t PinButton_Channel2_Button_Mute = 18;
static const uint8_t PinButton_Channel3_Button_Mute = 19;
static const uint8_t PinButton_Channel4_Button_Mute = 20;
static const uint8_t PinButton_Channel5_Button_Mute = 21;
static const uint8_t PinButton_Channel6_Button_Mute = 22;
static const uint8_t PinButton_Channel7_Button_Mute = 23;

static const uint8_t PinLed_Channel2_Button_Mute = 18;
static const uint8_t PinLed_Channel3_Button_Mute = 19;
static const uint8_t PinLed_Channel4_Button_Mute = 20;
static const uint8_t PinLed_Channel5_Button_Mute = 21;
static const uint8_t PinLed_Channel6_Button_Mute = 22;
static const uint8_t PinLed_Channel7_Button_Mute = 23;
//
static const uint8_t PinButton_Channel2_Button_Select = 26;
static const uint8_t PinButton_Channel3_Button_Select = 27;
static const uint8_t PinButton_Channel4_Button_Select = 28;
static const uint8_t PinButton_Channel5_Button_Select = 29;
static const uint8_t PinButton_Channel6_Button_Select = 30;
static const uint8_t PinButton_Channel7_Button_Select = 31;

static const uint8_t PinLed_Channel2_Button_Select = 26;
static const uint8_t PinLed_Channel3_Button_Select = 27;
static const uint8_t PinLed_Channel4_Button_Select = 28;
static const uint8_t PinLed_Channel5_Button_Select = 29;
static const uint8_t PinLed_Channel6_Button_Select = 30;
static const uint8_t PinLed_Channel7_Button_Select = 31;

//
static const uint8_t PinButton_Channel2_Vpot_Switch = 34;
static const uint8_t PinButton_Channel3_Vpot_Switch = 35;
static const uint8_t PinButton_Channel4_Vpot_Switch = 36;
static const uint8_t PinButton_Channel5_Vpot_Switch = 37;
static const uint8_t PinButton_Channel6_Vpot_Switch = 38;
static const uint8_t PinButton_Channel7_Vpot_Switch = 39;

//
static const uint8_t PinButton_VpotAssign_Track = 40;
static const uint8_t PinButton_VpotAssign_Send = 41;
static const uint8_t PinButton_VpotAssign_Pan = 42;
static const uint8_t PinButton_VpotAssign_PlugIn = 43;
static const uint8_t PinButton_VpotAssign_Eq = 44;
static const uint8_t PinButton_VpotAssign_Instrument = 45;

//
static const uint8_t PinButton_General_Fader_BankUp = 46;
static const uint8_t PinButton_General_Fader_BankDown = 47;
static const uint8_t PinButton_General_Channel_Up = 48;
static const uint8_t PinButton_General_Channel_Down = 49;
static const uint8_t PinButton_General_Flip = 50;
static const uint8_t PinButton_General_Edit = 51;

//
static const uint8_t PinButton_Display_ToggleVuMeter = 52;
static const uint8_t PinButton_Display_ToggleTempoOrTicks = 53;

//
static const uint8_t PinButton_Function_F1 = 54;
static const uint8_t PinButton_Function_F2 = 55;
static const uint8_t PinButton_Function_F3 = 56;
static const uint8_t PinButton_Function_F4 = 57;
static const uint8_t PinButton_Function_F5 = 58;
static const uint8_t PinButton_Function_F6 = 59;
static const uint8_t PinButton_Function_F7 = 60;
static const uint8_t PinButton_Function_F8 = 61;
static const uint8_t PinButton_Function_F9 = 62;
static const uint8_t PinButton_Function_F10 = 63;
static const uint8_t PinButton_Function_F11 = 64;
static const uint8_t PinButton_Function_F12 = 65;
static const uint8_t PinButton_Function_F13 = 66;
static const uint8_t PinButton_Function_F14 = 67;
static const uint8_t PinButton_Function_F15 = 68;
static const uint8_t PinButton_Function_F16 = 69;

//
static const uint8_t PinButton_Modifier_Shift = 70;
static const uint8_t PinButton_Modifier_Option = 71;
static const uint8_t PinButton_Modifier_Control = 72;
static const uint8_t PinButton_Modifier_Alt = 73;

//
static const uint8_t PinButton_Automation_ReadOff = 74;
static const uint8_t PinButton_Automation_Touch = 75;
static const uint8_t PinButton_Automation_Trim = 76;
static const uint8_t PinButton_Automation_Overwrite = 77;
static const uint8_t PinButton_Automation_Latch = 78;
static const uint8_t PinButton_Automation_TrimLatch = 79;

//
static const uint8_t PinButton_Edit_Save = 80;
static const uint8_t PinButton_Edit_AudibleMode = 81;
static const uint8_t PinButton_Edit_Undo = 82;
static const uint8_t PinButton_Edit_Redo = 83;

//
static const uint8_t PinButton_Daw_RTZ = 84;
static const uint8_t PinButton_Daw_Marker = 85;
static const uint8_t PinButton_Daw_EditGrid = 86;

static const uint8_t PinButton_Daw_Cycle = 87;
static const uint8_t PinButton_Daw_Punch = 88;
static const uint8_t PinButton_Daw_Select = 89;

static const uint8_t PinLed_Daw_Cycle = 87;
static const uint8_t PinLed_Daw_Punch = 88;
static const uint8_t PinLed_Daw_Select = 89;

//
static const uint8_t PinButton_Transport_Solo = 90;
static const uint8_t PinButton_Transport_Rewind = 91;
static const uint8_t PinButton_Transport_FastForward = 92;
static const uint8_t PinButton_Transport_Stop = 93;
static const uint8_t PinButton_Transport_Play = 94;
static const uint8_t PinButton_Transport_Record = 95;

static const uint8_t PinLed_Transport_Solo = 90;
static const uint8_t PinLed_Transport_Rewind = 91;
static const uint8_t PinLed_Transport_FastForward = 92;
static const uint8_t PinLed_Transport_Stop = 93;
static const uint8_t PinLed_Transport_Play = 94;
static const uint8_t PinLed_Transport_Record = 95;

//
static const uint8_t PinButton_Navigation_Up = 96;
static const uint8_t PinButton_Navigation_Down = 97;
static const uint8_t PinButton_Navigation_Scrub = 98;
static const uint8_t PinButton_Navigation_Zoom = 99;
static const uint8_t PinButton_Navigation_Left = 100;
static const uint8_t PinButton_Navigation_Right = 101;
