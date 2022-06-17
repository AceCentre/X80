# =======================================
# ========== USER CONSTANTS =============
# =======================================

from x80_constants import *
from output import Output

# You can pick between the following switches, which are named as labeled on the box
# INPUT_SWITCH_ZERO, INPUT_SWITCH_ZERO, INPUT_SWITCH_ZERO, INPUT_SWITCH_ZERO

# This is the switch that is passed through to the device
CONTROLLING_SWITCH = INPUT_SWITCH_ZERO

# This is the switch that changes the output device
SWITCHER_SWITCH = INPUT_SWITCH_ONE

# The color you want the box to appear when each switch is pressed, given in (r,g,b)
CONTROLLING_SWITCH_COLOR = (255, 0, 0)
SWITCHER_SWITCH_COLOR = (0, 255, 0)

# How bright you want the LED to appear
DOTSTAR_BRIGHTNESS = 1

# This is the list of output options to cycle through
# Must be a list of Output objects
OUTPUTS = [
    Output(
        'first',                # ID
        (0,0,255),              # Color box to be when this output is selected
        OUTPUT_SWITCH_W,        # Output 
        [                       # Tone to play when box is selected
            (600, 0.2)          # (note, time)
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
    )
]

# The amount of time a the switch needs to be held for before output is switched
# This is given in NS
# 1 * 1000000000 = 1 Second
MINIMUM_PRESS_TIME_TO_SWITCH = 1 * 1000000000
