# =======================================
# ========== USER CONSTANTS =============
# =======================================

from x80_constants import *
from output import Output, BluetoothOutput
from adafruit_hid.keycode import Keycode
from adafruit_hid.consumer_control_code import ConsumerControlCode

# How bright you want the LED to appear
DOTSTAR_BRIGHTNESS = 1

# This will force bluetooth connection before anything else can happen
BLUETOOTH_DEVICE_NAME = 'Ace Remote Keyboard'

# This is where the switches are plugged into, its best to leave it like this
CENTRE_BUTTON_PIN = INPUT_SWITCH_ZERO
UP_BUTTON_PIN = INPUT_SWITCH_ONE
DOWN_BUTTON_PIN = INPUT_SWITCH_TWO
RIGHT_BUTTON_PIN = INPUT_SWITCH_THREE
LEFT_BUTTON_PIN = INPUT_SWITCH_FOUR

# The keys that should be pressed on each key
CENTRE_KEY = Keycode.SPACE # THis is a short press
DOWN_KEY = Keycode.DOWN_ARROW
UP_KEY = Keycode.UP_ARROW
LEFT_KEY = Keycode.LEFT_ARROW
RIGHT_KEY = Keycode.RIGHT_ARROW

# Centre Key After n seconds
CENTRE_KEY_HOLD_FIRST = [Keycode.COMMAND, Keycode.H] # This is the home command
HOLD_MS_FIRST = 2000 # This is the length of time you have to hold to trigger the medium press

# Centre Key after n + x seconds
CENTRE_KEY_HOLD_SECOND = ConsumerControlCode.EJECT # This brings up the keyboard
HOLD_MS_SECOND = 4000 # This is the length of time you have to hold to trigger long press (must be longer than medium)
