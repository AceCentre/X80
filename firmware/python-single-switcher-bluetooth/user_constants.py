# =======================================
# ========== USER CONSTANTS =============
# =======================================

from x80_constants import *
from output import Output, BluetoothOutput
from adafruit_hid.keycode import Keycode

# You can pick between the following switches, which are named as labeled on the box
# INPUT_SWITCH_ZERO, INPUT_SWITCH_ZERO, INPUT_SWITCH_ZERO, INPUT_SWITCH_ZERO

# This is the switch that is passed through to the device and the one used to switch
INPUT_SWITCH = INPUT_SWITCH_ZERO

# The color you want the box to appear when each switch is pressed
INPUT_SWITCH_COLOR = (255, 0, 0)

# How bright you want the LED to appear
DOTSTAR_BRIGHTNESS = 1

# This will force bluetooth connection before anything else can happen
BLUETOOTH_DEVICE_IN_USE = True
BLUETOOTH_DEVICE_NAME = 'Ace Switcher Keyboard'

# The output objects, the first one will be enabled by default
OUTPUTS = [
    Output(
        'first',
        (0,0,255),
        OUTPUT_SWITCH_W,
        [
            (600, 0.2)
        ]
    ),
    Output(
        'second',
        (255,255,255),
        OUTPUT_SWITCH_X,
        [
            (600, 0.1),
            (0,0.1),
            (600,0.1)
        ]
    ),
    BluetoothOutput(
        'third',
        (255,255,255),
        [
            (600, 0.1),
            (0,0.1),
            (600,0.1),
            (0,0.1),
            (600,0.1)
        ],
        Keycode.SPACE
    )
]

# The amount of time a the switch needs to be held for before output is switched
# This is given in NS
# 5 * 1000000000 = 5 Second
MINIMUM_PRESS_TIME_TO_SWITCH = 5 * 1000000000

# This is the length of the pulse that is sent to the output device after a key press
SWITCH_PULSE_LENGTH = 0.3 * 1000000000 # 300ms
