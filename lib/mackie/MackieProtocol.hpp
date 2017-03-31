#pragma once

#include "MackieDefs.h"
#include <usb_midi.h>

BEGIN_MACKIE_NAMESPACE

class MackieProtocol;
mackie::MackieProtocol* protocol;

class MackieProtocol {

	private:
		usb_midi_class& mInterface;
		uint8_t mMode = SYX_NO_MODE;
		uint8_t mBuffer[SYSEX_BUF_SIZE];
		// exposed states
		uint16_t mCommandState[COMMANDS_NB];
		uint16_t mLcdLines[LCD_CHAR_NB];
		uint16_t mVpotRings[FADER_NB];
		uint16_t mFaders[FADER_NB];
		uint16_t mVuMeters[CHANNEL_NB];
		uint16_t mTimecode[MTC_DIGIT_NB];
    uint16_t mSegmentLcd[SEG_DIGIT_NB];

  public:
		uint8_t states[STATES_NB];
		uint16_t errors[ERRORS_NB];
  /** mode is one of OperationModes */
  MackieProtocol(uint8_t mode)
  	: mInterface(usbMIDI)
		, mMode(mode) {
		mackie::protocol = this;
	}

  virtual ~MackieProtocol() {}

  inline void begin(void) {
    mInterface.setHandleSysEx(MackieProtocol::sysExDelegate);
    mInterface.setHandleControlChange(MackieProtocol::controlChangeDelegate);
    mInterface.setHandlePitchChange(MackieProtocol::pitchChangeDelegate);
    mInterface.setHandleAfterTouch(MackieProtocol::afterTouchDelegate);
    mInterface.setHandleNoteOn(MackieProtocol::noteOnDelegate);
    mInterface.setHandleNoteOff(MackieProtocol::noteOffDelegate);
    sendSysEx(TX_HOST_CONNECT_QUERRY);
  }

  inline void update(void) {
  	mInterface.read();
  }

  /*	Error, Command, Lcd, VPotRings, Faders, VuMeters, Timecode, SegmentLcd
  */

  inline uint16_t* getState(uint8_t type) {
    switch (type) {
      case States::Error: {
        return getErrors();
      }

      case States::Command: {
        return getCommands();
      }

      case States::Lcd: {
        return getLcd();
      }

      case States::VPotRings: {
        return getVPotRings();
      }

      // case States::Faders: {
      //   return getFaders();
      // }

      case States::VuMeters: {
        return getVuMeters();
      }

      case States::Timecode: {
        return getTimecode();
      }

      case States::SegmentLcd: {
        return getSegmentLcd();
      }

      default: {
        return 0;
      }
    }
  }

  inline bool hasStateChanged(uint8_t type) {
    return this->states[type] ? true : false;
  }

  inline uint16_t* getErrors(void) {
    return this->errors;
  }

  inline uint16_t* getCommands(void) {
    this->states[States::Command] = State::Unchanged;
    return mCommandState;
  }

  inline uint16_t* getFaders(void) {
    this->states[States::Faders] = State::Unchanged;
    return mFaders;
  }

  inline uint16_t* getLcd(void) {
    this->states[States::Lcd] = State::Unchanged;
    return mLcdLines;
  }

  inline uint16_t* getVPotRings(void) {
    this->states[States::VPotRings] = State::Unchanged;
    return mVpotRings;
  }

  inline uint16_t* getVuMeters(void) {
    this->states[States::VuMeters] = State::Unchanged;
    return mVuMeters;
  }

  inline uint16_t* getTimecode(void) {
    this->states[States::Timecode] = State::Unchanged;
    return mTimecode;
  }


  inline uint16_t* getSegmentLcd(void) {
    this->states[States::SegmentLcd] = State::Unchanged;
    return mSegmentLcd;
  }

  inline void sendCommandOn( uint8_t cmd, uint8_t channel = CONTROL_CHANNEL) {
    sendCommand(cmd, On, channel);
  }

  inline void sendCommandOff(uint8_t cmd, uint8_t channel = CONTROL_CHANNEL) {
    sendCommand(cmd, Off, channel);
  }

  inline void sendCommand(uint8_t cmd, int value = On, uint8_t channel = CONTROL_CHANNEL) {
    switch(cmd) {
      /*
      @param channel 0-7
      @param value 0 - left, 1 - right
      */
      case Commands::JogWheelMove: {
        // starts with 16 up to 23
        return mInterface.sendControlChange(
          JOG_WHEEL_ID, value ? JOG_WHEEL_RIGHT : JOG_WHEEL_LEFT, CONTROL_CHANNEL);
      }
      /*
      @param channel 0-7
      @param value 0 - down, 1 - up
      */
      case Commands::Channel_VpotMove: {
        // starts with 16 up to 23
        return mInterface.sendControlChange(
          channel + 16, value ? JOG_WHEEL_RIGHT : JOG_WHEEL_LEFT, CONTROL_CHANNEL);
      }

      /*
      @param channel 0-7
      */
      case Commands::Channel_FaderMove: {
        return mInterface.sendPitchBend(TO_PITCHBEND_SCALE(value), ++channel);
      }

      default: {
        return mInterface.sendNoteOn(cmd, value, channel);
      }
    }
  }

