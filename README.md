# GAMEPOD

GAMEPOD is a DIY handheld gaming console built with an Arduino UNO and a 128×64 OLED display.

The project allows you to run games on a small screen using physical buttons. You can also create your own games using the GAMEPOD API.

## Hardware Requirements

- Arduino UNO (recommended)
- 128×64 I2C OLED display
- 4 push buttons
- Buzzer
- Jumper wires
- 220 Ω resistor, if required by your buzzer

## Wiring

### Buttons

Connect one pin of each button to **GND** and the other pin to the corresponding Arduino pin.

| Button | Arduino pin |
|---|---|
| Left | D2 |
| OK / Middle | D3 |
| Right | D4 |
| Home | D5 |

### Buzzer

The buzzer signal pin connects to **D6**. Connect its other terminal to **GND**, following the buzzer's specifications.

Do not connect an ordinary speaker directly to an Arduino pin without a suitable driver circuit.

### OLED Display

Connect the display to the Arduino UNO:

| OLED pin | Arduino pin |
|---|---|
| GND | GND |
| VCC | 5V (if supported by the module) |
| SCL | A5 |
| SDA | A4 |

## Uploading the Code

1. Download or clone this repository.
2. Install the required Arduino libraries.
3. Follow the project's build instructions to compile the code.
4. Connect the Arduino UNO to your computer using USB.
5. Upload the compiled firmware to the board.

Make sure your wiring matches the pin configuration used by the code before powering on the device.
