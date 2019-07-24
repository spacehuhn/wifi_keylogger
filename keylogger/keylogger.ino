#include <hidboot.h>
#include <usbhub.h>
#include "Keyboard.h"

#define Serial Serial1

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
  #include <spi4teensy3.h>
  #include <SPI.h>
#endif

#include <SPI.h>
#include <SD.h>

bool shift = false;

USB     Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
uint32_t next_time;

class KbdRptParser : public KeyboardReportParser{
  public:
    uint8_t _parse(uint8_t key);
    String _getChar(uint8_t key);
  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
    
    void _press(uint8_t key);
    void _release(uint8_t key);
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key){
  int parsedKey = _parse(key);
  
  if((key >= 0x59) && (key <= 0x62)){

    uint8_t c;
    switch(key){
      case 0x59: c = '1'; break;
      case 0x5A: c = '2'; break;
      case 0x5B: c = '3'; break;
      case 0x5C: c = '4'; break;
      case 0x5D: c = '5'; break;
      case 0x5E: c = '6'; break;
      case 0x5F: c = '7'; break;
      case 0x60: c = '8'; break;
      case 0x61: c = '9'; break;
      case 0x62: c = '0'; break;
    }
    OnKeyPressed(c);
    _press(c);
    
    }else if(parsedKey == key){
      uint8_t c = OemToAscii(mod, key);
      OnKeyPressed(c);
      if(c != 0x20 && c != 0x00) _press(c);
      else _press(key);
  }else _press(parsedKey);
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key){
  int parsedKey = _parse(key);


  if((key >= 0x59) && (key <= 0x62)){

    uint8_t c;
    switch(key){
      case 0x59: c = '1'; break;
      case 0x5A: c = '2'; break;
      case 0x5B: c = '3'; break;
      case 0x5C: c = '4'; break;
      case 0x5D: c = '5'; break;
      case 0x5E: c = '6'; break;
      case 0x5F: c = '7'; break;
      case 0x60: c = '8'; break;
      case 0x61: c = '9'; break;
      case 0x62: c = '0'; break;
    };
     _release(c);
     Serial.print(_getChar(key));
     
  } else if(parsedKey == key){
    uint8_t c = OemToAscii(mod, key);
    OnKeyPressed(c);
    if(c != 0x20 && c != 0x00){
      _release(c);
      Serial.print((char)c);
    }
    else{
      _release(key);
      Serial.print("0x");
      Serial.print(key, HEX);
    }
  }else{
    _release(parsedKey);
    Serial.print(_getChar(key));
  }
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  //left
  if(beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl){
    if(afterMod.bmLeftCtrl) Keyboard.press(KEY_LEFT_CTRL);
    else Keyboard.release(KEY_LEFT_CTRL);
    Serial.print("<ctrl "+(String)afterMod.bmLeftCtrl+">");
  }
  
  if(beforeMod.bmLeftShift != afterMod.bmLeftShift){
    if(afterMod.bmLeftShift) Keyboard.press(KEY_LEFT_SHIFT);
    else Keyboard.release(KEY_LEFT_SHIFT);
    shift = afterMod.bmLeftShift;
    //Serial.print("<shift "+(String)afterMod.bmLeftShift+">");
  }
  
  if(beforeMod.bmLeftAlt != afterMod.bmLeftAlt){
    if(afterMod.bmLeftAlt) Keyboard.press(KEY_LEFT_ALT);
    else Keyboard.release(KEY_LEFT_ALT);
    Serial.print("<alt "+(String)afterMod.bmLeftAlt+">");
  }
  
  if(beforeMod.bmLeftGUI != afterMod.bmLeftGUI){
    if(afterMod.bmLeftGUI) Keyboard.press(KEY_LEFT_GUI);
    else Keyboard.release(KEY_LEFT_GUI);
    Serial.print("<gui "+(String)afterMod.bmLeftGUI+">");
  }

  //right
  if(beforeMod.bmRightCtrl != afterMod.bmRightCtrl){
    if(afterMod.bmRightCtrl) Keyboard.press(KEY_RIGHT_CTRL);
    else Keyboard.release(KEY_RIGHT_CTRL);
    Serial.print("<ctrl "+(String)afterMod.bmRightCtrl+">");
  }
  
  if(beforeMod.bmRightShift != afterMod.bmRightShift){
    if(afterMod.bmRightShift) Keyboard.press(KEY_RIGHT_SHIFT);
    else Keyboard.release(KEY_RIGHT_SHIFT);
    shift = afterMod.bmLeftShift;
    //Serial.print("<shift "+(String)afterMod.bmRightShift+">");
  }
  
  if(beforeMod.bmRightAlt != afterMod.bmRightAlt){
    if(afterMod.bmRightAlt) Keyboard.press(KEY_RIGHT_ALT);
    else Keyboard.release(KEY_RIGHT_ALT);
    Serial.print("<alt "+(String)afterMod.bmRightAlt+">");
  }
  
  if(beforeMod.bmRightGUI != afterMod.bmRightGUI){
    if(afterMod.bmRightGUI) Keyboard.press(KEY_RIGHT_GUI);
    else Keyboard.release(KEY_RIGHT_GUI);
    Serial.print("<gui "+(String)afterMod.bmRightGUI+">");
  }

}

void KbdRptParser::OnKeyPressed(uint8_t key){
  /*Serial.print("ASCII: \"");
  Serial.print((char)key);
  Serial.print("\" = 0x");
  Serial.print(key, HEX);
  Serial.print("; ");*/
};

