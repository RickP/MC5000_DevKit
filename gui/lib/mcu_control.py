import serial
import time
import binascii

class MCUControl():

    def __init__(self, serial_port):
        self.serial_delay_time = 0.01
        self.ser = serial.Serial(serial_port, 9600, timeout=0.5)  # open serial port
        self.ser.reset_input_buffer()


    def checkString(self, a):
        if isinstance(a, str):
            try:
                a = int(a)
            except:
                a = a.encode('utf-8')[0]
        return a

    def GetStatus(self):
        if self.mcu_id is not None:
            self.ser.reset_input_buffer()
            self.ser.write(self.mcu_id)
            hex = []
            for i in range(4):
                line = self.ser.readline(3).rstrip()
                if len(line) < 2:
                    return None
                hex.append(line)
            ret = []
            ret.append(int(hex[0] + hex[1], 16))
            ret.append(int(hex[2] + hex[3], 16))
            return ret


    def UploadCode(self, code, mcu_id):
        self.serial_delay_time = 0.01

        self.mcu_id = mcu_id.encode('utf-8')[0].to_bytes(1, 'big')

        if len(code) > 1:
            self.write_start_prog(self.mcu_id)

            for line in code.splitlines():
                parts = line.split()
                add_to_command = 0;
                for i, part in enumerate(parts):
                    # Conditions
                    if part == '+':
                        add_to_command = 0x01;
                    if part == '-':
                        add_to_command = 0x02;

                    # commands
                    if part == 'mov':
                        self.write_serial_8bit((0x02 << 2) & (0xFC + add_to_command))
                        self.write_serial_16bit(parts[i + 1])
                        self.write_serial_8bit(parts[i + 2])
                        break
                    if part == 'slp':
                        self.write_serial_8bit(0x04 << 2 & (0xFC + add_to_command))
                        self.write_serial_16bit(parts[i + 1])
                        break

            self.write_end_prog()

    def write_start_prog(self, mcu_id):
        time.sleep(self.serial_delay_time)
        self.ser.write(b'$')
        time.sleep(self.serial_delay_time)
        self.ser.write(mcu_id)

    def write_serial_8bit(self, a):
        a = self.checkString(a)

        time.sleep(self.serial_delay_time)
        self.ser.write(a.to_bytes(1, 'big'))

    def write_serial_16bit(self, a):
        a = self.checkString(a)

        time.sleep(self.serial_delay_time)
        self.ser.write((a >> 8).to_bytes(1, 'big'))
        time.sleep(self.serial_delay_time)
        self.ser.write((a & 0x00FF).to_bytes(1, 'big'))

    def write_end_prog(self):
        time.sleep(self.serial_delay_time)
        self.ser.write(b'&')

    def __del__(self):
        self.ser.close()
