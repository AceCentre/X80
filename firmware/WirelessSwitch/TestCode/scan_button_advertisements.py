import asyncio
from bleak import BleakScanner
import time

target_mac = "C1:C1:A7:3E:2C:9C"
ID = 65535

async def main():
    stop_event = asyncio.Event()

    def callback(device, advertising_data):
        if len(target_mac) == 0:
            print("{:.3f} | MAC: {} | Adv: {}".format(time.time(), device.address, advertising_data))    
        elif device.address == target_mac:
            package_i = advertising_data.manufacturer_data[ID][0]
            received_package = int.from_bytes(advertising_data.manufacturer_data.get(ID)[1::], byteorder="big")
            print("{:.3f} | received index: {} | received package val: {}".format(time.time(), package_i, received_package, received_package))
        #stop_event.set()

    async with BleakScanner(callback, scanning_mode="active") as scanner:
        print("start scanning for: {}".format(target_mac))

        await stop_event.wait()
    print("scanning end")

asyncio.run(main())
