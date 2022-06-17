# ======================================
# ========== X80 CONSTANTS =============
# ======================================

import board
import digitalio

DOTSTAR_CLOCK = board.APA102_SCK
DOTSTAR_DATA = board.APA102_MOSI
INPUT_SWITCH_ZERO = board.A4                # This is marked >0 on the box
INPUT_SWITCH_ONE = board.A3                 # This is marked >1 on the box
INPUT_SWITCH_TWO = board.A2                 # This is marked >2 on the box
INPUT_SWITCH_THREE = board.A1               # This is marked >3+ on the box
PULL_MODES = {}                             # Different switches use different pull modes
PULL_MODES[board.A4] = None
PULL_MODES[board.A3] = digitalio.Pull.UP
PULL_MODES[board.A2] = digitalio.Pull.UP
PULL_MODES[board.A1] = digitalio.Pull.UP
OUTPUT_SWITCH_W = board.D7
OUTPUT_SWITCH_X = board.D9
OUTPUT_SWITCH_Y = board.D10
OUTPUT_SWITCH_Z = board.D11
PASSTHROUGH_SWITCH = board.D12
BUZZER_PIN = board.D13

NOTE_AS6 = 1865
NOTE_A6 = 1760
NOTE_F5 = 698
NOTE_CS5 = 554
NOTE_D5 = 587
NOTE_AS5 = 932
NOTE_AS4 = 466

STARTUP_TUNE = [
    (NOTE_AS6, 1/8),
    (NOTE_A6, 1/4),
    (NOTE_F5, 1/4),
    (NOTE_CS5, 1/16),
    (NOTE_D5, 1/4),
    (NOTE_F5, 1/4),
    (NOTE_AS4, 1/4)
]