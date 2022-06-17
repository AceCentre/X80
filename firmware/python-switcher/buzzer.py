import time
import pwmio

# This buzzer class allows us to play sounds without putting sleeps in the loop
# Putting sleeps in is bad because it means the x80 cant do anything else

OFF = 0
ON = 2**15

class Buzzer():
    def __init__(self, id, buzzerPin):
        self.id = id
        self.buzzerPin = buzzerPin
        self.buzzer = pwmio.PWMOut(buzzerPin, variable_frequency=True)
        self.startPlaying = False
        self.currentToneCount = 0
        self.currentlyPlayingSounds = []
        self.currentToneStartedAt = 0
        self.isPlaying = False
        
    def update(self):
        if self.startPlaying is True:
            self.startPlaying = False
            self.currentToneStartedAt = time.monotonic_ns()
            current = self.currentlyPlayingSounds[self.currentToneCount]
            self.buzzer.frequency = current[0]
            self.buzzer.duty_cycle = ON
            self.isPlaying = True
            
        if self.isPlaying is True:
            current = self.currentlyPlayingSounds[self.currentToneCount]
            currentTime = time.monotonic_ns()
            timeElapsed = currentTime - self.currentToneStartedAt
            
            if (timeElapsed / 1000000000) > current[1]:
                self.currentToneCount = self.currentToneCount + 1
                
                if self.currentToneCount >= len(self.currentlyPlayingSounds):
                    self.buzzer.duty_cycle = OFF
                    self.isPlaying = False
                else:
                    nextTone = self.currentlyPlayingSounds[self.currentToneCount]
                    self.currentToneStartedAt = time.monotonic_ns()
                    
                    if nextTone[0] == 0:
                        self.buzzer.duty_cycle = OFF
                    else: 
                        self.buzzer.duty_cycle = ON
                        self.buzzer.frequency = nextTone[0]


        
    def play(self, sounds):
        self.currentlyPlayingSounds = sounds
        self.startPlaying = True
        self.currentToneCount = 0
        self.currentToneStartedAt = 0
        self.isPlaying = False
