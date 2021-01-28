import serial
import time
import binascii

SIGNAL_BYTE = 0x7F

class MCUControl():

    def __init__(self, serial_port):
        self.serial_delay_time = 0.1
        self.ser = serial.Serial(serial_port, 2400, timeout=0.5)  # open serial port
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
            self.write_serial_8bit(self.mcu_id, False)
            hex = []
            for i in range(4):
                line = self.ser.readline(3).rstrip()
                if not len(line):
                    return None
                hex.append(line)
            ret = []
            ret.append(int(hex[0] + hex[1], 16)-1000)
            ret.append(int(hex[2] + hex[3], 16)-1000)
            return ret


    def UploadCode(self, code, mcu_id):
        self.serial_delay_time = 0.01

        self.mcu_id = ord(mcu_id)

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
                        self.write_serial_8bit((0x02 << 2) | add_to_command)
                        ri = parts[i + 1]
                        r = parts[i + 2]

                        if ri == 'x0':
                            self.write_serial_16bit(0x4000)
                        elif ri == 'x1':
                            self.write_serial_16bit(0x4800)
                        elif ri == 'p0':
                            self.write_serial_16bit(0x5000)
                        elif ri == 'p1':
                            self.write_serial_16bit(0x5800)
                        elif ri == 'dat':
                            self.write_serial_16bit(0x6000)
                        elif ri == 'acc':
                            self.write_serial_16bit(0x7000)
                        else:
                            self.write_serial_16bit(int(ri))

                        if r == 'x0':
                            self.write_serial_8bit(0x00)
                        elif r == 'x1':
                            self.write_serial_8bit(0x10)
                        elif r == 'p0':
                            self.write_serial_8bit(0x20)
                        elif r == 'p1':
                            self.write_serial_8bit(0x30)
                        elif r == 'dat':
                            self.write_serial_8bit(0x40)
                        elif r == 'acc':
                            self.write_serial_8bit(0x60)

                        else:
                            self.write_serial_8bit(0)
                        break
                    if part == 'slp':
                        self.write_serial_8bit(0x04 << 2 & (0xFC + add_to_command))
                        self.write_serial_16bit(int(parts[i + 1]))
                        break

            self.write_end_prog()

    def write_start_prog(self, mcu_id):
        self.write_serial_8bit(SIGNAL_BYTE)
        self.write_serial_8bit(mcu_id)

    def write_end_prog(self):
        self.write_serial_8bit(SIGNAL_BYTE)

    def write_serial_8bit(self, a, printval=True):
        time.sleep(self.serial_delay_time)
        if printval:
            print(a.to_bytes(1, 'big'))
        self.ser.write(a.to_bytes(1, 'big'))

    def write_serial_16bit(self, a):
        # This is a register
        if a >= 0x4000:
            time.sleep(self.serial_delay_time)
            self.ser.write((a >> 8).to_bytes(1, 'big'))
            time.sleep(self.serial_delay_time)
            self.ser.write((a & 0x00FF).to_bytes(1, 'big'))
        # This is a number
        else:
            # Condition number
            if a < -999:
                a = -999
            elif a > 999:
                a = 999
            a += 1000
            time.sleep(self.serial_delay_time)
            self.ser.write((a >> 6).to_bytes(1, 'big'))
            time.sleep(self.serial_delay_time)
            self.ser.write((a & 0x003F).to_bytes(1, 'big'))

    def __del__(self):
        self.ser.close()
