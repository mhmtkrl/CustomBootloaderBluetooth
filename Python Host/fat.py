#Project: Bootloader Server
#Communication Bus: Serial Port (UART)
#Author: Mehmet KORAL
#Date: 07.09.2021

import serial
import time

protocolLength = 7
protocolPacket = bytearray(protocolLength)
codeSize = 128
#Read Binary File
#with open ("test-bin-file", 'rb') as f:
with open ("AppCodeProject.bin", 'rb') as f:
    bootFile = f.read()
    
print('Length of the File: ', len(bootFile), ' bytes')
blockLength = int(len(bootFile) / codeSize)+1
print('Block Length: ', blockLength)
i = 0

BluetoothDevice = 'COM7'
BaudRate = 115200

print('Welcome to the Bootloader Server')
myBluetooth = serial.Serial(BluetoothDevice, BaudRate)
print('Connected to the Bluetooth Device')


while True:
    new_data = myBluetooth.readline()
    print("new data:", (new_data))
    if myBluetooth.read().hex() == "15":
        time.sleep(0.1)
        print('Transfer is being processing!')
        i = 0;
        myBluetooth.flushInput()
        while i != (blockLength):
            
            newPacket = bootFile[(codeSize*i):(codeSize*(i+1))];
            #if e lengt of the last block is less than codeSize
            #complete it to the codesize by adding dummy byte 
            if len(newPacket) < codeSize:
                for j in range(len(newPacket), codeSize):
                    newPacket = newPacket + bytes.fromhex("1A")
            
            if (i+1) == blockLength:
                protocolPacket = bytes.fromhex("04") + i.to_bytes(1, 'big') + (255-i).to_bytes(1, 'big') +  newPacket  + blockLength.to_bytes(2, 'little')
            else:
                protocolPacket = bytes.fromhex("01") + i.to_bytes(1, 'big') + (255-i).to_bytes(1, 'big') + newPacket  + blockLength.to_bytes(2, 'little')
                
            print(i+1, '. block by', blockLength, ': ', protocolPacket.hex());
            myBluetooth.write(protocolPacket)
            i = i + 1
            
            new_data = myBluetooth.read()
            while new_data.hex() != "06":
                new_data = myBluetooth.read()
                print("ack")
            time.sleep(0.1)
        

#myBluetooth.close()
print('File has been sent')
