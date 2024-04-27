// NSW-149 Controller Key Assign for X68000Z GRANADA/AXIS
#include <Keyboard.h>
#include <XBOXONE.h>
#include <HID.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

class NSW149 : public XBOXONE {
public:
  NSW149(USB *pUsb):XBOXONE(pUsb) {};
  virtual uint8_t Poll();
  virtual bool VIDPIDOK(uint16_t vid, uint16_t pid) {
    return (vid == 0x0F0D && pid == 0x00F1);
  };
  uint16_t getStick(ButtonEnum b);
  uint16_t getFunction(ButtonEnum b);
  uint16_t getButton(ButtonEnum b);

private:
  uint8_t readBuf[XBOX_ONE_EP_MAXPKTSIZE];
};

uint8_t NSW149::Poll() {
  uint8_t rcode = 0;

  if(!isReady())
    return 0;

  if((int32_t)((uint32_t)millis() - qNextPollTime) >= 0L) {
    qNextPollTime = (uint32_t)millis() + readPollInterval();
    uint16_t length =  (uint16_t)epInfo[ XBOX_ONE_INPUT_PIPE ].maxPktSize;
    uint8_t rcode = pUsb->inTransfer(bAddress, epInfo[ XBOX_ONE_INPUT_PIPE ].epAddr, &length, readBuf, readPollInterval());
    if(!rcode) {
    }
  }
  return rcode;
}

uint16_t NSW149::getStick(ButtonEnum b) {
  if (b == UP) {
    if (readBuf[2] == 0 || readBuf[2] == 1 || readBuf[2] == 7) return true;
  } else
  if (b == RIGHT) {
    if (readBuf[2] == 1 || readBuf[2] == 2 || readBuf[2] == 3) return true;
  } else
  if (b == DOWN) {
    if (readBuf[2] == 3 || readBuf[2] == 4 || readBuf[2] == 5) return true;
  } else
  if (b == LEFT) {
    if (readBuf[2] == 5 || readBuf[2] == 6 || readBuf[2] == 7) return true;
  }
  return false;
}

uint16_t NSW149::getFunction(ButtonEnum b) {
  if (b == CAPTURE) {
    if (readBuf[1] & 0b00100000) return true;
  } else
  if (b == HOME) {
    if (readBuf[1] & 0b00010000) return true;
  } else
  if (b == MINUS) {
    if (readBuf[1] & 0b00000001) return true;
  } else
  if (b == PLUS) {
    if (readBuf[1] & 0b00000010) return true;
  }
  return false;
}

uint16_t NSW149::getButton(ButtonEnum b) {
  if (b == Y) {
    if (readBuf[0] & 0b00000001) return true;
  } else
  if (b == X) {
    if (readBuf[0] & 0b00001000) return true;
  } else
  if (b == R) {
    if (readBuf[0] & 0b00100000) return true;
  } else
  if (b == L) {
    if (readBuf[0] & 0b00010000) return true;
  }
  if (b == B) {
    if (readBuf[0] & 0b00000010) return true;
  } else
  if (b == A) {
    if (readBuf[0] & 0b00000100) return true;
  } else
  if (b == ZR) {
    if (readBuf[0] & 0b10000000) return true;
  } else
  if (b == ZL) {
    if (readBuf[0] & 0b01000000) return true;
  }
  return false;
}

class REGIST
{
public:
  REGIST(void);
  void press(void);
  void release(void);
};

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  0x05, 0x0C,
  0x09, 0x01,
  0xA1, 0x01,
  0x85, 0x01,
  0x15, 0x00,
  0x25, 0x01,
  0x09, 0xE9,
  0x09, 0xEA,
  0x09, 0xE2,
  0x75, 0x01,
  0x95, 0x03,
  0x81, 0x02,
  0x75, 0x01,
  0x95, 0x05,
  0x81, 0x03,
  0xC0
};

REGIST::REGIST(void)
{
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
}

void REGIST::press()
{
  u8 m[] = {2};
  HID().SendReport(0x01, m, sizeof(m));
}

