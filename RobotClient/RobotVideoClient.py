import socket
import threading
import numpy as np
import cv2

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


def from_bytes(buffer, pos, length):
    num = 0
    for i in range(length):
        num <<= 8
        num |= buffer[pos + length - i - 1]

    return num


HOST = '192.168.0.18'
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
STRAIGHT_MARGIN = 100
DISPLAY_WIDTH = 1920
LEFT_MARGIN = (DISPLAY_WIDTH - STRAIGHT_MARGIN) / 2
RIGHT_MARGIN = (DISPLAY_WIDTH + STRAIGHT_MARGIN) / 2


mouse_x = 0
mouse_y = 0


def mouse_event(event, x, y, flags, param):
    if event == cv2.EVENT_MOUSEMOVE:
        global mouse_x
        global mouse_y
        mouse_x = x
        mouse_y = y


def key_pressed_handle(key):
    power = 0

    if key == ord('w'):
        power = 1080 - mouse_y
    elif key == ord('s'):
        power = mouse_y - 1080

    if power != 0:
        if mouse_x < LEFT_MARGIN:
            radius = LEFT_MARGIN * 200 / (mouse_x - LEFT_MARGIN) - mouse_x + 199
        elif mouse_x > RIGHT_MARGIN:
            radius = RIGHT_MARGIN * 200 / (mouse_x - RIGHT_MARGIN) - mouse_x + DISPLAY_WIDTH - 222
        else:
            radius = STRAIGHT_RADIUS

        send_motion_cmd(radius, power)


cv2.namedWindow('disparity')

BUFFER_SIZE = 1024*1024


def recv_task():
    while True:
        data = bytearray(BUFFER_SIZE)
        view = memoryview(data)

        nbytes = robot_socket.recv_into(view, BUFFER_SIZE)

        cmd = data[0]
        if nbytes > 5 and cmd == 0xDF:
            width = from_bytes(data, 1, 2)
            height = from_bytes(data, 3, 2)
            toread = width * height + 5 - nbytes

            while toread > 0:
                view = view[nbytes:]  # slicing views is cheap
                nbytes = robot_socket.recv_into(view, toread)
                toread -= nbytes

            frame = np.frombuffer(data, dtype=np.uint8, count=width * height, offset=5)

            frame = np.reshape(frame, (width, height))
            cv2.imshow('disparity', frame)


recv_thread = threading.Thread(target=recv_task)
recv_thread.start()


while True:
    key = cv2.waitKey(1)
    if key != -1:
        key_pressed_handle(key & 0xFF)


robot_socket.close()
