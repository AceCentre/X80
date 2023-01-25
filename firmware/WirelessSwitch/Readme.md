# Wireless Switch

## What does this do?

You have a nrf52840 board that sends and a nrf52840 board that receives. We are demonstrating here how to do wireless switches with LOW POWER. 

## How?

Most wireless switches either use Radio (e.g. LoRa) or BLE HID to operate. The problem with BLE HID is that its power hungry and for switch users it cant really sleep. So this technique has a advertisement (advertisement) beacon which starts advertisement on a switch press. The receiving code (central) is searching for a MAC address beacon and seeing it come on/off.  The central code then sends a HID space over USB. 

We aim to give example code to receive this BLE central code so you can do this without a dongle. 

## Roadmap

[] Get video demo working
[] Add in low power features
[] Adapt for x80 
[] Write demo code for PC 
[] Create docs page on Ace site

## Licence

MIT

## Credits 

Will Wade, With a big nod to Prente Romiche/Saltillo and their aeroswitch. 
