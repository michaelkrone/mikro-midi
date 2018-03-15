#pragma once

#include <SPI.h>
#include "IoDefs.h"
#include "IoIc.hpp"

BEGIN_IO_NAMESPACE

#ifndef MAX72_BUFFERSIZE
  #define MAX72_NUM_DRIVERS 1
  #define MAX72_BUFFERSIZE MAX72_NUM_DRIVERS * 8
#endif

class IcMax72 : public Ic {
  protected:
  ioPin mPinCs;
  byte mNumDrivers;
  byte mDisplayBuffer[MAX72_BUFFERSIZE];

  /* Initialise the Maxim driver(s) */
  void init(void) {
    for (byte driverIndex = 0; driverIndex < MAX72_NUM_DRIVERS; driverIndex++) {
      write(MAX72_DECODE, 0, driverIndex);
      setDriverIntensity(0x0F, driverIndex);
      setScanLimit(7, driverIndex);
      setTestMode(MAX72_TESTMODEOFF, driverIndex);
      shutdown(MAX72_ON, driverIndex);
      clear();
      refresh();
    }
  }

  /* Write to one of the drivers registers. No-ops are sent to all other
     drivers in the chain.
     Driver is the driver number in the chain
  */
  void write(byte address, byte data, byte driverNumber) {
    digitalWriteFast(mPinCs, LOW);

    for (byte driverIndex = 0; driverIndex < MAX72_NUM_DRIVERS; driverIndex++) {
      if(driverIndex == driverNumber) {
    	  SPI.transfer(address);
    	  SPI.transfer(data);
      } else {
    	  SPI.transfer(MAX72_NOOP);
    	  SPI.transfer(0);
      }
    }
    digitalWriteFast(mPinCs, HIGH);
    digitalWriteFast(mPinCs, LOW);
  }

  public:
  IcMax72(ioPin pinCs)
      : mPinCs(pinCs)
  {
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    pinMode(mPinCs, OUTPUT);
    digitalWriteFast(mPinCs, HIGH);
    init();
  }

  virtual ~IcMax72() {}

  /* Set the intensity of the LED's.
     Valid values for Level are 0 (min) to 0x0F (max)
     Driver is the driver number in the chain
  */
  void setDriverIntensity(byte level, byte driverNumber) {
    if (level < 0 || level > 0x0F) {
      return;
    }
    write(MAX72_INTENSITY, level, driverNumber);
  }

  /* Set the intensity of the LED's.
     Valid values for Level are 0 (min) to 0x0F (max)
  */
  void setIntensity(byte level) {
    for (byte driverIndex = 0; driverIndex < MAX72_NUM_DRIVERS; driverIndex++) {
      setDriverIntensity(level, driverIndex);
    }
  }

  /* Sets the number of digits to drive in each driver.
     Valid values for Digits are:
     0 (Digit 0)
     1 (Digit 0 & 1)
     2 (Digit 0, 1 & 2)
     3 (Digit 0, 1, 2, & 3)
     4 (Digit 0, 1, 2, 3 & 4)
     5 (Digit 0, 1, 2, 3, 4 & 5)
     6 (Digit 0, 1, 2, 3, 4, 5, & 6)
     7 (Digit 0, 1, 2, 3, 4, 5,, 6 & 7)
     Driver is the driver number in the chain
  */
  void setScanLimit(byte digits, byte driverNumber) {
    if (digits < 0 || digits > 7) {
      return;
    }
    write(MAX72_SCANLIMIT, digits, driverNumber);
  }

  /* Put the driver into test mode. All outputs are turned on.
     Valid values for mode are:
     MAX72_TESTMODEOFF (test mode off)
     MAX72_TESTMODEON (test mode on)
     Driver is the driver number in the chain
  */
  void setTestMode(boolean enabled, byte driverNumber) {
    write(MAX72_TEST, enabled ? MAX72_TESTMODEON : MAX72_TESTMODEOFF, driverNumber);
  }

