"""
Whatever state the iPad is in this code will wake the device up
Attach a button to Feather nRF52840. Set the pin on line 21.
WARNING 1: DO NOT HAVE A PASSCODE FOR IPAD!
Change line 58 to the app of your choice. 
"""
import time
import board
from digitalio import DigitalInOut, Direction, Pull

import adafruit_ble
from adafruit_ble.advertising import Advertisement
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement
from adafruit_ble.services.standard.hid import HIDService
from adafruit_ble.services.standard.device_info import DeviceInfoService
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keyboard_layout_us import KeyboardLayoutUS
from adafruit_hid.keycode import Keycode

button_1 = DigitalInOut(board.D5)
button_1.direction = Direction.INPUT

hid = HIDService()

device_info = DeviceInfoService(
    software_revision=adafruit_ble.__version__, manufacturer="Ace Centre"
)
advertisement = ProvideServicesAdvertisement(hid)
advertisement.appearance = 961
scan_response = Advertisement()
scan_response.complete_name = "AceWakeButton"

ble = adafruit_ble.BLERadio()
if not ble.connected:
    print("advertising")
    ble.start_advertising(advertisement, scan_response)
else:
    print("already connected")
    print(ble.connections)

k = Keyboard(hid.devices)
kl = KeyboardLayoutUS(k)
while True:
    while not ble.connected:
        pass
    print("Start typing:")

    while ble.connected:
        if button_1.value is False:
            k.press(Keycode.COMMAND, Keycode.H)
            k.release_all()
            k.press(Keycode.COMMAND, Keycode.SPACE)
            k.release_all()
            k.press(Keycode.COMMAND, Keycode.A)
            k.release_all()
            k.send(Keycode.DELETE)
            time.sleep(0.2)
            kl.write("Photos")
            time.sleep(0.1)
            k.send(Keycode.ENTER)
            time.sleep(0.4)
    ble.start_advertising(advertisement)
