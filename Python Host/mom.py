import serial
from xmodem import XMODEM
from time import sleep

BluetoothDevice = 'COM7'
BaudRate = 115200

print('Welcome to the Bootloader Server')
s = serial.Serial(BluetoothDevice, BaudRate)


def getc(size, timeout=1):
    return s.read(size)
def putc(data, timeout=1):
    s.write(data)
modem = XMODEM(getc, putc)

stream = open('AppCodeProject.bin', 'rb')
status = modem.send(stream, retry=8)
status = modem.send(stream, retry=8)
s.close()
stream.close()