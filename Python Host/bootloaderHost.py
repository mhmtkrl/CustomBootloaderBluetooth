import serial
import time

BluetoothDevice = 'COM7'
BaudRate = 115200

print('Welcome to the Bootloader Server')
myBluetooth = serial.Serial(BluetoothDevice, BaudRate)
print('Connected to the Bluetooth Device')

IsThereAnyUpdate = input('Is There Any Update?')
print(IsThereAnyUpdate)
    
while True:
    new_data = myBluetooth.readline()
    print(str(new_data))
    myBluetooth.write(bytes(0x65))
    time.sleep(1)
    
