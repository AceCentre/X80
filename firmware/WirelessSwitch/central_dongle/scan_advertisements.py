import asyncio
from bleak import BleakScanner

async def main():
    stop_event = asyncio.Event()

    def callback(device, advertising_data):
        print("addr: {}; adv package: {}".format(device.address, advertising_data.manufacturer_data))
        
        #stop_event.set()

    async with BleakScanner(callback, scanning_mode="active") as scanner:
        print("start scanning")
        
        
        await stop_event.wait()
    print("scanning end")

asyncio.run(main())