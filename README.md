# STM32 Traffic Signal Controller – Bare-Metal Embedded C

This project implements a traffic signal controller using the STM32F446RE microcontroller with bare-metal register-level programming. The system controls red, yellow, and green traffic lights, supports pedestrian button input, and displays a countdown timer on a 7-segment display.

## Features

* Bare-metal Embedded C programming
* Direct STM32 register-level GPIO configuration
* Traffic light sequence control
* Pedestrian button request handling
* Software debounce for button input
* 7-segment countdown display
* GPIO output control using BSRR register
* No HAL library used

## Hardware Used

* STM32F446RE Nucleo board
* Traffic light LED module
* Push button
* 7-segment display
* Jumper wires
* Breadboard

## Pin Configuration

| Component         | STM32 Pin |
| ----------------- | --------- |
| Red LED           | PA6       |
| Yellow LED        | PA7       |
| Green LED         | PA8       |
| Pedestrian Button | PC13      |
| 7-Segment Display | PB0–PB6   |

## How It Works

The system runs a normal traffic light cycle:

1. Green light turns ON for a fixed delay.
2. Yellow light turns ON for a fixed delay.
3. Red light turns ON for a fixed delay.

During green and yellow states, the program continuously scans the pedestrian button. If the button is pressed, the request is stored and handled after the yellow light.

When a pedestrian request is detected:

1. The system switches to red light.
2. A countdown from 9 to 0 is shown on the 7-segment display.
3. After the countdown, the display turns off.
4. The traffic signal returns to the normal cycle.

## Important Concepts Used

### GPIO Clock Enable

GPIOA, GPIOB, and GPIOC clocks are enabled using the RCC AHB1 peripheral clock register.

### GPIO Output Configuration

PA6, PA7, and PA8 are configured as output pins for the traffic lights.

PB0 to PB6 are configured as output pins for the 7-segment display.

### Button Input

PC13 is configured as an input pin for the pedestrian button.

### BSRR Register

The GPIO BSRR register is used to set and reset output pins safely without affecting other pins.

### Button Debouncing

A software debounce delay is added to avoid false triggering caused by mechanical button bouncing.

## Project Code Highlights

* `red_on()` turns on the red LED and turns off yellow and green.
* `yellow_on()` turns on the yellow LED and turns off red and green.
* `green_on()` turns on the green LED and turns off red and yellow.
* `button_pressed_once()` detects a single valid button press.
* `wait_and_scan_button()` keeps checking the pedestrian button during traffic light delays.
* `pedestrian_crossing()` handles the pedestrian countdown sequence.

## Skills Demonstrated

* Embedded C programming
* STM32 bare-metal development
* Register-level GPIO configuration
* Bit manipulation
* 7-segment display interfacing
* Button input handling
* Software debouncing
* Real-time embedded logic
* Hardware debugging

## Development Environment

* Microcontroller: STM32F446RE
* IDE: Keil uVision / STM32CubeIDE
* Language: Embedded C
* Programming Method: Bare-metal register-level programming

## Future Improvements

* Add timer interrupt instead of software delay
* Add external interrupt for pedestrian button
* Add UART debug messages
* Add buzzer for pedestrian crossing
* Use FreeRTOS tasks for traffic light and pedestrian handling
* Add multiple traffic directions


https://github.com/user-attachments/assets/531fc0eb-c051-43bf-8ab0-bfd0fe0f5db5