  /* Clear the output buffer */
  void clear(void) {
    for (unsigned int bufferIndex = 0; bufferIndex < MAX72_BUFFERSIZE; bufferIndex++) {
  	   mDisplayBuffer[bufferIndex] = 0;
    }
  }

  /* Update all drivers with the contents of the output buffer */
  void refresh(void) {
    byte driverIndex = 0;
    byte digit = 0;

    /* Write to each of the 8 digit registers in each driver */
    while(driverIndex < MAX72_BUFFERSIZE) {
      write(digit + MAX72_DIGIT1, mDisplayBuffer[digit + (driverIndex * 8)], driverIndex);
      digit++;
      if (digit == 8) {
        digit = 0;
        driverIndex++;
      }
    }
  }

  /* Put a driver into shutdown mode. Valid values for Mode are:
     MAX7219OFF (shutdown mode)
     MAX7219ON (normal operation)
     Driver is the driver number in the chain
  */
  void shutdown(boolean enable, byte driverNumber) {
    write(MAX72_SHUTDOWN, enable ? MAX7219OFF : MAX7219ON, driverNumber);
  }

  /* Loads a text string into the output buffer using the seven segment
     character set
  */
  void write7SegString(char text[], unsigned int offset) {
    unsigned int len = strlen(text);
    unsigned int bufferIndex;

    /* Set output buffer pointer */
    if(offset < MAX72_BUFFERSIZE) {
      bufferIndex = offset;
    } else {
      bufferIndex = MAX72_BUFFERSIZE;
    }

    /* If text runs beyond the output buffer then crop it */
    byte index = 0;
    if (offset > MAX72_BUFFERSIZE)
      // index = index - (offset - MAX72_BUFFERSIZE);
      index = offset - MAX72_BUFFERSIZE;
    /* Copy text into output buffer */
    while(bufferIndex != 0 && index != len) {
      bufferIndex--;
      mDisplayBuffer[bufferIndex] = MAX72_7SEG_CHAR_MAP[text[index] - 32];
      index++;
    }
  }

  /* Loads a long into the output buffer using the seven segment
     character set
  */
  void write7SegNumber(long number, unsigned int offset, int decimalPlace = MAX72_NO_DECIMAL) {
    long tmp = number;
    char digits[10];
    byte index = 0;
    unsigned int bufferIndex;

   /*Is the number negative ? If so then remove the sign */
    if (tmp < 0) {
      tmp *= -1;
    }

    /* Is the number zero ? */
    if (tmp == 0) {
      digits[index] = '0';
      index++;
    } else {
      /* Convert the number to an ASCII decimal string */
      while (tmp) {
        digits[index] = (tmp % 10) + 48;
        tmp /= 10;
        index++;
      }

      /* If the number was negative add the sign */
      if (number < 0) {
        digits[index] = '-';
        index++;
      }
    }

    if (decimalPlace != MAX72_NO_DECIMAL && decimalPlace == index) {
      digits[index] = '0';
      index++;
    }

    /* Set output buffer pointer */
    if(offset < MAX72_BUFFERSIZE) {
      bufferIndex = offset;
    } else {
      bufferIndex = MAX72_BUFFERSIZE;
    }

    /* If text runs beyond the output buffer then crop it */
    if (offset > MAX72_BUFFERSIZE) {
      index = index - (offset - MAX72_BUFFERSIZE);
    }

    /* Copy text into output buffer */
    while(index && bufferIndex) {
      index--;
      bufferIndex--;
      mDisplayBuffer[bufferIndex] = MAX72_7SEG_CHAR_MAP[digits[index]-32];

      if(decimalPlace != MAX72_NO_DECIMAL && decimalPlace != 0 && index == decimalPlace) {
        mDisplayBuffer[bufferIndex] = MAX72_7SEG_CHAR_MAP[digits[index] - 32] |  MAX72_7SEG_CHAR_MAP[14];
      } else {
        mDisplayBuffer[bufferIndex] = MAX72_7SEG_CHAR_MAP[digits[index] - 32];
      }
    }
  }
};

END_IO_NAMESPACE
