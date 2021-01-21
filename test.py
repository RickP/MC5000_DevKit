import serial
import time

ser = serial.Serial('/dev/tty.usbserial-0001')  # open serial port
while True:
    ser.write(b'1')
    time.sleep(0.2)
    ser.write(b'2')
    time.sleep(0.2) 
ser.close()
