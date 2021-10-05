import serial

BluetoothDevice = 'COM7'
BaudRate = 115200

print('Welcome to the Bootloader Server')
myBluetooth = serial.Serial(BluetoothDevice, BaudRate)
print('Connected to the Bluetooth Device')

while True:
    new_data = myBluetooth.readline()
    print(str(new_data))
