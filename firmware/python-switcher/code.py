import board
import digitalio
import time
import adafruit_dotstar
from adafruit_debouncer import Debouncer
from x80_constants import *
from user_constants import *

# Controlling Switch
controllingSwitch = digitalio.DigitalInOut(CONTROLLING_SWITCH)
controllingSwitch.direction = digitalio.Direction.INPUT
controllingSwitch.pull = PULL_MODES[CONTROLLING_SWITCH]

# Switcher
switcherSwitchPin = digitalio.DigitalInOut(SWITCHER_SWITCH)
switcherSwitchPin.direction = digitalio.Direction.INPUT
switcherSwitchPin.pull = PULL_MODES[SWITCHER_SWITCH]
switcherSwitch = Debouncer(switcherSwitchPin)

dotstars = adafruit_dotstar.DotStar(
    DOTSTAR_CLOCK, DOTSTAR_DATA, 1, brightness=DOTSTAR_BRIGHTNESS
)

selectedOutputCount = 0
switcherSwitchPressedAt = 0
isSwitcherSwitchPressed = False

while True:
    switcherSwitch.update()
    wrappedOutputCount = selectedOutputCount % len(OUTPUTS)
    selectedOutput = OUTPUTS[wrappedOutputCount]

    newDotColor = selectedOutput.selectedColor

    if controllingSwitch.value == False:
        newDotColor = CONTROLLING_SWITCH_COLOR

    if switcherSwitch.value == False:
        newDotColor = SWITCHER_SWITCH_COLOR

    # First time loop ran when switch is pressed
    if switcherSwitch.value is False and isSwitcherSwitchPressed is False:
        switcherSwitchPressedAt = time.monotonic_ns()
        print('first press started at', switcherSwitchPressedAt)
    # Switch is lifted after being down
    if switcherSwitch.value is True and isSwitcherSwitchPressed is True:
        switchHeldFor = time.monotonic_ns() - switcherSwitchPressedAt

        print('Switch lifted after being held for:', switchHeldFor)

        # Make sure the switch was held for long enough before switching output
        if switchHeldFor > MINIMUM_PRESS_TIME_TO_SWITCH:
            print('Output changed')
            print()
            selectedOutputCount = selectedOutputCount + 1
        else:
            print()


    isSwitcherSwitchPressed = switcherSwitch.value is False

    dotstars[0] = newDotColor
