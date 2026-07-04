from tkinter import *
from tkinter import ttk
import serial
import threading
import struct
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
from collections import deque
import numpy as np

ser = serial.Serial(port='/dev/ttyAMA0', baudrate=57600)

SYNC = bytes([0xAA, 0xBB, 0xCC, 0xDD])
# 512 samples * 4 ints per struct (16 bytes) = 8192 bytes
PAYLOAD_SIZE = 512 * 16

# Store last 512 samples for graphing
data_buffer = {
    'original_left': deque(maxlen=512),
    'original_right': deque(maxlen=512),
    'watermark': deque(maxlen=512)
}

def read_frame():
    """Read a complete frame with 4 sync structs + 512 data structs"""
    sync_values = [0xAA, 0xBB, 0xCC, 0xDD]
    sync_count = 0
    
    while sync_count < 4:
        # Read one 16-byte struct
        struct_bytes = ser.read(16)
        
        if len(struct_bytes) < 16:
            continue
        
        # Unpack as 4 int32s (little-endian)
        values = struct.unpack('<iiii', struct_bytes)
        
        print(f"Struct {sync_count}: {[hex(v & 0xFF) for v in values]}")
        
        # Check if ALL 4 fields match the expected sync value
        expected = sync_values[sync_count]
        if all((v & 0xFF) == expected for v in values):
            sync_count += 1
        else:
            sync_count = 0
    
    # Now read the 512 data structs
    payload = b''
    for i in range(512):
        payload += ser.read(16)
    return payload

def parse_payload(payload):
    """Parse struct SignalComparator array from payload"""
    samples = []
    # Each struct is 16 bytes (4 ints)
    for i in range(0, len(payload), 16):
        if i + 16 <= len(payload):
            # Unpack 4 int32s (little-endian)
            sof_value, original_left, original_right, watermark = struct.unpack(
                '<iiii', payload[i:i+16]
            )
            samples.append({
                'original_left': original_left,
                'original_right': original_right,
                'watermark': watermark,
                'sof_value': sof_value
            })
    return samples

def update_graph():
    """Update the matplotlib graph"""
    ax.clear()
    
    if len(data_buffer['original_left']) > 0:
        x = np.arange(len(data_buffer['original_left']))
        
        ax.plot(x, list(data_buffer['original_left']), label='Original Left', alpha=0.5)
        ax.fill_between(x, list(data_buffer['original_left']), facecolor = 'blue', alpha = 0.5)
        ax.plot(x, list(data_buffer['original_right']), label='Original Right', alpha=0.5)
        ax.fill_between(x, list(data_buffer['original_right']), facecolor = 'red', alpha = 0.5)
        ax.plot(x, list(data_buffer['watermark']), label='Watermark', alpha=0.5)
        ax.fill_between(x, list(data_buffer['watermark']), facecolor = 'green', alpha = 0.5)

        
        ax.set_xlabel('Sample')
        ax.set_ylabel('Amplitude')
        ax.set_title('Real-time Audio Comparison')
        ax.legend(loc='upper right')
        ax.grid(True)
    
    canvas.draw()

def rx_thread():
    """Background thread that reads UART data"""
    while True:
        try:
            payload = read_frame()
            samples = parse_payload(payload)
            
            # Add samples to buffer
            for sample in samples:
                data_buffer['original_left'].append(sample['original_left'])
                data_buffer['original_right'].append(sample['original_right'])
                data_buffer['watermark'].append(sample['watermark'])
            
            # Schedule graph update on main thread
            root.after(0, update_graph)
            
        except Exception as e:
            print(f"Error: {e}")

def sendUART(*args):
    try:
        text = message.get()
        ser.write(SYNC + text.encode('utf-8'))
    except ValueError:
        pass

# Create GUI
root = Tk()
root.title("Kryptophony - Real-time Audio Watermark Viewer")
root.geometry("1000x600")

mainframe = ttk.Frame(root, padding=(3, 3, 12, 12))
mainframe.grid(column=0, row=0, sticky=(N, W, E, S))

# Control panel
message = StringVar()
message_entry = ttk.Entry(mainframe, width=20, textvariable=message)
message_entry.grid(column=0, row=0, sticky=(W, E), padx=5, pady=5)

ttk.Button(mainframe, text="Send", command=sendUART).grid(column=1, row=0, sticky=W, padx=5)

# Graph area
fig = Figure(figsize=(10, 5), dpi=100)
ax = fig.add_subplot(111)

canvas = FigureCanvasTkAgg(fig, master=mainframe)
canvas.get_tk_widget().grid(column=0, row=1, columnspan=2, sticky=(N, W, E, S), padx=5, pady=5)

# Status label
status_label = ttk.Label(mainframe, text="Waiting for data...")
status_label.grid(column=0, row=2, columnspan=2, sticky=W, padx=5, pady=5)

# Configure grid weights
root.columnconfigure(0, weight=1)
root.rowconfigure(0, weight=1)
mainframe.columnconfigure(0, weight=1)
mainframe.rowconfigure(1, weight=1)

message_entry.focus()
root.bind("<Return>", sendUART)

# Start RX thread as daemon
t = threading.Thread(target=rx_thread, daemon=True)
t.start()

root.mainloop()
ser.close()
