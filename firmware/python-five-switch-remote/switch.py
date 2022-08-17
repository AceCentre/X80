import digitalio
from x80_constants import *

class Switch():
    def __init__(self, pin):
        self.switch = digitalio.DigitalInOut(pin)
        self.switch.direction = digitalio.Direction.INPUT
        self.switch.pull = PULL_MODES[pin]

    @property
    def value(self):
        return self.switch.value
