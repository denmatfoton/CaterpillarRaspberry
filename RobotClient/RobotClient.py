import socket
from tkinter import *

CMD_MOTOR_SETTINGS = 0x01
LEFT_MOTOR_IDX = 1
RIGHT_MOTOR_IDX = 2

DIRECTION_STOP = 0
DIRECTION_FORWARD = 1
DIRECTION_REVERSE = 2


def to_bytes(num, length, order):
    res = bytearray(length)
    for i in range(length):
        res[i] = num & 0xff
        num >>= 8
    if order == 'big':
        res.reverse()
    return res


HOST = '192.168.9.161'
PORT = 4385
robot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
robot_socket.connect((HOST, PORT))


def send_motor_cmd(motor_idx, power):
    cmd_bytes = bytearray()
    cmd_bytes.append(0x01)
    cmd_bytes.append(motor_idx)

    if power == 0:
        direction = DIRECTION_STOP
    elif power < 0:
        direction = DIRECTION_REVERSE
        power = -power
    else:
        direction = DIRECTION_FORWARD

    #print("power: " + str(power) + " direction: " + str(direction) + "\n")
    cmd_bytes.append(direction)
    cmd_bytes.extend(to_bytes(power, 4, 'little'))

    robot_socket.send(cmd_bytes)


def left_motor_changed(value):
    #print "New left value: " + str(value)
    send_motor_cmd(LEFT_MOTOR_IDX, int(value))


def right_motor_changed(value):
    #print "New right value: " + str(value)
    send_motor_cmd(RIGHT_MOTOR_IDX, int(value))


def all_motor_changed(value):
    left_motor_slider.set(value)
    right_motor_slider.set(value)

#os.system('xset r off')

master = Tk()
left_motor_slider = Scale(master, from_=-1024, to=1024, length=1024, tickinterval=32, command=left_motor_changed, orient=HORIZONTAL)
left_motor_slider.set(0)
left_motor_slider.pack()
all_motor_slider = Scale(master, from_=-1024, to=1024, length=1024, tickinterval=32, command=all_motor_changed, orient=HORIZONTAL)
all_motor_slider.set(0)
all_motor_slider.pack()
right_motor_slider = Scale(master, from_=-1024, to=1024, length=1024, tickinterval=32, command=right_motor_changed, orient=HORIZONTAL)
right_motor_slider.set(0)
right_motor_slider.pack()

mainloop()

robot_socket.close()
