# X80

X80 is an Arduino-based multi-functional input/output device designed to be an interface between assistive technology inputs such as accessibility switches or specilist sensors and mainstream technology outputs, such as to a mobile phone or Augmentative and Alternative Communication (AAC) device.

An X80 can be preprogrammed and repurposed to perform a different function. It's built around the Adafruit Feather, generally the nrf82540 Express. It's essentially a Feather Wing for Assistive Technology (AT).

The PCB is still a work in progress but it is designed to have:

* Switch inputs via 3.5mm jack
* switch outputs via 3.5mm jack (simulates a switch press as an input into an Assistive Technology device)
* DB9 connector designed to accomodate devices meeting the TRACE SET Standard often used with accessibility input devices (https://park.org/Guests/Trace/pavilion/setdoc1.htm)
* buzzer
* potentiometer
* on/off switch

There are a few differences between the prototype versions:

## Version 4

The funtionality has been split into two boards:

* X80 Standard (designed for Adafruit ItsyBitsy nrf52840 https://www.adafruit.com/product/4481)
* X80 Power (required for LIPO battery as this board includes the chargin circuit)

### X80 Standard

* Majority surface mount components
* 4 switch inputs (5 with splitter cable on one port)
* 4 switch outputs
* One switch 'pass through' (one switch input will still control on switch output without power)
* buzzer
* setting potentiometer
* header for connecting X80 Power or wiring direct to board)

### X80 Power

* JST connector for LIPO battery
* DB9 connector for connecting external peripherals
* will allow space in enclosure for LIPO battery

## Version 3

* Majority surface mount components
* 4 switch inputs (5 with splitter cable on one port)
* 4 switch outputs
* One switch 'pass through' (one switch input will still control on switch output without power)
* buzzer
* on-board JST connector
* setting potentiometer
* on/off switch

We removed the DB9 connector to save space.

## Version 2

* 3 switch inputs
* 2 switch outputs
* user button
* DB9 connector
* on/off switch
* setting potentiometer
* on-board JST connector

## Version 1

* 1 switch input
* DB9 connector
* 2 setting poteniometers
* on/off switch

This is very much a work in progress! There are issues with the PCB right now we're working on.
