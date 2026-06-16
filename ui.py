from tkinter import *
from tkinter import ttk
import serial
import threading
import struct

ser = serial.Serial(port='/dev/ttyAMA0', baudrate=9600)

SYNC = bytes([0xAA, 0xBB, 0xCC, 0xDD])
PAYLOAD_SIZE = 512 * 4  # 512 int32s, 4 bytes each

def read_frame():
    state = 0
    while True:
        byte = ser.read(1)[0]  # read one byte as int
        if state < 4:
            if byte == SYNC[state]:
                state += 1
            else:
                state = 0  # reset, re-hunt
                if byte == SYNC[0]:  # could be start of new sequence
                    state = 1
        else:
            # sync confirmed, now read full payload
            payload = ser.read(PAYLOAD_SIZE)
            return payload

def sendUART(*args):
    try:
        text = message.get()
        if not text.endswith('\n'):
            text = text + '\n'
        ser.write(SYNC + text.encode('utf-8'))
    except ValueError:
        pass

def rx_thread():
    while True:
        try:
            payload = read_frame()
            root.after(0, display_values, payload)  # schedule UI update on main thread
        except Exception as e:
            root.after(0, lambda e=e: rx_text.insert(END, f"Error: {e}\n"))

def display_values(payload):
    # each byte is one bit, group into 8 to get characters
    bits = ''.join(str(b) for b in payload)
    # split into 8-bit chunks and convert to characters
    chars = []
    for i in range(0, len(bits), 8):
        byte_str = bits[i:i+8]
        if len(byte_str) == 8:
            chars.append(chr(int(byte_str, 2)))
    message = ''.join(chars)

    rx_text.config(state=NORMAL)
    rx_text.insert(END, message + '\n')
    rx_text.see(END)
    rx_text.config(state=DISABLED)

root = Tk()
root.title("Kryptophony")

mainframe = ttk.Frame(root, padding=(3, 3, 12, 12))
mainframe.grid(column=0, row=0, sticky=(N, W, E, S))

message = StringVar()
message_entry = ttk.Entry(mainframe, width=10, textvariable=message)
message_entry.grid(column=2, row=1, sticky=(W, E))

ttk.Button(mainframe, text="Send", command=sendUART).grid(column=3, row=3, sticky=W)

ttk.Label(mainframe, text="Received:").grid(column=0, row=1, sticky=W)
rx_text = Text(mainframe, width=50, height=15, state=DISABLED)
rx_text.grid(column=0, row=2, columnspan=2, sticky=(N, W, E, S))

scrollbar = ttk.Scrollbar(mainframe, orient=VERTICAL, command=rx_text.yview)
scrollbar.grid(column=2, row=2, sticky=(N, S))
rx_text.config(yscrollcommand=scrollbar.set)

root.columnconfigure(0, weight=1)
root.rowconfigure(0, weight=1)
mainframe.columnconfigure(0, weight=1)
mainframe.rowconfigure(2, weight=1)

for child in mainframe.winfo_children():
    child.grid_configure(padx=5, pady=5)

message_entry.focus()
root.bind("<Return>", sendUART)

# start RX thread as daemon so it dies when the window closes
t = threading.Thread(target=rx_thread, daemon=True)
t.start()

root.mainloop()
