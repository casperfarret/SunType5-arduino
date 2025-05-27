/*
 * Complete Sun Type 5 USB Keyboard Adapter - Simplified
 * Working polling method with simple Caps Lock toggle
 * 
 * SparkFun Pro Micro Wiring:
 *    Keyboard Pin 2 : GND
 *    Keyboard Pin 8 : +5V  
 *    Keyboard Pin 6 : Arduino Pin 2 (RX from keyboard) (Optionally TX to Arduino Pin 3)
 */

#include <Keyboard.h>

#define RX_PIN 2
#define BIT_DURATION_US 833  // 1200 baud = ~833 microseconds per bit

// Sun keyboard responses
#define RESP_IDLE           0x7F
#define RESP_LAYOUT         0xFE  
#define RESP_RESET          0xFF

// Complete Sun Type 5 International scan code to USB HID mapping
const uint8_t sun_to_usb[128] = {
  0,             // 0x00 - Not used
  0,             // 0x01 - Stop (no USB equivalent)
  0,             // 0x02 - Volume Down (could map to media key)
  0,             // 0x03 - Again (no direct equivalent)
  0,             // 0x04 - Volume Up (could map to media key)
  KEY_F1,        // 0x05 - F1
  KEY_F2,        // 0x06 - F2
  KEY_F10,       // 0x07 - F10
  KEY_F3,        // 0x08 - F3
  KEY_F11,       // 0x09 - F11
  KEY_F4,        // 0x0A - F4
  KEY_F12,       // 0x0B - F12
  KEY_F5,        // 0x0C - F5
  KEY_RIGHT_ALT, // 0x0D - Graph Alt
  KEY_F6,        // 0x0E - F6
  0,             // 0x0F - Not used
  KEY_F7,        // 0x10 - F7
  KEY_F8,        // 0x11 - F8
  KEY_F9,        // 0x12 - F9
  KEY_LEFT_ALT,  // 0x13 - Alt
  KEY_UP_ARROW,  // 0x14 - Up Arrow (main keyboard)
  KEY_PAUSE,     // 0x15 - Pause
  KEY_PRINT_SCREEN, // 0x16 - Print Screen
  KEY_SCROLL_LOCK,  // 0x17 - Scroll Lock
  KEY_LEFT_ARROW, // 0x18 - Left Arrow (main keyboard)
  0,             // 0x19 - Props (no equivalent)
  0,             // 0x1A - Undo (no equivalent)
  KEY_DOWN_ARROW, // 0x1B - Down Arrow (main keyboard)
  KEY_RIGHT_ARROW, // 0x1C - Right Arrow (main keyboard)
  KEY_ESC,       // 0x1D - Escape
  '1',           // 0x1E - 1
  '2',           // 0x1F - 2
  '3',           // 0x20 - 3
  '4',           // 0x21 - 4
  '5',           // 0x22 - 5
  '6',           // 0x23 - 6
  '7',           // 0x24 - 7
  '8',           // 0x25 - 8
  '9',           // 0x26 - 9
  '0',           // 0x27 - 0
  '-',           // 0x28 - -
  '=',           // 0x29 - =
  '`',           // 0x2A - ` (grave/tilde)
  KEY_BACKSPACE, // 0x2B - Backspace
  0,             // 0x2C - Not used
  '=',           // 0x2D - Keypad =
  '/',           // 0x2E - Keypad /
  '*',           // 0x2F - Keypad *
  0,             // 0x30 - Not used
  0,             // 0x31 - Front (no equivalent)
  KEY_DELETE,    // 0x32 - Keypad Del/Period
  0,             // 0x33 - Copy (handled specially)
  0,             // 0x34 - Not used
  KEY_TAB,       // 0x35 - Tab
  'q',           // 0x36 - Q
  'w',           // 0x37 - W
  'e',           // 0x38 - E
  'r',           // 0x39 - R
  't',           // 0x3A - T
  'y',           // 0x3B - Y
  'u',           // 0x3C - U
  'i',           // 0x3D - I
  'o',           // 0x3E - O
  'p',           // 0x3F - P
  '[',           // 0x40 - [
  ']',           // 0x41 - ]
  KEY_DELETE,    // 0x42 - Delete
  0,             // 0x43 - Compose
  KEY_HOME,      // 0x44 - Keypad Home/7
  KEY_UP_ARROW,  // 0x45 - Keypad Up/8
  KEY_PAGE_UP,   // 0x46 - Keypad PgUp/9
  '-',           // 0x47 - Keypad -
  0,             // 0x48 - Open (no equivalent)
  0,             // 0x49 - Paste (handled specially)
  0,             // 0x4A - Not used
  0,             // 0x4B - Not used
  KEY_LEFT_CTRL, // 0x4C - Control
  'a',           // 0x4D - A
  's',           // 0x4E - S
  'd',           // 0x4F - D
  'f',           // 0x50 - F
  'g',           // 0x51 - G
  'h',           // 0x52 - H
  'j',           // 0x53 - J
  'k',           // 0x54 - K
  'l',           // 0x55 - L
  ';',           // 0x56 - ;
  '\'',          // 0x57 - '
  '\\',          // 0x58 - \ (backslash)
  KEY_RETURN,    // 0x59 - Return
  KEY_RETURN,    // 0x5A - Enter (keypad)
  KEY_LEFT_ARROW,  // 0x5B - Keypad Left/4
  '5',           // 0x5C - Keypad 5
  KEY_RIGHT_ARROW, // 0x5D - Keypad Right/6
  '0',           // 0x5E - Keypad Ins/0
  0,             // 0x5F - Find (handled specially)
  0,             // 0x60 - Not used
  0,             // 0x61 - Cut (handled specially)
  KEY_NUM_LOCK,  // 0x62 - Num Lock
  KEY_LEFT_SHIFT, // 0x63 - Shift
  'z',           // 0x64 - Z
  'x',           // 0x65 - X
  'c',           // 0x66 - C
  'v',           // 0x67 - V
  'b',           // 0x68 - B
  'n',           // 0x69 - N
  'm',           // 0x6A - M
  ',',           // 0x6B - ,
  '.',           // 0x6C - .
  '/',           // 0x6D - /
  KEY_RIGHT_SHIFT, // 0x6E - Right Shift
  KEY_RETURN,    // 0x6F - Line Feed (map to Return)
  KEY_END,       // 0x70 - Keypad End/1
  KEY_DOWN_ARROW, // 0x71 - Keypad Down/2
  KEY_PAGE_DOWN, // 0x72 - Keypad PgDn/3
  0,             // 0x73 - Not used
  0,             // 0x74 - Not used
  0,             // 0x75 - Not used
  0,             // 0x76 - Help (no equivalent)
  KEY_CAPS_LOCK, // 0x77 - Caps Lock (handled specially)
  0,             // 0x78 - Left Meta (handled specially as CMD)
  ' ',           // 0x79 - Space
  0,             // 0x7A - Right Meta (handled specially as CMD)
  0,             // 0x7B - Not used
  0,             // 0x7C - Not used
  '+',           // 0x7D - Keypad +
  0,             // 0x7E - Not used
  0              // 0x7F - Idle (handled separately)
};

