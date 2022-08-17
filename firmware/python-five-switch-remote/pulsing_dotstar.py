import time

class PulsingDotstar():
    def __init__(self):
        self.lastChangeTime = time.monotonic_ns()
        self.currentBrightness = 1.0
        self.currentModifier = 0.01
        self.currentColor = (0,0,255,1)

    def update(self):
        self.id=""

        currentTime = time.monotonic_ns()
        timeSince = (currentTime - self.lastChangeTime) / 1000000000

        if(timeSince > 0.01):
            self.lastChangeTime = currentTime

            if(self.currentBrightness is 1.0):
                self.currentModifier = -0.01

            if(self.currentBrightness is 0.0):
                self.currentModifier = 0.01

            self.currentBrightness = round(self.currentBrightness + self.currentModifier, 2)

        self.currentColor = (0,0,255, self.currentBrightness)

    def pulse(self):
        return self.currentColor
