import socket
import math
from tkinter import *

CMD_MOTOR_SETTINGS = 0x01
CMD_SET_MOTION     = 0x02


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


def send_motion_cmd(radius, power):
    cmd_bytes = bytearray()
    cmd_bytes.append(CMD_SET_MOTION)
    cmd_bytes.extend(to_bytes(radius, 4, 'little'))
    cmd_bytes.extend(to_bytes(power, 4, 'little'))

    robot_socket.send(cmd_bytes)


DEFAULT_POWER = 800
STRAIGHT_RADIUS = 0x7fffffff
STRAIGHT_MARGIN = 80
ZERO_LIMIT = 600
EXP_DIV = ZERO_LIMIT / 10
DISPLAY_WIDTH = 1920
LEFT_MARGIN = (DISPLAY_WIDTH - STRAIGHT_MARGIN) / 2
RIGHT_MARGIN = (DISPLAY_WIDTH + STRAIGHT_MARGIN) / 2


def key_pressed_handle(event):
    #print "Key pressed: " + event.char
    power = 0

    y = master.winfo_pointerxy()[1]

    if event.char == 'w':
        power = 1080 - y
    elif event.char == 's':
        power = y - 1080

    if power != 0:
        x = master.winfo_pointerxy()[0]

        if x < LEFT_MARGIN:
            #radius = - math.exp((x - LEFT_MARGIN + ZERO_LIMIT) / EXP_DIV) - 1
            radius = math.exp((x - LEFT_MARGIN + ZERO_LIMIT) / EXP_DIV) + max(0, x - LEFT_MARGIN + ZERO_LIMIT) + 1
        elif x > RIGHT_MARGIN:
            #radius = math.exp((RIGHT_MARGIN - x + ZERO_LIMIT) / EXP_DIV) + 1
            radius = -(math.exp((RIGHT_MARGIN - x + ZERO_LIMIT) / EXP_DIV) + max(0, RIGHT_MARGIN - x + ZERO_LIMIT) + 1)
        else:
            radius = STRAIGHT_RADIUS
        radius = int(radius)
        label_text.set("Radius: " + str(radius))
        send_motion_cmd(radius, power)


master = Tk()
label_text = StringVar()
Label(master, textvariable=label_text).pack()
label_text.set("Radius: N/A")
master.bind_all('<Key>', key_pressed_handle)

mainloop()

robot_socket.close()
