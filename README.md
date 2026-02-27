# ESP32 16x16 LED Matrix Display

A versatile, WiFi-enabled 16x16 LED matrix display built with ESP32 and WS2812B programmable RGB LEDs. Features interactive games, dynamic patterns, clock display, and a web-based control interface.

## Features

### Display Modes
- **Display Mode**: Showcase beautiful animations and patterns
- **Clock Mode**: Display current time with NTP synchronization
- **Games Mode**: Play interactive games directly on the matrix
- **Robot Mode**: Display robot facial expressions

### Animation Patterns
- 📝 **Scrolling Text**: Customizable scrolling text with color options
- 🌧️ **Rainbow Rain**: Animated rain with rainbow colors
- 🧱 **Tetris Cascade**: Block-falling animation
- 🌈 **Aurora**: Aurora borealis effect
- 🔥 **Fire**: Realistic fire animation
- ✨ **Glitter**: Sparkling glitter effects
- 🎆 **Fireworks**: Animated firework displays
- 🎨 **Drawing Mode**: Create custom pixel art
- And more!

### Interactive Games
- 🐍 **Snake**: Classic snake game with food collection
- 🐦 **Flappy Bird**: Side-scrolling obstacle avoidance
- 🧱 **Breakout**: Brick-breaking paddle game
- 🚀 **Space Invaders**: Shoot descending enemies
- 🦖 **Dino Run**: Dodge obstacles while running

### Web UI Controls
- **Brightness Control**: Adjustable LED brightness
- **Speed Control**: Adjust animation and game speed
- **Pattern Selection**: Choose from various display patterns
- **Text Configuration**: Set custom scrolling text and colors
- **Game Controls**: Virtual joystick for game input
- **Mode Switching**: Easy transition between display modes

## Hardware Requirements

### Components
- **ESP32 Development Board** (with WiFi capability)
- **256 WS2812B RGB LEDs** (NeoPixel compatible)
- **16x16 LED Matrix Grid** (or addressable LED strips arranged in grid)
- **5V Power Supply** (10A+ recommended)
- **Jumper Wires** (connecting ESP32 to LED data/power)
- **3D Printed Enclosure** (optional - files included)

### Pinout
- **LED Data Pin**: GPIO 19
- **Power Supply**: 5V (from external PSU)
- **Ground**: Common GND

### 3D Printed Parts
The following 3D models are included:
- `back part.stl` - Rear enclosure panel
- `Grid_cover.stl` - Front LED grid cover
- `middle part.stl` - Middle support structure

## Software Requirements

- **Arduino IDE** or **PlatformIO**
- **ESP32 Board Support** (via Arduino Board Manager)
- **Libraries**:
  - Adafruit_NeoPixel
  - WiFiManager
  - WebServer (built-in)
  - Preferences (built-in)

## Installation