void REGIST::release()
{
  u8 m[] = {0};
  HID().SendReport(0x01, m, sizeof(m));
}

USB Usb;
NSW149 fs(&Usb);
REGIST Regist;

static bool kp8 = false;
static bool kp2 = false;
static bool kp4 = false;
static bool kp6 = false;
static bool kpe = false;
static bool kesc = false;
static bool kspc = false;
static bool kreg = false;
static bool kx = false;
static bool kz = false;

void setup() {
  if (Usb.Init() == -1) {
    while (1); //halt
  }
}

void loop() {
  Usb.Task();
  if (fs.XboxOneConnected) {
    if (fs.getStick(UP)) {
      if (!kp8) {
        Keyboard.press(KEY_KP_8);
        kp8 = true;
      }
    } else {
      if (kp8) {
        Keyboard.release(KEY_KP_8);
        kp8 = false;
      }
    }
    if (fs.getStick(DOWN)) {
      if (!kp2) {
        Keyboard.press(KEY_KP_2);
        kp2 = true;
      }
    } else {
      if (kp2) {
        Keyboard.release(KEY_KP_2);
        kp2 = false;
      }
    }
    if (fs.getStick(LEFT)) {
      if (!kp4) {
        Keyboard.press(KEY_KP_4);
        kp4 = true;
      }
    } else {
      if (kp4) {
        Keyboard.release(KEY_KP_4);
        kp4 = false;
      }
    }
    if (fs.getStick(RIGHT)) {
      if (!kp6) {
        Keyboard.press(KEY_KP_6);
        kp6 = true;
      }
    } else {
      if (kp6) {
        Keyboard.release(KEY_KP_6);
        kp6 = false;
      }
    }

    if (fs.getButton(B)) {
      if (!kz) {
        Keyboard.press('z');
        kz = true;
      }
    } else {
      if (kz) {
        Keyboard.release('z');
        kz = false;
      }
    }
    if (fs.getButton(A)) {
      if (!kx) {
        Keyboard.press('x');
        kx = true;
      }
    } else {
      if (kx) {
        Keyboard.release('x');
        kx = false;
      }
    }
    if (fs.getButton(ZR)) {
      if (!kspc) {
        if (kp8) {
          Keyboard.release(KEY_KP_8);
        }
        if (kp2) {
          Keyboard.release(KEY_KP_2);
        }
        if (kp4) {
          Keyboard.release(KEY_KP_4);
        }
        if (kp6) {
          Keyboard.release(KEY_KP_6);
        }
        Keyboard.releaseAll();
        delay(100);
        Keyboard.press(' ');
        if (kx) {
          Keyboard.press('x');
        }
        if (kz) {
          Keyboard.press('z');
        }
        if (kp8) {
          Keyboard.press(KEY_KP_8);
        }
        if (kp2) {
          Keyboard.press(KEY_KP_2);
        }
        if (kp4) {
          Keyboard.press(KEY_KP_4);
        }
        if (kp6) {
          Keyboard.press(KEY_KP_6);
        }
        kspc = true;
      }
    } else {
      if (kspc) {
        delay(100);
        Keyboard.release(' ');
        kspc = false;
      }
    }

    if (fs.getButton(L)) {
      if (!kreg) {
        Regist.press();
        kreg = true;
      }
    } else {
      if (kreg) {
        Regist.release();
        kreg = false;
      }
    }

    if (fs.getFunction(CAPTURE)) {
      if (!kesc) {
        Keyboard.press(KEY_ESC);
        kesc = true;
      }
    } else {
      if (kesc) {
        Keyboard.release(KEY_ESC);
        kesc = false;
      }
    }
    if (fs.getFunction(HOME)) {
      if (!kpe) {
        Keyboard.write(KEY_KP_ENTER);
        kpe = true;
      }
    } else {
      if (kpe) {
        Keyboard.write(KEY_KP_ENTER);
        kpe = false;
      }
    }
  }

  delay(1);
}
