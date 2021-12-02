#!/usr/bin/env python3

import socket

HOST = '192.168.18.91'  # The server's hostname or IP address
PORT = 80        # The port used by the server
print("Trying to connect")
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'\x23\x23\x01\x00\x40')
    data = s.recv(1024)

print('Received', repr(data))