### 1. Install Arduino IDE
Download and install from [arduino.cc](https://www.arduino.cc/en/software)

### 2. Add ESP32 Board Support
1. Open Arduino IDE → File → Preferences
2. Add to "Additional Board Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Go to Tools → Board Manager
4. Search for "esp32" and install "esp32 by Espressif"

### 3. Install Required Libraries
In Arduino IDE, go to Sketch → Include Library → Manage Libraries and search for:
- `Adafruit NeoPixel`
- `WiFiManager`

### 4. Upload Firmware
1. Clone or download this repository
2. Open `firmware/led_matrix_1616/led_matrix_1616.ino` in Arduino IDE
3. Select Board: **ESP32 Dev Module** (or your specific board)
4. Select the correct COM port
5. Click Upload

### 5. WiFi Setup
On first boot:
1. Look for WiFi network `LED-MATRIX-SETUP`
2. Connect to it (no password)
3. Open browser to `192.168.4.1`
4. Select your WiFi network and enter password
5. Device will connect to your home WiFi

## Usage

### Accessing the Web Interface
1. Once connected to WiFi, open **Serial Monitor** (9600 baud) to see the device IP
2. Enter the IP address in your browser (e.g., `http://192.168.1.100`)
3. The web UI will load with all controls

### Web UI Controls

#### Mode Buttons
- **Display**: Show patterns and custom content
- **Clock**: Display current time
- **Games**: Play interactive games
- **Robot**: Show robot facial expressions

#### Display Mode
- Select from multiple animation patterns
- Control scrolling text
- Customize text color
- Enable random color cycling
- Adjust animation speed

#### Clock Mode
- Displays current time with NTP synchronization
- Time is set using India Standard Time (IST UTC+5:30)
- Clock updates automatically every frame

#### Games Mode
- Select game using dropdown menu
- Use virtual joystick for controls
- View game status and score
- Game ends on collision/failure

#### Settings
- **Brightness**: 10-150 range
- **Speed**: 0-100 range
- **Persistence**: Settings are saved and restored on reboot

## File Structure

```
led_matrix_1616/
├── firmware/
│   └── led_matrix_1616/
│       ├── led_matrix_1616.ino    # Main sketch
│       ├── Constants.h             # Global constants and declarations
│       ├── Utils.h                 # Utility functions (LED mapping, character drawing)
│       ├── Patterns.h              # Animation patterns
│       ├── Games.h                 # Interactive games logic
│       ├── Clock.h                 # Clock display functionality
│       ├── WebUI.h                 # Web interface and HTTP handlers
│       └── Robot.h                 # Robot expression mode
├── 3d_files/
│   ├── back part.stl
│   ├── Grid_cover.stl
│   └── middle part.stl
├── circuit/                         # Circuit diagrams (TBD)
├── LICENSE                          # GNU General Public License v3
└── README.md                        # This file
```

## Project Structure

### Main Components

**led_matrix_1616.ino**
- Central Arduino sketch
- WiFi and web server initialization
- Main event loop handling mode switching
- Preference/settings management

**Constants.h**
- Global variable declarations
- LED configuration (256 LEDs, 16x16 grid)
- Matrix dimensions and font specs

**Utils.h**
- LED indexing and physical mapping
- Character drawing functions
- Color wheel utility
- Display helper functions

**Patterns.h**
- 10+ animation patterns
- Scrolling text implementation
- Procedural generation algorithms

**Games.h**
- Snake game logic
- Flappy Bird physics
- Breakout collision detection
- Space Invaders gameplay
- Dino Run mechanics

**Clock.h**
- Time display formatting
- NTP synchronization
- 7-segment style digit drawing

**WebUI.h**
- HTML/CSS/JavaScript interface
- HTTP endpoint handlers
- Real-time control interface

**Robot.h**
- Robot face expression engine
- Eye and mouth drawing
- Emotion states

## Building for Production

### Power Considerations
- WS2812B LEDs draw ~60mA per LED at full white brightness
- 256 LEDs at full brightness = ~15.3A
- Use external 5V PSU rated for 10A+
- Never power from USB alone
- Add capacitor (470µF) across power rails

### WiFi Range
- Use 2.4GHz WiFi for best range
- Ensure clear line of sight to router
- Consider adding external antenna if needed

### Heat Management
- Ensure adequate ventilation in enclosure
- Consider heatsinks for power delivery circuit
- Monitor temperature during active use

## Troubleshooting

### LEDs Not Lighting
- Check data pin connection (GPIO 19)
- Verify 5V power supply connected
- Check LED strip polarity (DIN/DOUT direction)
- Restart ESP32

### WiFi Connection Issues
- Verify 2.4GHz WiFi network available
- Try resetting WiFi by erasing preferences
- Check WiFi credentials in captive portal

### Web Interface Not Loading
- Ensure device on same network
- Check IP address in Serial Monitor
- Try clearing browser cache
- Restart ESP32

### Games Not Responding
- Check touchscreen/joystick calibration
- Verify game control speed setting
- Restart device

## Contributing

Contributions are welcome! Please feel free to:
- Report bugs and issues
- Suggest new patterns or games
- Improve documentation
- Optimize code performance

## License

This project is licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Built with **Adafruit NeoPixel** library
- WiFi management with **WiFiManager**
- ESP32 platform by Espressif Systems

## Author

**Pradum Pal**

---

*Happy animating! 🎨✨*
