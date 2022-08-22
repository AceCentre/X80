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
from switch import Switch
from pulsing_dotstar import PulsingDotstar
from adafruit_hid.consumer_control import ConsumerControl
from adafruit_hid.consumer_control_code import ConsumerControlCode

# Watchdog will save if you crash
watchdoggo.timeout = 10
watchdoggo.mode = WatchDogMode.RAISE

# Disable passthrough
passthrough = digitalio.DigitalInOut(board.D12)
passthrough.direction = digitalio.Direction.OUTPUT
passthrough.value = True

# Setup switches
centreSwitchPin = digitalio.DigitalInOut(CENTRE_BUTTON_PIN)
centreSwitchPin.direction = digitalio.Direction.INPUT
centreSwitchPin.pull = PULL_MODES[CENTRE_BUTTON_PIN]
centre = Debouncer(centreSwitchPin)
left = Switch(LEFT_BUTTON_PIN)
right = Switch(RIGHT_BUTTON_PIN)
up = Switch(UP_BUTTON_PIN)
down = Switch(DOWN_BUTTON_PIN)

# Connect to dotstar
dotstars = adafruit_dotstar.DotStar(
    DOTSTAR_CLOCK, DOTSTAR_DATA, 1, brightness=DOTSTAR_BRIGHTNESS
)

# Setup buzzer
buzzer = Buzzer('Buzzer', BUZZER_PIN)
pulsingDotstar = PulsingDotstar()

# Setup bluetooth stuff
hid = HIDService()
device_info = DeviceInfoService(software_revision=adafruit_ble.__version__, manufacturer="Ace Centre")
advertisement = ProvideServicesAdvertisement(hid)
advertisement.appearance = 961
scan_response = Advertisement()
scan_response.complete_name = BLUETOOTH_DEVICE_NAME
ble = adafruit_ble.BLERadio()
keyboard = Keyboard(hid.devices)
consumerControl = ConsumerControl(hid.devices)
keyboardLayout = KeyboardLayoutUS(keyboard)
if not ble.connected:
    print("Advertising bluetooth connection")
    ble.start_advertising(advertisement, scan_response)
else:
    print("Already connected to bluetooth device")

# Setup initial state
isCentreHeld = False
centreHeldAt = 0
playedFirstBeep = False
playedSecondBeep = False
startupTunePlayed = False

while True:
    # Run updates
    pulsingDotstar.update()
    centre.update()
    buzzer.update()
    watchdoggo.feed()

    newDotColor = (0,0,0)

    # Play the start up tune
    if startupTunePlayed is False:
        buzzer.play(STARTUP_TUNE)
        startupTunePlayed = True

    # If we aren't connected via bluetooth the advertise
    if(ble.connected is False and ble.advertising is False):
        ble.start_advertising(advertisement, scan_response)
        print('Bluetooth is not connected and not advertising')

    # Record the time that the centre button begins being held
    if(centre.value is False and isCentreHeld is False):
        isCentreHeld = True
        centreHeldAt = time.monotonic_ns()

    # The centre key is released so we need to see how long for
    if(centre.value is True and isCentreHeld is True):
        isCentreHeld = False
        playedFirstBeep = False
        playedSecondBeep = False
        currentTime = time.monotonic_ns()
        heldFor = (currentTime - centreHeldAt) / 1000000

        # If you have held the key for less than the shorted time we do a short press
        if(heldFor < HOLD_MS_FIRST):
            print('short press')
            keyboard.send(CENTRE_KEY)
        # If its been less than the long press then run the medium press
        elif(heldFor < (HOLD_MS_SECOND)):
            print('medium press')
            keyboard.send(*CENTRE_KEY_HOLD_FIRST)
        # Otherwise its a long press
        else:
            print('long press start')
            consumerControl.send(CENTRE_KEY_HOLD_SECOND)
            print('Ejected')
            time.sleep(2)
            print('Slept')
            keyboard.send(*CENTRE_KEY_HOLD_SECOND_FOLLOW_UP)
            print('long press finished')

    # If you are currently holding the centre button we want to play the buzzer to indicate that you have hit the threshold
    if(centre.value is False and playedFirstBeep is False):
        currentTime = time.monotonic_ns()
        currentHeldFor = (currentTime - centreHeldAt) / 1000000

        if(currentHeldFor > HOLD_MS_FIRST):
            playedFirstBeep = True
            buzzer.play([(600, 0.2)])

    # If you are currently holding the centre button we want to play the buzzer to indicate that you have hit the threshold
    if(centre.value is False and playedSecondBeep is False):
        currentTime = time.monotonic_ns()
        currentHeldFor = (currentTime - centreHeldAt) / 1000000

        if(currentHeldFor > (HOLD_MS_SECOND)):
            playedSecondBeep = True
            buzzer.play([(600, 0.1),(0,0.1),(600,0.1)])


    # Match up switch value to key value
    if(up.value is False):
        keyboard.press(UP_KEY)
    else:
        keyboard.release(UP_KEY)

    if(down.value is False):
        keyboard.press(DOWN_KEY)
    else:
        keyboard.release(DOWN_KEY)

    if(left.value is False):
        keyboard.press(LEFT_KEY)
    else:
        keyboard.release(LEFT_KEY)

    if(right.value is False):
        keyboard.press(RIGHT_KEY)
    else:
        keyboard.release(RIGHT_KEY)


    # Pulse the dotstar if you are advertising
    if(ble.advertising is True):
        newDotColor = pulsingDotstar.pulse()


    # Set the color of the LED
    dotstars[0] = newDotColor