uint8_t KbdRptParser::_parse(uint8_t key){
  /*
  Serial.print("0x");
  Serial.print(key, HEX);
  Serial.print(" = ");*/
  switch(key){
    case 0x2C: return 0x20; break; // SPACE
    case 40: return KEY_RETURN; break;
    case 41: return KEY_ESC; break;
    case 42: return KEY_BACKSPACE; break;
    case 43: return KEY_TAB; break;
    case 58: return KEY_F1; break;
    case 59: return KEY_F2; break;
    case 60: return KEY_F3; break;
    case 61: return KEY_F4; break;
    case 62: return KEY_F5; break;
    case 63: return KEY_F6; break;    
    case 64: return KEY_F7; break;
    case 65: return KEY_F8; break;
    case 66: return KEY_F9; break;
    case 67: return KEY_F10; break;
    case 68: return KEY_F11; break;
    case 69: return KEY_F12; break;
    case 73: return KEY_INSERT; break;
    case 74: return KEY_HOME; break;
    case 75: return KEY_PAGE_UP; break;
    case 76: return KEY_DELETE; break;
    case 77: return KEY_END; break;
    case 78: return KEY_PAGE_DOWN; break;
    case 79: return KEY_RIGHT_ARROW; break;
    case 80: return KEY_LEFT_ARROW; break;
    case 81: return KEY_DOWN_ARROW; break;
    case 82: return KEY_UP_ARROW; break;
    case 88: return KEY_RETURN; break;
    // Extension for Caps Lock
    case 0x39: return KEY_CAPS_LOCK; break;
    
    //=====[DE-Keyboard]=====//
    case 0x64: return 236; break; // <
    case 0x32: return 92; break; // #
    //======================//
    default: {
      //Serial.print(" N/A ");
      return key;
    }
  }
}
String KbdRptParser::_getChar(uint8_t key){
  switch(key){
    case 0x2C: return " "; break;
    case 40: return "<RETURN>\n"; break;
    case 41: return "<ESC>\n"; break;
    case 42: return "<BACKSPACE>"; break;
    case 43: return "<TAB>\n"; break;
    case 58: return "<F1>\n"; break;
    case 59: return "<F2>\n"; break;
    case 60: return "<F3>\n"; break;
    case 61: return "<F4>\n"; break;
    case 62: return "<F5>\n"; break;
    case 63: return "<F6>\n"; break;    
    case 64: return "<F7>\n"; break;
    case 65: return "<F8>\n"; break;
    case 66: return "<F9>\n"; break;
    case 67: return "<F10>\n"; break;
    case 68: return "<F11>\n"; break;
    case 69: return "<F12>\n"; break;
    case 73: return "<INSERT>"; break;
    case 74: return "<HOME>\n"; break;
    case 75: return "<PAGE_UP>\n"; break;
    case 76: return "<DELETE>"; break;
    case 77: return "<END>\n"; break;
    case 78: return "<PAGE_DOWN>\n"; break;
    case 79: return "<RIGHT_ARROW>\n"; break;
    case 80: return "<LEFT_ARROW>\n"; break;
    case 81: return "<DOWN_ARROW>\n"; break;
    case 82: return "<UP_ARROW>\n"; break;
    case 88: return "<RETURN>\n"; break;
    // Extension for NumPad
    case 0x59: return "<NUMPAD_1>\n"; break;
    case 0x5A: return "<NUMPAD_2>\n"; break;
    case 0x5B: return "<NUMPAD_3>\n"; break;
    case 0x5C: return "<NUMPAD_4>\n"; break;
    case 0x5D: return "<NUMPAD_5>\n"; break;
    case 0x5E: return "<NUMPAD_6>\n"; break;
    case 0x5F: return "<NUMPAD_7>\n"; break;
    case 0x60: return "<NUMPAD_8>\n"; break;
    case 0x61: return "<NUMPAD_9>\n"; break;
    case 0x62: return "<NUMPAD_0>\n"; break;
    case 0x39: return "<CAPS_LOCK>\n"; break;
    //=====[DE-Keyboard]=====//
    case 0x64: {
      if(shift) return "<";
      else return ">";
      break;
    }
    case 0x32:{
      if(shift) return "'";
      else return "#";
      break;
    }
    //======================//
    default: {
      return "";
    }
  }
}

void KbdRptParser::_press(uint8_t key){
  /*Serial.print("0x");
  Serial.print(key, HEX);
  Serial.println(" DOWN");*/
  Keyboard.press(key);
}

void KbdRptParser::_release(uint8_t key){
  /*Serial.print("0x");
  Serial.print(key, HEX);
  Serial.println(" UP");
  Serial.println();*/
  Keyboard.release(key);
}

KbdRptParser parser;


void setup()
{
  Serial.begin(115200);
  Keyboard.begin();
  delay(2000);
  
/*
  for(int i=0;i<256;i++){
    
    int key = parser._parse(i);
    if(key == i){
      Keyboard.print((String)i+" ");
      Keyboard.write(i);
      delay(200);
      Keyboard.write(KEY_RETURN);
    }
  }*/
  
  if(Usb.Init() == -1) Serial.println("OSC did not start.");
  
  delay(200);

  next_time = millis() + 5000;

  HidKeyboard.SetReportParser(0, &parser);

}

void loop(){
  Usb.Task();
}
