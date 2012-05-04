from bluetooth import *
import sys
import binascii

server_adress = "00:06:66:03:A9:D1"
port = 1

sock = BluetoothSocket( RFCOMM )
try:
    sock.connect((server_adress, port))
except:
    print "Host is down"
    sys.exit(1)

while True:
    header = (binascii.hexlify(sock.recv(1)))
    data = (binascii.hexlify(sock.recv(1)))
    print("Header is {0}".format(header))
    print("Data is {0}".format(data))

sock.close()
