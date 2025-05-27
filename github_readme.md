# Sun Type 5 USB Keyboard Adapter

Convert your vintage Sun Type 5 keyboard into a modern USB HID keyboard with full Mac compatibility and special function key support.

## Overview

This project transforms a Sun Microsystems Type 5 keyboard into a fully functional USB keyboard using a SparkFun Pro Micro (Arduino-compatible board). The adapter handles the Sun keyboard's unique negative logic serial protocol and maps all keys to modern USB HID equivalents.

## Features

### Complete Key Mapping
- All standard keys: Letters, numbers, symbols, function keys
- Arrow keys: Both main keyboard and numeric keypad arrows
- Modifier keys: Shift, Ctrl, Alt with proper state tracking
- Special keys: Caps Lock (toggle), Num Lock, Print Screen

### Mac-Optimized Functions
- CMD keys: Sun's diamond/meta keys → Mac CMD key
- Copy/Paste/Cut: Direct Sun keys → CMD+C/V/X
- Find: Sun Find key → CMD+F
- Screenshot: Sun Print Screen → Shift+CMD+4 (Mac selection tool)

### Brightness & System Controls
- Brightness Up: Shift + Volume Up → F2
- Brightness Down: Shift + Volume Down → F1
- Lock Screen: Shift + Mute → Ctrl+CMD+Q

### Robust Communication
- Manual serial decoding with negative logic support
- Polling-based protocol for reliable key detection
- Bit-level timing control at 1200 baud (833μs per bit)

## Hardware Requirements

### SparkFun Pro Micro
- Model: SparkFun Pro Micro 5V/16MHz
- Microcontroller: ATmega32U4 with native USB HID support
- Why this board: USB HID capability + precise timing control

### Sun Type 5 Keyboard
- Compatible models: Sun Type 5, Type 5c (International layout)
- Connector: 8-pin mini-DIN
- Protocol: 1200 baud, 8N1, negative logic

### Wiring Diagram

```
Sun Keyboard (8-pin mini-DIN) → SparkFun Pro Micro
├── Pin 2 (White - GND)       → GND
├── Pin 5 (Green - TX)        → Pin 3 (optional, for future LED control)
├── Pin 6 (Yellow - RX)       → Pin 2 (Arduino RX from keyboard)
└── Pin 8 (Red - +5V)         → VCC (+5V)
```

Note: You only need to connect 3 wires (GND, +5V, RX) for basic functionality.

## Software Installation

### 1. Arduino IDE Setup
```bash
# Install Arduino IDE
# Add SparkFun board support:
# File → Preferences → Additional Board Manager URLs:
https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json

# Tools → Board Manager → Search "SparkFun" → Install
```

### 2. Upload Code
1. Disconnect keyboard during programming (pins 0/1 used for USB)
2. Select Board: "SparkFun Pro Micro"
3. Select Processor: "ATmega32U4 (5V, 16MHz)"
4. Upload the provided sketch
5. Reconnect keyboard after upload

## Key Mappings

### Standard Keys
| Sun Key | USB Output | Notes |
|---------|------------|-------|
| A-Z, 0-9 | A-Z, 0-9 | Direct mapping |
| F1-F12 | F1-F12 | Function keys |
| Arrow Keys | Arrow Keys | Both main and keypad |
| Space, Enter, Tab | Space, Enter, Tab | Standard keys |
| Shift, Ctrl, Alt | Shift, Ctrl, Alt | Modifier keys |

### Special Sun Keys
| Sun Key | USB Output | Function |
|---------|------------|----------|
| Diamond Keys (L/R) | CMD | Mac command key |
| Copy | CMD+C | Copy to clipboard |
| Paste | CMD+V | Paste from clipboard |
| Cut | CMD+X | Cut to clipboard |
| Find | CMD+F | Find/search |
| Print Screen | Shift+CMD+4 | Mac screenshot selection |
| Caps Lock | Caps Lock | Toggle functionality |

