# Python Switcher

This firmware is a CircuitPython based switcher switcher.

It takes two switch inputs. One is the 'controller switch' this is passed through to the target device. The 'switcher switch' is used to change the where the controller switch is passed to.

The 'switcher switch' is debounced however the 'controller switch' is passed as is, so the target device can do its own processing.

## Tasks

- [x] Make the dot star work
- [x] Accept two inputs
- [x] Store the state of which output is currently selected
- [x] Allow for switching output
- [x] Make output user configurable
- [ ] Add some sounds
- [ ] Make the output actually output

## Setting up circuit python

1. [Download the CircuitPython firmware for the X80 here](https://circuitpython.org/board/itsybitsy_nrf52840_express)
2. [Follow the instructions to flash Circuit Python firmware to the device here](https://learn.adafruit.com/adafruit-itsybitsy-nrf52840-express/circuitpython)
3. Drag the contents of this folder (`python-switcher`) onto the CIRCUITPY device.
4. Done!

To develop the code use [`Code With Mu`](https://codewith.mu/). It will automatically connect to the device if its plugged in. You can set it up with VSCode but honestly its not worth the hassle

## Configuration

All of the X80 configuration is found in `x80_constants.py`. This creates a bunch of constants specific to the x80 like what switches are connected to what pins and what pull mode the switches are in. These shouldn't need to be changed between users.

The `user_constants.py` is where we can tweak the behaviour of the switcher. Change which ports we want to enable and the colors of the LED etc.

## Long term

What would be cool is if we could do some of this configuration without touching any code or any config files because that means a very small number of people can do it.

My thinking is that we could build a webpage that connects to the X80 using bluetooth and then can tweak the values currently stored in `user_constants.py`. This would be done in a nice user friendly way, so any one can change the settings of the device without knowing anything about how it actually works.

Even longer term, we could have just one circuit python firmware which connects via bluetooth and allows you to change the function (via a webpage) of the device without flashing the firmware. This would be super cool.

At some point it would be quite nice to have some kind of auto updater. You load the webpage connect to the X80 via bluetooth and it pushes any new functions and updates to the device. That way users can update themselves without having to flash the firmware.
