import board
import digitalio
import time
import adafruit_dotstar
import adafruit_ble
from adafruit_debouncer import Debouncer
from x80_constants import *
from user_constants import *
from buzzer import Buzzer
from microcontroller import watchdog as watchdoggo
from watchdog import WatchDogMode
from pulser import Pulser
from adafruit_ble.services.standard.hid import HIDService
from adafruit_ble.services.standard.device_info import DeviceInfoService
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement
from adafruit_ble.advertising import Advertisement
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keyboard_layout_us import KeyboardLayoutUS

watchdoggo.timeout = 2.5
watchdoggo.mode = WatchDogMode.RAISE

# Disable passthrough
passthrough = digitalio.DigitalInOut(board.D12)
passthrough.direction = digitalio.Direction.OUTPUT
passthrough.value = True

# Input Switch
inputSwitchPin = digitalio.DigitalInOut(INPUT_SWITCH)
inputSwitchPin.direction = digitalio.Direction.INPUT
inputSwitchPin.pull = PULL_MODES[INPUT_SWITCH]
inputSwitch = Debouncer(inputSwitchPin)

# Pulser
pulser = Pulser()

# Connect to dotstar
dotstars = adafruit_dotstar.DotStar(
    DOTSTAR_CLOCK, DOTSTAR_DATA, 1, brightness=DOTSTAR_BRIGHTNESS
)

# Connect buzzer
buzzer = Buzzer('Buzzer', BUZZER_PIN)

# Initialise state
# TODO: One day it would be nice to sync this with non-volatile memory
selectedOutputCount = 0
switchPressedAt = 0
isSwitchPressed = False
tuneTriggered = False
startupTunePlayed = False

# Setup bluetooth stuff, even if we don't use it we can still define all the variables
hid = HIDService()
device_info = DeviceInfoService(software_revision=adafruit_ble.__version__, manufacturer="Ace Centre")
advertisement = ProvideServicesAdvertisement(hid)
advertisement.appearance = 961
scan_response = Advertisement()
scan_response.complete_name = BLUETOOTH_DEVICE_NAME

ble = adafruit_ble.BLERadio()
keyboard = Keyboard(hid.devices)
keyboardLayout = KeyboardLayoutUS(keyboard)
if(BLUETOOTH_DEVICE_IN_USE is True):
    if not ble.connected:
        print("Advertising bluetooth connection")
        ble.start_advertising(advertisement, scan_response)
    else:
        print("Already connected to bluetooth device")

while True:
    watchdoggo.feed()
    inputSwitch.update() # Update debouncer
    buzzer.update() # Update buzzer
    pulser.update() # Update pulser

    if startupTunePlayed is False:
        buzzer.play(STARTUP_TUNE)
        startupTunePlayed = True

    if(BLUETOOTH_DEVICE_IN_USE is True and ble.connected is False and ble.advertising is False):
        ble.start_advertising(advertisement, scan_response)
        print('Bluetooth is not connected and not advertising')


    # Get the current output. Using the modulo operator means we can just
    # endlessly add to the output count and it will wrap around
    wrappedOutputCount = selectedOutputCount % len(OUTPUTS)
    selectedOutput = OUTPUTS[wrappedOutputCount]

    if(selectedOutput.type == 'Bluetooth'):
        selectedOutput.keyboard = keyboard
        selectedOutput.keyboardLayout = keyboardLayout

    # Default the dotstar color to represent what is currently selected
    newDotColor = selectedOutput.selectedColor

    # Change the color of the dotstar if a switch is being pressed down
    if inputSwitch.value == False:
        newDotColor = INPUT_SWITCH_COLOR

    # First time loop ran when switch is pressed, then store the moment it was pressed
    if inputSwitch.value is False and isSwitchPressed is False:
        switchPressedAt = time.monotonic_ns()
        print('first press started at', switchPressedAt)
    # Play a tune if the output change has been held down for long enough to switch
    # Note, the switch wont actually happen until you lift the switchç
    elif inputSwitch.value is False and isSwitchPressed is True:
        switchHeldFor = time.monotonic_ns() - switchPressedAt
        if switchHeldFor > MINIMUM_PRESS_TIME_TO_SWITCH and tuneTriggered is False:
            print('Play a tune here')

            # Calculate the next output and play its tune
            nextOutput = OUTPUTS[(selectedOutputCount + 1) % len(OUTPUTS)]
            buzzer.play(nextOutput.tune)
            tuneTriggered = True



    # Switch is lifted after being down
    # Then check if it was held down for long enough
    if inputSwitch.value is True and isSwitchPressed is True:
        switchHeldFor = time.monotonic_ns() - switchPressedAt

        print('Switch lifted after being held for:', switchHeldFor)

        # Make sure the switch was held for long enough before switching output
        if switchHeldFor > MINIMUM_PRESS_TIME_TO_SWITCH:
            print('Output changed')
            print()
            selectedOutputCount = selectedOutputCount + 1
            tuneTriggered = False
        else:
            print('Send Pulse')
            pulser.sendPulse(selectedOutput)


    # Store the current state of the switcher
    isSwitchPressed = inputSwitch.value is False

    # Set the color of the LED
    dotstars[0] = newDotColor