  inline void noteOnHandler(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (channel != CONTROL_CHANNEL || note > COMMANDS_NB) {
      return;
    }
    mCommandState[note] = velocity;
    this->states[States::Command] = State::Changed;
  }

  inline void noteOffHandler(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (channel != CONTROL_CHANNEL || note > COMMANDS_NB) {
      return;
    }
    mCommandState[note] = Off;
    this->states[States::Command] = State::Changed;
  }

  /*
  Time code display 7 segment display, single char, fired continously
  Handles pan pot and general purpose controller updates as well
  Handled protocol requests: RX_WRITE_SINGLE_MTC
  */
  inline void controlChangeHandler(uint8_t channel, uint8_t number, uint8_t value) {
    if(channel != CONTROL_CHANNEL) {
      return;
    }

    uint8_t command = number & 0xF0;
    switch (command) {
      // write single digit to led position
      case RX_WRITE_SINGLE_MTC: {
        uint8_t pos = (number & 0x0F);
        if(pos >= MTC_DIGIT_NB) {
          return;
        }
        mTimecode[pos] = (value & 0x0F);
        this->states[States::Timecode] = State::Changed;
        return;
      }

      // writes vpot ring values
      case RX_WRITE_VPOT_RING: {
        uint8_t chan = number & 0x0F;
        if (chan >= FADER_NB) {
          return;
        }
        mVpotRings[chan] = value & 0x0F;
        this->states[States::VPotRings] = State::Changed;
        return;
      }
    }
  }

  /*
  Fader movement, fired continously if fader moves, fires on automation
  Get fader index 0-7 or 08 if master fader
  */
  inline void pitchChangeHandler(uint8_t channel, int bend) {
    if (--channel > FADER_NB) {
      return;
    }
    // int to 7 bit fader value
    // mFaders[channel] = (bend >> 7) & 0x7F;
		// mFaders[channel] = (bend >> 4);
		mFaders[channel] = FROM_PITCHBEND_SCALE(bend);
    this->states[States::Faders] = State::Changed;
  }

  /*
  8 channels metering bargraph, fired continously
  */
  inline void afterTouchHandler(uint8_t channel, uint8_t pressure) {
    uint8_t chan = pressure >> 4;
    if(channel != CONTROL_CHANNEL || chan >= CHANNEL_NB) {
      return;
    }
    mVuMeters[chan] = pressure & 0x0F;
    this->states[States::VuMeters] = State::Changed;
  }

  /**
  Handle all SystemExclusive messages:
  RX_DEVICE_QUERY, RX_HOST_CONNECT_REPLY, RX_FW_VERSION_REQUEST, RX_SERIAL_NB_REQUEST_MACKIE,
  RX_WRITE_MULTIPLE_MTC, RX_WRITE_MULTIPLE_7SEG, RX_WRITE_LCD_TXT
  Sends following commads:
  TX_HOST_CONNECT_QUERRY TX_CONFIRM_CONNECT, TX_FW_VERSION_REPLY, TX_SERIAL_NB_REPLY_MACKIE
  */
  inline void sysExHandler(const uint8_t *data, uint16_t length, bool complete) {
    uint16_t pos = 0;
    // only accept mackie sysex with the correct version number
    if (length < SYX_HEADER_SIZE || data[SYX_HEADER_SIZE] != mMode) {
      return;
    }

    // only accept correct mackie header sysexes
    while((data[pos] == SYSEX_HEADER[pos]) && (pos < SYX_HEADER_SIZE)) {
      pos++;
    }

    if(pos != SYX_HEADER_SIZE) {
      return;
    }

    uint8_t command = data[++pos];
    switch(command) {
      // The host request a device query : answer
      case RX_DEVICE_QUERY: {
        return sendSysEx(TX_HOST_CONNECT_QUERRY);
      }

      // The host answered from a connection query : confirm
      case RX_HOST_CONNECT_REPLY: {
        return sendSysEx(TX_CONFIRM_CONNECT);
      }

      // The host requests the firmware version query : confirm
      case RX_FW_VERSION_REQUEST: {
        return sendSysEx(TX_FW_VERSION_REPLY);
      }

      // Is it a RX_SERIAL_NB_REQUEST_MACKIE V1.69 Mackie only
      // The host answered from a connection query : confirm
      case RX_SERIAL_NB_REQUEST_MACKIE: {
        return sendSysEx(TX_SERIAL_NB_REPLY_MACKIE);
      }

      // The host sends multiple caracters to display on mackie time code
      case RX_WRITE_MULTIPLE_MTC: {
        uint8_t diplayIndex = 0;
        while(data[++pos] != MIDI_EOX && diplayIndex > MTC_DIGIT_NB) {
          mTimecode[diplayIndex++] = data[pos];
        }
        this->states[States::Timecode] = State::Changed;
        return;
      }

      // The host sends multiple caracters to display on mackie 7 segment code
      case RX_WRITE_MULTIPLE_7SEG: {
        mSegmentLcd[1] = data[pos + 1];
        mSegmentLcd[0] = data[pos + 2];
        this->states[States::SegmentLcd] = State::Changed;
        return;
      }

      // Get the offset in the lines to update and get all new texts
      case RX_WRITE_LCD_TXT: {
        pos++;
        uint8_t displayLen = data[pos];
        while(data[++pos] != MIDI_EOX && displayLen < LCD_CHAR_NB) {
          mLcdLines[displayLen++] = ((char) data[pos]);
        }
        this->states[States::Lcd] = State::Changed;
        return;
      }

      // RX_OFFLINE_STATUS - Logic Only
      case RX_OFFLINE_STATUS_LOGIC: {
        if (mMode == SYX_ID3_LOGIC || mMode == SYX_ID3_LOGIC_XT) {
          this->errors[Errors::Offline] = Errors::Offline;
          this->states[States::Error] = State::Changed;
        }
      }
    }
  }

