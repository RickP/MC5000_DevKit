import serial
import time

class MCUControl():

    def checkString(self, a):
        if isinstance(a, str):
            try:
                a = int(a)
            except:
                a = a.encode('utf-8')[0]
        return a

    def UploadCode(self, code, mcu_id, serial_port):
        serial_delay_time = 0.01

        ser = serial.Serial(serial_port)  # open serial port

        ser.reset_input_buffer()

        def write_start_prog(cpu_serial):
            time.sleep(serial_delay_time)
            ser.write(b'$')
            time.sleep(serial_delay_time)
            ser.write(cpu_serial.encode('utf-8')[0].to_bytes(1, 'big'))

        def write_serial_8bit(a):
            a = self.checkString(a)

            time.sleep(serial_delay_time)
            ser.write(a.to_bytes(1, 'big'))

        def write_serial_16bit(a):
            a = self.checkString(a)

            time.sleep(serial_delay_time)
            ser.write((a >> 8).to_bytes(1, 'big'))
            time.sleep(serial_delay_time)
            ser.write((a & 0x00FF).to_bytes(1, 'big'))

        def write_end_prog():
            time.sleep(serial_delay_time)
            ser.write(b'&')

        write_start_prog(mcu_id)

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
                    write_serial_8bit((0x02 << 2) & (0xFC + add_to_command))
                    write_serial_16bit(parts[i+1])
                    write_serial_8bit(parts[i+2])
                    break
                if part == 'slp':
                    write_serial_8bit(0x04 << 2 & (0xFC + add_to_command))
                    write_serial_16bit(parts[i+1])
                    break

        write_end_prog()

        ser.close()
