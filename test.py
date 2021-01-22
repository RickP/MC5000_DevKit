import serial
import time

ser = serial.Serial('/dev/tty.usbserial-0001')  # open serial port

ser.reset_input_buffer()

def write_start_prog(cpu_serial):
    time.sleep(0.1)
    ser.write(b'$')
    time.sleep(0.1)
    ser.write(str(cpu_serial).encode('utf-8'))

def write_serial_8bit(a):
    time.sleep(0.1)
    ser.write(a.to_bytes(1, 'big'))

def write_serial_16bit(a):
    time.sleep(0.1)
    ser.write((a >> 8).to_bytes(1, 'big'))
    time.sleep(0.1)
    ser.write((a & 0x00FF).to_bytes(1, 'big'))

def write_end_prog():
    time.sleep(0.1)
    ser.write(b'&')

write_start_prog(0)
write_serial_8bit(0x02 << 2)
write_serial_16bit(100)
write_serial_8bit(1)
write_serial_8bit(0x04 << 2)
write_serial_16bit(1)
write_serial_8bit(0x02 << 2)
write_serial_16bit(100)
write_serial_8bit(0)
write_serial_8bit(0x04 << 2)
write_serial_16bit(10)
write_end_prog()

for i in range(3):
    print(ser.readline())

ser.close()
