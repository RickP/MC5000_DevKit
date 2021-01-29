import serial
import time

SIGNAL_BYTE = 0x7F
COMMENT_MARKER = "#"
LABEL_MARKER = ":"
LABEL_HEXCODE = 0x10 << 2

COMMANDS = {
    "nop": {
        "hexcode": 0x01 << 2,
        "parameters": []
    },
    "mov": {
        "hexcode": 0x02 << 2,
        "parameters": ["ri", "r"]
    },
    "jmp": {
        "hexcode": 0x03 << 2,
        "parameters": ["l"]
    },
    "slp": {
        "hexcode": 0x04 << 2,
        "parameters": ["ri"]
    },
    "slx": {
        "hexcode": 0x05 << 2,
        "parameters": ["r"]
    },
    "teq": {
        "hexcode": 0x06 << 2,
        "parameters": ["ri", "ri"]
    },
    "tgt": {
        "hexcode": 0x07 << 2,
        "parameters": ["ri", "ri"]
    },
    "tlt": {
        "hexcode": 0x08 << 2,
        "parameters": ["ri", "ri"]
    },
    "tcp": {
        "hexcode": 0x09 << 2,
        "parameters": ["ri", "ri"]
    },
    "add": {
        "hexcode": 0x0A << 2,
        "parameters": ["ri"]
    },
    "sub": {
        "hexcode": 0x0B << 2,
        "parameters": ["ri"]
    },
    "mul": {
        "hexcode": 0x0C << 2,
        "parameters": ["ri"]
    },
    "not": {
        "hexcode": 0x0D << 2,
        "parameters": []
    },
    "dgt": {
        "hexcode": 0x0E << 2,
        "parameters": ["ri"]
    },
    "dst": {
        "hexcode": 0x0F << 2,
        "parameters": ["ri", "ri"]
    }
}

class MCUControl():

    def __init__(self, serial_port):
        self.serial_delay_time = 0.01
        self.ser = serial.Serial(serial_port, 19200, timeout=0.1)  # open serial port
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
            self.write_serial_8bit(self.mcu_id, False)
            bytes = self.ser.read(4)
            if len(bytes) < 4:
                return None
            ret = []
            ret.append((bytes[0] << 7 | bytes[1])-1000)
            ret.append((bytes[2] << 7 | bytes[3])-1000)
            return ret


    def UploadCode(self, code, mcu_id):
        self.serial_delay_time = 0.01

        self.mcu_id = ord(mcu_id)

        if len(code) > 1:
            self.write_start_prog(self.mcu_id)

            labels = []

            for line in code.splitlines():
                parts = line.split()

                add_to_command = 0
                for i, part in enumerate(parts):

                    if part.startswith(COMMENT_MARKER):
                        continue

                    if part.endswith(LABEL_MARKER):
                        if part in labels:
                            label_id = labels.index(part)
                        else:
                            label_id = len(labels)
                            labels.append(part)
                        self.write_serial_8bit(LABEL_HEXCODE)
                        self.write_serial_8bit(label_id)

                    # Conditions
                    if i == 0:
                        if part == '+':
                            add_to_command = 0x01;
                        if part == '-':
                            add_to_command = 0x02;

                    # commands
                    if part in COMMANDS:
                        command = COMMANDS[part]
                        self.write_serial_8bit(command["hexcode"] | add_to_command)

                        for j, parameter in enumerate(command['parameters']):
                            if parameter == 'ri':
                                self.write_ri(parts[i + j + 1])
                            elif parameter == 'r':
                                self.write_r(parts[i + j + 1])
                            elif parameter == 'l':
                                print("L", parts[i + j + 1])
                                if parts[i + j + 1] in labels:
                                    self.write_serial_8bit(labels.index(parts[i + j + 1]))
                                else:
                                    self.write_serial_8bit(len(labels))  # @FixMe: parse all labels first

                        break

            self.write_end_prog()

    def write_r(self, r):
        if r == 'x0':
            self.write_serial_8bit(0x30)
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
            self.write_serial_8bit(0x0)

    def write_ri(self, ri):
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
        elif ri == 'null':
            self.write_serial_16bit(0x0)
        else:
            self.write_serial_16bit(int(ri))

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

    def write_serial_16bit(self, a, printval=True):
        if printval:
            print(a.to_bytes(2, 'big'))

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
