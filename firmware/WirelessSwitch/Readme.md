# Wireless Switch

## What does this do?

You have a nrf52840 board that sends and a nrf52840 board that receives. We are demonstrating here how to do wireless switches with LOW POWER. 

## How?

Most wireless switches either use Radio (e.g. LoRa) or BLE HID to operate. The problem with BLE HID is that its power hungry and for switch users it cant really sleep. So this technique has a advertisement (advertisement) beacon which starts advertisement on a switch press. The receiving code (central) is searching for a MAC address beacon and seeing it come on/off.  The central code then sends a HID space over USB. 

We aim to give example code to receive this BLE central code so you can do this without a dongle. 

## Details

To try thus out get two nrf52840 feather express boards from adafruit. Then load up the firmware for each board. 

The firmware for the switch sending part is called *advertisement*. You need to add two switches -  one for the actual switch (use a switch Jack) and a tactile swich for pairing. 

The firmware for the recievfing part is known as the *central* firmware. There should be defined a led for indicating pairing. By default it's the red led of a nrf52840 Feather express.

When pairing button is pressed on tits he advertisement board then the board sends specific pairing package and dongle receives this package and MAC address of the board it will listen for. The MAC is saved in flash memory and can be read later on start of dongle code

## Roadmap

- [x] Get basic code working
- [x] Add in pairing functionality by bringing close to central device
- [ ] Get video demo working
- [ ] Add in low power features
- [ ] Adapt for x80 
- [ ] Write demo code for PC 
- [ ] Create docs page on Ace site
- [ ] Add ability to listen for more than one mac address (more than one switch)


## Licence

MIT

## Credits 

Will Wade, With a big nod to Prente Romiche/Saltillo and their aeroswitch. Note  - I have no idea if there's does exactly the same as this but a big clue was the fact they called it "Broadcast mode" 