### System Controls
| Key Combination | Function | Mac Shortcut |
|-----------------|----------|--------------|
| Shift + Volume Up | Brightness Up | F2 |
| Shift + Volume Down | Brightness Down | F1 |
| Shift + Mute | Lock Screen | Ctrl+CMD+Q |

## Technical Details

### Sun Keyboard Protocol
- **Baud Rate**: 1200 bps
- **Format**: 1 start bit, 8 data bits, 1 stop bit, no parity
- **Logic**: Negative logic (inverted signal levels)
- **Communication**: Asynchronous serial, full duplex

### Scan Code Format
- **Make Codes**: 0x00-0x7F (key press)
- **Break Codes**: 0x80-0xFF (key release, make code + 0x80)
- **Special Codes**: 0x7F (idle), 0xFF (reset response)

### Arduino Implementation
```cpp
// Manual bit-level serial reading with negative logic
delayMicroseconds(BIT_DURATION_US);
int bitValue = !digitalRead(RX_PIN);  // Invert for negative logic

// Scan code processing
boolean keyUp = (scancode & 0x80) != 0;
uint8_t makeCode = scancode & 0x7F;
```

## Troubleshooting

### Keyboard Not Detected
1. Check wiring: Ensure correct pin connections
2. Power supply: Verify +5V connection
3. Upload process: Disconnect keyboard during programming

### Keys Not Working
1. Scan codes: Enable debug mode to see raw scan codes
2. Timing: Verify 1200 baud timing (833μs per bit)
3. Logic levels: Confirm negative logic inversion

### Mac-Specific Issues
1. CMD key: Ensure diamond keys are mapped to GUI
2. Function keys: Check System Preferences → Keyboard
3. Shortcuts: Verify Mac keyboard shortcuts are enabled

## Project History & Credits

This project builds upon excellent prior work in the Sun keyboard adapter community:

### Original Inspiration
- [benr/SunType5_ArduinoAdapter](https://github.com/benr/SunType5_ArduinoAdapter) by Ben Rockwood
  - Initial Arduino-based Sun keyboard adapter
  - SoftwareSerial implementation
  - Basic USB HID functionality

- [DirtBagXon/sunkbd](https://github.com/DirtBagXon/sunkbd) 
  - Comprehensive Sun keyboard documentation
  - Protocol analysis and scan code tables
  - Multiple implementation approaches

### Key Improvements in This Version
- **Manual bit-level decoding**: More reliable than SoftwareSerial
- **Complete key mapping**: Full International scan set support
- **Mac optimization**: Native Mac shortcuts and functions
- **Robust polling protocol**: Precise timing control for negative logic
- **Special functions**: Brightness, screenshot, lock screen

### Sun Keyboard Documentation

#### Official Sun References
- Sun Type 5 Keyboard Manual: Protocol specification
- Scan Code Tables: International layout mapping
- Hardware Interface: Mini-DIN pinout and electrical specs

### Protocol Commands
| Command | Code | Function |
|---------|------|----------|
| Reset | 0x01 | Keyboard self-test |
| Bell On/Off | 0x02/0x03 | Speaker control |
| Click On/Off | 0x0A/0x0B | Key click sound |
| LED Control | 0x0E + data | Caps/Num/Scroll Lock LEDs |

## Contributing

Contributions welcome! Areas for improvement:

- LED feedback: Implement full LED control
- Layout variants: Support for other Sun keyboard models  
- Media keys: Better volume control implementation
- Windows support: Windows-specific key mappings

## License

This project is open source. Please credit original authors and contribute improvements back to the community.

## Hardware Suppliers

### SparkFun Pro Micro
- SparkFun: [Official Pro Micro](https://www.sparkfun.com/products/12640)
- Alternatives: Any ATmega32U4-based board with USB HID support

### Sun Keyboards
- eBay: Search "Sun Type 5 keyboard"
- Vintage computer forums: Often available from collectors
- Corporate surplus: Sometimes available from IT liquidations

---

**Enjoy your vintage Sun keyboard with modern functionality!**