# Wireless Switch

## What does this do?

You have a nrf52840 board that sends and a nrf52840 board that receives. We are demonstrating here how to do wireless switches with LOW POWER. 

## How?

Most wireless switches either use Radio (e.g. LoRa) or BLE HID to operate. The problem with BLE HID is that its power hungry and for switch users it cant really sleep. So this technique has a advertisement (advertisement) beacon which starts advertisement on a switch press. The receiving code (central) is searching for a MAC address beacon and seeing it come on/off.  The central code/dongle then sends a HID space over USB. 

On the advertisement (switch sending) board:
- When device sleeps and key button pressed first time, board wakes up. Once the LED lights the board is ready and you can start pairing or transmitting button data.
- When switch is pressed again board starts collecting button data into packages and advertise them.
- It then sends out packages. These data packages are changed every 100ms. Each package has recorded 10 buttons states with interval 10 ms(which in total 100ms).
- After some time of inactivity (5sec default) it will go back to sleep.

On the central (dongle) side:
- It scans BLE devices and for the MAC address device it receives package which has recorded button states over 100ms and then replays these states via USB keyboard.


## Details

To try this out get two [nrf52840 feather express boards](https://www.adafruit.com/product/4062) from adafruit. Then load up the firmware for each board (its a arduino sketch - you want to first follow the guide [here](https://learn.adafruit.com/introducing-the-adafruit-nrf52840-feather/arduino-bsp-setup)). 

The firmware for the switch sending part is called *advertisement*. You need to add two switches -  one for the actual switch (use a switch jack) and a tactile swich for pairing. 

The firmware for the recievfing part is known as the *central* firmware. There should be defined a LED for indicating pairing. By default it's the red led of a nrf52840 Feather express.

When pairing button is pressed on the advertisement board it will send a specific pairing package and dongle receives this package and MAC address of the board it will listen for. The MAC is saved in flash memory and can be read later on start of dongle code.

Note - to save battery the advertisement board is set to sleep. Pressing the switch will wake it up - all in all this takes around 1second. 

## Battery life estimates


When pressing key buttton and transmitting data, it consumes around 1000 uA.
In sleep mode it consumes 20 uA.
Good coin cell battery can have around 200 mAh.
So 200mAh/20uA = 10000 hrs in deep sleep mode.
10000hrs > 416 days > a bit more than a year.

But in general cheap batteries will have less capacity.

Consumption can also vary on different boads. My measurements were made with nrf52840 dongle.


## Roadmap

- [x] Get basic code working
- [x] Add in pairing functionality
- [ ] Get video demo working
- [x] Add in low power features
- [ ] Adapt for x80 
- [x] Write demo code for PC 
- [ ] Create docs page on Ace site
- [ ] Add ability to listen for more than one switch


## Licence

MIT

## Credits 

Will Wade, With a big nod to Prente Romiche/Saltillo and their aeroswitch. Note  - I have no idea if there's does exactly the same as this but a big clue was the fact they called it "Broadcast mode" 
