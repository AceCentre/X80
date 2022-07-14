import time
from user_constants import *

class Pulser():
    def __init__(self):
        print('Test')
        self.isSendingPulse = False
        self.currentOutputDevice = None

    def update(self):
        currentTime = time.monotonic_ns()

        if(self.isSendingPulse is True):
            timeElapsed = currentTime - self.startPulsing
            if(timeElapsed > SWITCH_PULSE_LENGTH):
                self.isSendingPulse = False
                self.currentOutputDevice.value = False

    def sendPulse(self, outputToPulse):
        # If we are already sending a pulse then ignore it
        if(self.isSendingPulse is True):
            return

        self.isSendingPulse = True
        self.currentOutputDevice = outputToPulse
        self.startPulsing = time.monotonic_ns()
        self.currentOutputDevice.value = True
