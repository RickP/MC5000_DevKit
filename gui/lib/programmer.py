from subprocess import Popen, PIPE, STDOUT
import time

p = Popen(['easypdkprog', 'start'], stdout=PIPE, stdin=PIPE, stderr=PIPE)

while (True):
    time.sleep(1)
    print(p.communicate(input=b"0"))