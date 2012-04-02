from bluetooth import *
import sys

server_adress = "00:06:66:03:A9:D1"
port = 1

sock = BluetoothSocket( RFCOMM )
sock.connect((server_adress, port))

while True:
    data = raw_input("Styrdata tack:\n")
    if len(data) != 2:
        print "Enter two digits please"
        continue
    else:
        sock.send("40".decode("hex"))
        sock.send(data.decode("hex"))

sock.close()