  inline void sendSysEx(uint8_t command) {
    // add header + mMode + command + eox bytes to sysex lenght
    uint8_t len = SYX_HEADER_SIZE + 3;
    // Send a Mackie sysex header
    mBuffer[0] = MIDI_SOX;
    mBuffer[1] = SYX_ID0;
    mBuffer[2] = SYX_ID1;
    mBuffer[3] = SYX_ID2;
    mBuffer[4] = mMode;
    mBuffer[5] = command;

    if(command == TX_FW_VERSION_REPLY) {
      // send  VV VV VV VV VV F7 (VV=firmware version eg "1.0")
      mBuffer[6] = 'V';
      mBuffer[7] = TO_SYSBYTE(IDENTITY[FW_VERSION_INDEX]);
      mBuffer[8] = '.';
      mBuffer[9] = TO_SYSBYTE(IDENTITY[FW_SUB_VERSION_INDEX]);
      mBuffer[10] = '0';
      len += FW_VERSION_LENGHT;
    } else {
      // here command is one of TX_HOST_CONNECT_QUERRY TX_CONFIRM_CONNECT
      // TX_ERROR_CONNECT, TX_SERIAL_NB_REPLY_MACKIE
      // send SS SS SS SS SS SS SS F7 (SS=serial number)
      mBuffer[6] = TO_SYSBYTE(IDENTITY[MANUF_YEAR_INDEX]);
      mBuffer[7] = TO_SYSBYTE(IDENTITY[MANUF_MONTH_INDEX]);
      mBuffer[8] = TO_SYSBYTE(IDENTITY[SERIAL_4_INDEX]);
      mBuffer[9] = TO_SYSBYTE(IDENTITY[SERIAL_3_INDEX]);
      mBuffer[10] = TO_SYSBYTE(IDENTITY[SERIAL_2_INDEX]);
      mBuffer[11] = TO_SYSBYTE(IDENTITY[SERIAL_1_INDEX]);
      mBuffer[12] = TO_SYSBYTE(IDENTITY[SERIAL_0_INDEX]);
      len += SERIAL_NB_LENGHT;

      // if command is connection query must send chalenge code
      // send LL LL LL LL
      if(command == TX_HOST_CONNECT_QUERRY) {
        mBuffer[13] = '1';
        mBuffer[14] = '2';
        mBuffer[15] = '3';
        mBuffer[16] = '4';
        len += CHALLENGE_LENGHT;
      }
    }

    // send end of sysex
    mBuffer[len -1] = MIDI_EOX;
    mInterface.sendSysEx(len, mBuffer);
    mInterface.send_now();
  }

  static inline void sysExDelegate(const uint8_t *data, uint16_t length, bool complete) {
    mackie::protocol->sysExHandler(data, length, complete);
  }

  static inline void noteOnDelegate(uint8_t channel, uint8_t note, uint8_t velocity) {
    mackie::protocol->noteOnHandler(channel, note, velocity);
  }

  static inline void noteOffDelegate(uint8_t channel, uint8_t note, uint8_t velocity) {
    mackie::protocol->noteOffHandler(channel, note, velocity);
  }

  static inline void pitchChangeDelegate(uint8_t channel, int bend) {
    mackie::protocol->pitchChangeHandler(channel, bend);
  }

  static inline void controlChangeDelegate(uint8_t channel, uint8_t number, uint8_t value) {
    mackie::protocol->controlChangeHandler(channel, number, value);
  }

  static inline void afterTouchDelegate(uint8_t channel, uint8_t pressure) {
    mackie::protocol->afterTouchHandler(channel, pressure);
  }
};

END_MACKIE_NAMESPACE
