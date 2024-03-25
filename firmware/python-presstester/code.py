import board
import digitalio
import time
import adafruit_dotstar
from adafruit_debouncer import Debouncer
from x80_constants import *
from user_constants import *
from buzzer import Buzzer
from microcontroller import watchdog as watchdoggo
from watchdog import WatchDogMode
from pulser import Pulser

# Initialize watchdog
watchdoggo.timeout = 2.5
watchdoggo.mode = WatchDogMode.RAISE

# Connect to dotstar
dotstars = adafruit_dotstar.DotStar(
    DOTSTAR_CLOCK, DOTSTAR_DATA, 1, brightness=DOTSTAR_BRIGHTNESS
)

# Controlling Switch (the switch that is passed through to the target)
controllingSwitch = digitalio.DigitalInOut(CONTROLLING_SWITCH)
controllingSwitch.direction = digitalio.Direction.INPUT
controllingSwitch.pull = PULL_MODES[CONTROLLING_SWITCH]

# Pulser
pulser = Pulser()

# Connect buzzer
buzzer = Buzzer('Buzzer', BUZZER_PIN)

# Timing for toggling OUTPUT_SWITCH_W
last_toggle_time = time.monotonic()
toggle_interval = 1  # seconds
tuneTriggered = False
startupTunePlayed = False
selectedOutputCount = 0
isOutputOn = False  # Tracks the current state of the output

while True:
    watchdoggo.feed()
    buzzer.update() # Update buzzer
    pulser.update() # Update pulser
    
    if startupTunePlayed is False:
        buzzer.play(STARTUP_TUNE)
        startupTunePlayed = True
    
    # Get the current output. Using the modulo operator means we can just
    # endlessly add to the output count and it will wrap around
    wrappedOutputCount = selectedOutputCount % len(OUTPUTS)
    selectedOutput = OUTPUTS[wrappedOutputCount]
    newDotColor = selectedOutput.selectedColor

    # Check if it's time to toggle OUTPUT_SWITCH_W
    current_time = time.monotonic()
    if current_time - last_toggle_time >= toggle_interval:
        # It's time to toggle the state
        isOutputOn = not isOutputOn  # Toggle the state
        last_toggle_time = current_time  # Update the last toggle time

        # Apply the new state to the selected output and DotStar
        selectedOutput.value = isOutputOn
        if isOutputOn:
            print(f"{selectedOutput.id} Switch ON")
            print('Send Pulse')
            pulser.sendPulse(selectedOutput)
            dotstars[0] = selectedOutput.selectedColor  # Use the selected output's color
        else:
            print(f"{selectedOutput.id} Switch OFF")
            dotstars[0] = (255, 255, 255)  # Turn off the DotStar or set to a different color


    
    selectedOutput.value = controllingSwitch.value is False
    
    # Set the color of the LED
    dotstars[0] = newDotColor

