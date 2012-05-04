from bluetooth import *
import sys

server_adress = "00:06:66:03:A9:D1"
port = 1

sock = BluetoothSocket( RFCOMM )
sock.connect((server_adress, port))

while True:
    data = raw_input("Sensordata tack:\n")
    sock.send("42".decode("hex"))
    sock.send(data.decode("hex"))

sock.close()
