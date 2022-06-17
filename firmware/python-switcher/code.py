import board
import digitalio
import time
import adafruit_dotstar
from adafruit_debouncer import Debouncer
from x80_constants import *
from user_constants import *

# Disable passthrough
passthrough = digitalio.DigitalInOut(board.D12)
passthrough.direction = digitalio.Direction.OUTPUT
passthrough.value = True

# Controlling Switch (the switch that is passed through to the target)
controllingSwitch = digitalio.DigitalInOut(CONTROLLING_SWITCH)
controllingSwitch.direction = digitalio.Direction.INPUT
controllingSwitch.pull = PULL_MODES[CONTROLLING_SWITCH]

# Switcher switch (the switch that changes the target)
switcherSwitchPin = digitalio.DigitalInOut(SWITCHER_SWITCH)
switcherSwitchPin.direction = digitalio.Direction.INPUT
switcherSwitchPin.pull = PULL_MODES[SWITCHER_SWITCH]
switcherSwitch = Debouncer(switcherSwitchPin)

# Connect to dotstar
dotstars = adafruit_dotstar.DotStar(
    DOTSTAR_CLOCK, DOTSTAR_DATA, 1, brightness=DOTSTAR_BRIGHTNESS
)

# Initialise state
# TODO: One day it would be nice to sync this with non-volatile memory
selectedOutputCount = 0
switcherSwitchPressedAt = 0
isSwitcherSwitchPressed = False
tuneTriggered = False

while True:
    switcherSwitch.update() # Update debouncer
    
    # Get the current output. Using the modulo operator means we can just
    # endlessly add to the output count and it will wrap around
    wrappedOutputCount = selectedOutputCount % len(OUTPUTS)
    selectedOutput = OUTPUTS[wrappedOutputCount]

    # Default the dotstar color to represent what is currently selected
    newDotColor = selectedOutput.selectedColor

    # Change the color of the dotstar if a switch is being pressed down
    if controllingSwitch.value == False:
        newDotColor = CONTROLLING_SWITCH_COLOR

    if switcherSwitch.value == False:
        newDotColor = SWITCHER_SWITCH_COLOR

    # First time loop ran when switch is pressed, then store the moment it was pressed
    if switcherSwitch.value is False and isSwitcherSwitchPressed is False:
        switcherSwitchPressedAt = time.monotonic_ns()
        print('first press started at', switcherSwitchPressedAt)
    # Play a tune if the output change has been held down for long enough to switch
    # Note, the switch wont actually happen until you lift the switchç
    elif switcherSwitch.value is False and isSwitcherSwitchPressed is True:
        switchHeldFor = time.monotonic_ns() - switcherSwitchPressedAt
        if switchHeldFor > MINIMUM_PRESS_TIME_TO_SWITCH and tuneTriggered is False:
            print('Play a tune here')
            tuneTriggered = True


        
    # Switch is lifted after being down
    # Then check if it was held down for long enough
    if switcherSwitch.value is True and isSwitcherSwitchPressed is True:
        switchHeldFor = time.monotonic_ns() - switcherSwitchPressedAt

        print('Switch lifted after being held for:', switchHeldFor)

        # Make sure the switch was held for long enough before switching output
        if switchHeldFor > MINIMUM_PRESS_TIME_TO_SWITCH:
            print('Output changed')
            print()
            selectedOutputCount = selectedOutputCount + 1
            tuneTriggered = False
        else:
            print()


    # Store the current state of the switcher
    isSwitcherSwitchPressed = switcherSwitch.value is False
    
    # Output the value of the controlling switch
    selectedOutput.value = controllingSwitch.value is False
    
    # Set the color of the LED
    dotstars[0] = newDotColor