// Simple state tracking
boolean caps_lock_on = false;
boolean shift_pressed = false;

void setup() {
  pinMode(RX_PIN, INPUT);
  
  // Initialize USB keyboard
  Keyboard.begin();
  
  delay(1000);
}

void loop() {
  uint8_t scancode = readSunByte();
  
  if (scancode != 0) {
    processScanCode(scancode);
  }
}

uint8_t readSunByte() {
  // Wait for start bit (transition from LOW to HIGH in negative logic)
  while (digitalRead(RX_PIN) == HIGH) {
    // Wait for line to go LOW (idle state in negative logic)
  }
  
  while (digitalRead(RX_PIN) == LOW) {
    // Wait for start bit (HIGH in negative logic)  
  }
  
  // Found start bit! Wait half a bit period to center on start bit
  delayMicroseconds(BIT_DURATION_US / 2);
  
  // Verify we're still in start bit
  if (digitalRead(RX_PIN) != HIGH) {
    return 0; // False start
  }
  
  // Read 8 data bits
  uint8_t receivedByte = 0;
  
  for (int bit = 0; bit < 8; bit++) {
    // Wait for next bit period
    delayMicroseconds(BIT_DURATION_US);
    
    // Read and invert bit (negative logic)
    int bitValue = !digitalRead(RX_PIN);
    
    // Store bit (LSB first)
    if (bitValue) {
      receivedByte |= (1 << bit);
    }
  }
  
  // Wait for stop bit
  delayMicroseconds(BIT_DURATION_US);
  
  return receivedByte;
}

void processScanCode(uint8_t scancode) {
  // Handle special response codes
  if (scancode == RESP_IDLE) {
    Keyboard.releaseAll();
    return;
  } else if (scancode == RESP_RESET) {
    return;
  } else if (scancode == RESP_LAYOUT) {
    return;
  } else if (scancode == 0x04) {
    return;
  }
  
  // Determine if this is a key press or release
  boolean keyUp = (scancode & 0x80) != 0;
  uint8_t makeCode = scancode & 0x7F;
  
  // Handle the key press/release
  handleKey(makeCode, keyUp);
}

