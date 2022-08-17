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
        self.type = 'Pin'

    @property
    def value(self):
        return self.output.value

    @value.setter
    def value(self, value):
        self.output.value = value

class StubKeyboard():
    def __init__(self):
        print('Init stub')

    def press(self, key):
        print('Press stub')

    def release_all(self):
        print('Release stub')

class StubKeyboardLayout():
    def __init__(self):
        print('Init stub')

class BluetoothOutput():
    def __init__(self, id, selectedColor, tune, key):
        self.id = id
        self.selectedColor = selectedColor
        self.keyboard = StubKeyboard()
        self.keyboardLayout = StubKeyboardLayout()
        self.value = False
        self.tune = tune
        self.type = 'Bluetooth'
        self.key = key
        self.isKeyPressed = False


    @property
    def value(self):
        return self.isKeyPressed

    @value.setter
    def value(self, value):
        if(self.keyboard is None or self.keyboardLayout is None):
            print('Do nothing as keyboard is not initialised')

        if(value is True):
            self.keyboard.press(self.key)
            self.isKeyPressed = True
        elif(value is False):
            self.keyboard.release_all()
            self.isKeyPressed = False
