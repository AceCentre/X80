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