from threading import Thread
import socket
from time import sleep
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.button import Button


# TODO: REPORT???
# TODO: Label info at the top - network info.
# TODO: Classes for buttons? kinda messy


localaddr = ('', 41181)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# AF_INET = internet family addresses (IP), SOCK_DGRAM = udp datagram packets
sock.bind(localaddr)

rover_address = ('172.20.10.7', 2390)

control = [0, 0, 0, 0]
# Control array, corresponds to the 4 possible motor states:
# forward, backward, left, right (in that order)
move = ["forward", "backward", "left", "right"]


class ControlInterface(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    @staticmethod
    def move_forward():
        control[1] = 0  # set backward to low
        control[0] = 1

    @staticmethod
    def move_backward():
        control[0] = 0
        control[1] = 1

    @staticmethod
    def move_left():
        control[3] = 0
        control[2] = 1

    @staticmethod
    def move_right():
        control[2] = 0
        control[3] = 1

    @staticmethod
    def forward_release():
        control[0] = 0

    @staticmethod
    def backward_release():
        control[1] = 0

    @staticmethod
    def left_release():
        control[2] = 0

    @staticmethod
    def right_release():
        control[3] = 0


class rover(App):
    pass


def send():
    while True:
        for i in range(0, 4):
            if control[i] == 1:
                msg = (move[i]).encode(encoding='utf-8')
                sent = sock.sendto(msg, rover_address)
                if sent:
                    print("sent " + move[i])

        sleep(0.1)  # note that this suspends execution of only the thread


def recv():
    while True:
        try:
            data, server = sock.recvfrom(1518)
            print("recieved : " + data.decode(encoding='utf-8'))
        except Exception:
            print("\nExit...\n")
            break
        sleep(0.1)


if __name__ == "__main__":
    sendThread = Thread(target=send)
    sendThread.setDaemon(True)

    recvThread = Thread(target=recv)
    recvThread.setDaemon(True)

    sendThread.start()
    recvThread.start()

    rover().run()
