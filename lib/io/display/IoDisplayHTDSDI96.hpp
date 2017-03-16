#pragma once

#include "IoDisplayDefs.h"
#include "IoDisplay.hpp"

BEGIN_IO_NAMESPACE

//command macro
#define SSD1306_CMD_DISPLAY_OFF 0xAE // --turn off the OLED
#define SSD1306_CMD_DISPLAY_ON 0xAF // --turn on oled panel
#define SSD1306_SWAP(a, b) { int16_t t = a; a = b; b = t; }

class DisplayHTDSDI96 : public Adafruit_GFX {
  protected:
  	ioPin mPinSDA;
  	ioPin mPinSCL;
    // the frame buffer for the adafruit gfx. size=64x8 bytes
  	unsigned char* mFrameBuffer;

  public:
    DisplayHTDSDI96(ioPin sda, ioPin scl) : Adafruit_GFX(SSD1306_WIDTH, SSD1306_HEIGHT)
    , mPinSDA(sda)
    , mPinSCL(scl)
    , mFrameBuffer(0) {}

    ~DisplayHTDSDI96() {
    	// release the memory.
    	if (mFrameBuffer){
    		free(mFrameBuffer);
    	}
    }

    // initialized the ssd1306 in the setup function
    bool initialize() {
    	// setup the pin mode
    	pinMode(mPinSDA, OUTPUT);
    	pinMode(mPinSCL, OUTPUT);

    	// malloc the framebuffer.
    	mFrameBuffer = (unsigned char*) malloc(SSD1306_FBSIZE);
    	if (mFrameBuffer == 0) {
    		return false;
    	}
    	memset(mFrameBuffer, 0, SSD1306_FBSIZE); // clear it.

    	// write command to the screen registers.
    	writeCommand(SSD1306_CMD_DISPLAY_OFF); // display off
    	writeCommand(0x00); // Set Memory Addressing Mode
    	writeCommand(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    	writeCommand(0x40); // Set Page Start Address for Page Addressing Mode,0-7
    	writeCommand(0xB0); // Set COM Output Scan Direction
    	writeCommand(0x81); // ---set low column address
    	writeCommand(0xCF); // ---set high column address
    	writeCommand(0xA1); // --set start line address
    	writeCommand(0xA6); // --set contrast control register
    	writeCommand(0xA8);
    	writeCommand(0x3F); // --set segment re-map 0 to 127
    	writeCommand(0xC8); // --set normal display
    	writeCommand(0xD3); // --set multiplex ratio(1 to 64)
    	writeCommand(0x00); //
    	writeCommand(0xD5); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    	writeCommand(0x80); // -set display offset
    	writeCommand(0xD9); // -not offset
    	writeCommand(0xF1); // --set display clock divide ratio/oscillator frequency
    	writeCommand(0xDA); // --set divide ratio
    	writeCommand(0x12); // --set pre-charge period
    	writeCommand(0xDB); //
    	writeCommand(0x40); // --set com pins hardware configuration
    	writeCommand(0x8D); // --set vcomh
    	writeCommand(0x14); // 0x20,0.77xVcc
    	writeCommand(0xAF); // --set DC-DC enable
    	writeCommand(SSD1306_CMD_DISPLAY_ON); // --turn on oled panel

    	delay(10); // wait for the screen loaded.
    	return true;
    }

    void clear(bool isUpdateHW = false) {
    	memset(mFrameBuffer,0,SSD1306_FBSIZE); // clear the back buffer.
    	if (isUpdateHW) {
         // update the hw immediately
        update();
      }
    }

    void writeCommand(unsigned char cmd) {
    	startIIC();
    	writeByte(0x78);  // Slave address,SA0=0
    	writeByte(0x00);	// write command
    	writeByte(cmd);
    	stopIIC();
    }

    void writeByte(unsigned char b) {
    	unsigned char i;
    	for(i = 0; i < 8; i++) {
    		if ((b << i) & 0x80) {
    			digitalWriteFast(mPinSDA, HIGH);
    		} else {
    			digitalWriteFast(mPinSDA, LOW);
    		}
    		digitalWriteFast(mPinSCL, HIGH);
    		digitalWriteFast(mPinSCL, LOW);
    		//     IIC_Byte<<=1;
    	}
    	digitalWriteFast(mPinSDA, HIGH);
    	digitalWriteFast(mPinSCL, HIGH);
    	digitalWriteFast(mPinSCL, LOW);
    }

    void startIIC() {
    	digitalWriteFast(mPinSCL, HIGH);
    	digitalWriteFast(mPinSDA, HIGH);
    	digitalWriteFast(mPinSDA, LOW);
    	digitalWriteFast(mPinSCL, LOW);
    }

    void stopIIC() {
    	digitalWriteFast(mPinSCL, LOW);
    	digitalWriteFast(mPinSDA, LOW);
    	digitalWriteFast(mPinSCL, HIGH);
    	digitalWriteFast(mPinSDA, HIGH);
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color) {
    	unsigned char row;
    	unsigned char offset;
    	unsigned char  preData; // previous data.
    	unsigned char val;
    	int16_t  index;

    	if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()) || (mFrameBuffer == 0)) {
        return;
      }

