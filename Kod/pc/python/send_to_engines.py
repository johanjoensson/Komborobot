from bluetooth import *
import sys
import curses

server_adress = "00:06:66:03:A9:D1"
port = 1

sock = BluetoothSocket( RFCOMM )
try:
    sock.connect((server_adress, port))
except:
    print "Host is Down"
    sys.exit(1)
header = "40".decode("hex")
data = "00".decode("hex")

screen = curses.initscr()
curses.noecho()
curses.curs_set(0)
screen.keypad(1)

def send_command(socket, header, data):
    socket.send(header)
    socket.send(data)

def print_sensor_info(socket):
    header = socket.recv(1)
    data = socket.recv(1)
    top_pos = 2
    left_pos = 30
    screen.addstr("Sensor: " + data)

screen.addstr("Idle")

#print_sensor_info(sock)

try:
    while True: 
        event = screen.getch() 
        if event == ord("p"):
            break 
        if event == curses.KEY_EXIT:
            break 
        if event == ord("q"): 
            screen.clear() 
            screen.addstr("Framat vanster") 
            data = "28".decode("hex")
            send_command(sock,header,data)
        if event == ord("w"): 
            screen.clear() 
            screen.addstr("Framat") 
            data = "18".decode("hex")
            send_command(sock,header,data)
        if event == ord("e"): 
            screen.clear() 
            screen.addstr("Framat hoger") 
            data = "38".decode("hex")
            send_command(sock,header,data)
        if event == ord("a"): 
            screen.clear() 
            screen.addstr("Rotera vanster") 
            data = "48".decode("hex")
            send_command(sock,header,data)
        if event == ord("s"): 
            screen.clear() 
            screen.addstr("Back") 
            data = "68".decode("hex")
            send_command(sock,header,data)
        if event == ord("d"): 
            screen.clear() 
            screen.addstr("Rotera hoger") 
            data = "58".decode("hex")
            send_command(sock,header,data)
        if event == ord(" "): 
            screen.clear() 
            screen.addstr("Stopp")
            data = "01".decode("hex")
            send_command(sock,header,data)
        #print_sensor_info(sock)


finally:
    sock.close()
    curses.endwin()
