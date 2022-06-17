import digitalio

class Output():
    def __init__(self, id, selectedColor, outputPin, tune):
        self.id = id
        self.selectedColor = selectedColor
        self.outputPin = outputPin
        self.output = digitalio.DigitalInOut(self.outputPin)
        self.output.direction = digitalio.Direction.OUTPUT
        self.value = self.output.value
        self.tune = tune
        
    @property
    def value(self):
        return self.output.value
        
    @value.setter
    def value(self, value):
        self.output.value = value