      // only support 180 degree
      // otherwise the buffer has to be resized
      switch (getRotation()) {
        case 1: {
          SSD1306_SWAP(x, y);
          x = SSD1306_WIDTH - x - 1;
          break;
        }
        case 2: {
          x = SSD1306_WIDTH - x - 1;
          y = SSD1306_HEIGHT - y - 1;
          break;
        }
        case 3: {
          SSD1306_SWAP(x, y);
          y = SSD1306_WIDTH - y - 1;
          break;
        }
      }

    	// get the previous data;
    	row = y / 8;
    	offset = y % 8;
    	index = row * width() + x;
    	preData = mFrameBuffer[index];

    	// set pixel;
    	val = 1<<offset;
    	if (color!=0) { // white! set bit.
    		mFrameBuffer[index] = preData | val;
    	} else { // black! clear bit.
    		mFrameBuffer[index] = preData & (~val);
    	}
    }

    void startDataSequence() {
    	startIIC();
    	writeByte(0x78);
    	writeByte(0x40);	// write data
    }

    void update() {
      //  maybe use
      //  updateRow(0,SSD1306_MAXROW);
    	unsigned int  i=0;
    	unsigned char m,n;
    	for(m = 0; m < 8; m++) {
    		writeCommand(0xb0+m);	// page0-page1
    		writeCommand(0x00);		// low column start address
    		writeCommand(0x10);		// high column start address

    		startDataSequence();
    		for(n = 0; n < 128; n++) {
    			writeByte(mFrameBuffer[i++]);
    		}
    		stopIIC();
    	}
    }

    void updateRow(int rowID) {
    	unsigned char x;
    	unsigned int index;
    	if (rowID >= 0 && rowID < SSD1306_MAXROW && mFrameBuffer) {
        // this part is faster than else
    		// set the position
    		startIIC();
    		writeByte(0x78);  // Slave address,SA0=0
    		writeByte(0x00);	// write command

    		writeByte(0xb0 + rowID);
    		writeByte(((x & 0xf0)>>4) | 0x10); // |0x10
    		writeByte((x & 0x0f) | 0x01); // |0x01

    		stopIIC();

    		// start painting the buffer.
    		startDataSequence();
    		for(x = 0; x < SSD1306_WIDTH; x++) {
    			index = rowID * SSD1306_WIDTH + x;
    	  	writeByte(mFrameBuffer[index]);
    		}
    		stopIIC();
    	}
    }

    void updateRow(int startID, int endID) {
    	unsigned char y =0;
    	for(y = startID; y < endID; y++) {
    		updateRow(y);
    	}
    }
};

END_IO_NAMESPACE
