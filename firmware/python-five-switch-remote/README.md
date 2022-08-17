# Five Switch Remote

This firmware is a CircuitPython based remote control with five switches.

The centre switch should be plugged into the switch port marked '0' on the X80. The other switches should be plugged into the other jacks and (with a splitter in the port marked '3+').

The four direction arrows will map to a keypress on the coresponding arrow key. You can click and hold for a longer keypress, the press is passed directly through.

The centre key has three options:

- Short press - Will press space
- Medium press - Will press CMD+H
- Long press - Will press EJECT

The timings and keys can all be configured in the `user_constants.py` file.

The x80 will pulse with a blue light when it is in 'pairing' mode.

## Setting up circuit python

1. [Download the CircuitPython firmware for the X80 here](https://circuitpython.org/board/itsybitsy_nrf52840_express)
2. [Follow the instructions to flash Circuit Python firmware to the device here](https://learn.adafruit.com/adafruit-itsybitsy-nrf52840-express/circuitpython)
3. Drag the contents of this folder (`python-switcher`) onto the CIRCUITPY device.
4. Done!

To develop the code use [`Code With Mu`](https://codewith.mu/). It will automatically connect to the device if its plugged in. You can set it up with VSCode but honestly its not worth the hassle

## Configuration

All of the X80 configuration is found in `x80_constants.py`. This creates a bunch of constants specific to the x80 like what switches are connected to what pins and what pull mode the switches are in. These shouldn't need to be changed between users.

The `user_constants.py` is where we can tweak the behaviour of the switcher. Change which ports we want to enable and the colors of the LED etc.
