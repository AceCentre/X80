import asyncio
from bleak import BleakScanner
import time
from pynput.keyboard import Key, Controller

keyboard = Controller()

target_mac = "C1:C1:A7:3E:2C:9C"
keyboard_repeat = True

ID = 65535
                         
package_size = 8
replay_interval = 0.03 # in seconds
          
replay_buffer = asyncio.Queue()

prev_package_i = -1
          
keyboard_state = 0

                    
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
                        await asyncio.to_thread(keyboard.press, Key.space)

                    keyboard_state += 1
                else:
                    keyboard_state = 0
            else:
                if keyboard_state == 0 and received_state == 1:
                    await asyncio.to_thread(keyboard.press, Key.space)            
                keyboard_state = received_state
            
            replay_val = replay_val >> 1        
            
            if current:
                await asyncio.sleep(replay_interval)

async def scan():    
    stop_event = asyncio.Event()

    def callback(device, advertising_data):
        global replay_buffer, prev_package_i
        if len(target_mac) == 0:
            print("{:.3f} | MAC: {} | Adv: {}".format(time.time(), device.address, advertising_data))    
        elif device.address == target_mac:
            package_i = advertising_data.manufacturer_data[ID][0]

            if prev_package_i != package_i:
                print("Adv package: {}".format(advertising_data))
                print("received index", package_i)
                
                adv_package =advertising_data.manufacturer_data[ID][1::]
                
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
                                                                                                           
        #stop_event.set()

    async with BleakScanner(callback, scanning_mode="active") as scanner:
        print("start scanning for: {}".format(target_mac))

        await stop_event.wait()
    print("scanning end")

async def main():
    await asyncio.gather(scan(), replay())

asyncio.run(main())
                                                                                                  