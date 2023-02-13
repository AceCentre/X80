import sys 
import pathlib
import time

import asyncio
from bleak import BleakScanner

from pynput.keyboard import Key, Controller

active_key = Key.space
keyboard_repeat = True
MANUFACTURER_ID = 65535

button_filepath = pathlib.Path(__file__).parent.resolve() / "button_mac"

target_mac = None

prev_package_i = -1

pairing_package = b'\x9c|\x00\x00\x00'

scan_stop_event = asyncio.Event()

package_size = 8
replay_interval = 0.03 # in seconds          
replay_buffer = asyncio.Queue()
keyboard_state = 0

keyboard = Controller()

def load_mac():
    global target_mac

    if button_filepath.exists():
        with open(button_filepath, "r") as f:
            target_mac = f.read()
            return True
    else:
        return False

def save_mac(mac):
    global target_mac

    print("saving mac: ", mac)
    with open(button_filepath, "w") as f:
        target_mac = f.write(mac)

async def replay():
    global replay_buffer, keyboard_state

    print("start replay function. Interval:", replay_interval)
    while True:
        replay_i, replay_val, current = await replay_buffer.get()
        print("replay pack: {} - {}; curr: {}".format(replay_i, bin(replay_val), current))
        
        for i in range(package_size):
            received_state = replay_val & 1

            if keyboard_repeat:
                if received_state == 1:
                    if keyboard_state == 0 or keyboard_state > 20:
                        await asyncio.to_thread(keyboard.press, active_key)

                    keyboard_state += 1
                else:
                    keyboard_state = 0
            else:
                if keyboard_state == 0 and received_state == 1:
                    await asyncio.to_thread(keyboard.press, active_key)
                keyboard_state = received_state
            
            replay_val = replay_val >> 1        
            
            if current:
                await asyncio.sleep(replay_interval)

def receive_button_data(device, advertising_data):
    global replay_buffer, prev_package_i
    if len(target_mac) == 0:
        print("{:.3f} | MAC: {} | Adv: {}".format(time.time(), device.address, advertising_data))    
    elif device.address == target_mac:
        package_i = advertising_data.manufacturer_data[MANUFACTURER_ID][0]

        if prev_package_i != package_i:
            print("Adv package: {}".format(advertising_data))
            print("received index", package_i)
            
            adv_package = advertising_data.manufacturer_data[MANUFACTURER_ID][1::]
            
            package_diff = package_i - prev_package_i
            if package_diff < 0:
                package_diff += 256
            if package_diff > 4:
                package_diff = 4
            i = package_diff - 1
            print("package_diff: ", package_diff)

            current = False

            while i >= 0:
                if i == 0:
                    current = True
                print("put package {}:{}; curr:{}".format((package_i-i), bin(adv_package[i]), current))    
                replay_buffer.put_nowait(((package_i-i), adv_package[i], current))

                i-=1
            
            prev_package_i = package_i

def pair(device, advertising_data):
    dev_mac = device.address
    adv_package = advertising_data.manufacturer_data.get(MANUFACTURER_ID)
    if adv_package != None and len(adv_package) == 5:
        if pairing_package == adv_package:
            print("Got pairing package from: {}".format(dev_mac))
            save_mac(dev_mac)
            
            scan_stop_event.set()
    else:
        print("other device:", dev_mac)

async def scan(callback_fun):
    async with BleakScanner(callback_fun, scanning_mode="active") as scanner:

        await scan_stop_event.wait()


async def main():    
    n_args = len(sys.argv)
    if n_args == 1:
        if load_mac():
            print("start receiving from: {}".format(target_mac))
            await asyncio.gather(scan(receive_button_data), replay())
        else:
            print("No button was paired. Run pairing firstly.")
    elif n_args == 2 and sys.argv[1] == "pair":
        print("Start pairing")        
        await scan(pair)
        print("pairing end")

asyncio.run(main())