void handleKey(uint8_t makeCode, boolean keyUp) {
  // Handle Caps Lock specially - toggle on key press only
  if (makeCode == 0x77) { // Caps Lock
    if (!keyUp) { // Only on key press (down)
      caps_lock_on = !caps_lock_on;
      
      // Method 1: Standard caps lock toggle
      Keyboard.press(KEY_CAPS_LOCK);
      delay(100);  
      Keyboard.release(KEY_CAPS_LOCK);
    }
    return; // Don't process caps lock as a regular key
  }
  
  // Handle Sun special keys for Mac
  if (makeCode == 0x78 || makeCode == 0x7A) { // Left/Right Meta (Diamond keys)
    if (keyUp) {
      Keyboard.release(KEY_LEFT_GUI);  // Mac CMD key
    } else {
      Keyboard.press(KEY_LEFT_GUI);
    }
    return;
  }
  
  if (makeCode == 0x33) { // Copy key
    if (!keyUp) { // Only on key press
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('c');
      delay(50);
      Keyboard.release('c');
      Keyboard.release(KEY_LEFT_GUI);
    }
    return;
  }
  
  if (makeCode == 0x49) { // Paste key
    if (!keyUp) { // Only on key press
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('v');
      delay(50);
      Keyboard.release('v');
      Keyboard.release(KEY_LEFT_GUI);
    }
    return;
  }
  
  if (makeCode == 0x61) { // Cut key
    if (!keyUp) { // Only on key press
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('x');
      delay(50);
      Keyboard.release('x');
      Keyboard.release(KEY_LEFT_GUI);
    }
    return;
  }
  
  if (makeCode == 0x5F) { // Find key
    if (!keyUp) { // Only on key press
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('f');
      delay(50);
      Keyboard.release('f');
      Keyboard.release(KEY_LEFT_GUI);
    }
    return;
  }
  
  if (makeCode == 0x16) { // Print Screen key
    if (!keyUp) { // Only on key press
      // Mac screenshot selection: Shift+Cmd+4
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('4');
      delay(50);
      Keyboard.release('4');
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_SHIFT);
    }
    return;
  }
  
  if (makeCode == 0x2D) { // Mute/Sound Off key
    if (!keyUp) { // Only on key press
      if (shift_pressed) {
        // Shift + Mute = Lock Screen (Ctrl+Cmd+Q on Mac)
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('q');
        delay(50);
        Keyboard.release('q');
        Keyboard.release(KEY_LEFT_GUI);
        Keyboard.release(KEY_LEFT_CTRL);
      }
      // For now, disable regular mute function to avoid issues
      // You can manually adjust volume or we can find a different solution
    }
    return;
  }
  
  if (makeCode == 0x02) { // Volume Down key
    if (!keyUp) { // Only on key press
      if (shift_pressed) {
        // Shift + Volume Down = Brightness Down (F1 on Mac)
        Keyboard.press(KEY_F1);
        delay(50);
        Keyboard.release(KEY_F1);
      }
      // Disable regular volume down for now to avoid Mission Control
      // Only brightness control (with Shift) works
    }
    return;
  }
  
  if (makeCode == 0x04) { // Volume Up key  
    if (!keyUp) { // Only on key press
      if (shift_pressed) {
        // Shift + Volume Up = Brightness Up (F2 on Mac)
        Keyboard.press(KEY_F2);
        delay(50);
        Keyboard.release(KEY_F2);
      }
      // Disable regular volume up for now
      // Only brightness control (with Shift) works
    }
    return;
  }
  
  // Handle Num Lock specially
  if (makeCode == 0x62) { // Num Lock
    if (!keyUp) { // Only on key press
      Keyboard.press(KEY_NUM_LOCK);
      delay(50);
      Keyboard.release(KEY_NUM_LOCK);
    }
    return;
  }
  
  // Handle modifier keys specially
  if (makeCode == 0x63 || makeCode == 0x6E) { // Left/Right Shift
    shift_pressed = !keyUp;  // Track shift state
    if (keyUp) {
      Keyboard.release(KEY_LEFT_SHIFT);
    } else {
      Keyboard.press(KEY_LEFT_SHIFT);
    }
    return;
  }
  
  if (makeCode == 0x4C) { // Control
    if (keyUp) {
      Keyboard.release(KEY_LEFT_CTRL);
    } else {
      Keyboard.press(KEY_LEFT_CTRL);
    }
    return;
  }
  
  if (makeCode == 0x13 || makeCode == 0x0D) { // Alt/Graph Alt
    if (keyUp) {
      Keyboard.release(KEY_LEFT_ALT);
    } else {
      Keyboard.press(KEY_LEFT_ALT);
    }
    return;
  }
  
  // Handle regular keys
  if (makeCode < 128) {
    uint8_t usbKey = sun_to_usb[makeCode];
    
    if (usbKey != 0) {
      if (keyUp) {
        Keyboard.release(usbKey);
      } else {
        Keyboard.press(usbKey);
      }
    }
  }
}