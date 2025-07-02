# libraries
import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports

class RobotArmControl:
    def __init__(self, root):
        self.root = root
        self.root.title("Robot Arm Servo Control")
        self.root.geometry("500x600")
        
        self.serial_connection = None
        
        self.servo_names = ["Base", "Shoulder", "Elbow", "Wrist Rotate", "Wrist Vertical", "Gripper"]
        self.num_servos = len(self.servo_names)
        self.current_positions = [90] * self.num_servos 
        
        self.create_widgets()
        self.refresh_ports()
        
    def create_widgets(self):
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        serial_frame = ttk.LabelFrame(main_frame, text="Serial Connection", padding="10")
        serial_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(serial_frame, text="Port:").grid(row=0, column=0, padx=5)
        self.port_combo = ttk.Combobox(serial_frame)
        self.port_combo.grid(row=0, column=1, padx=5)
        
        ttk.Button(serial_frame, text="Refresh", command=self.refresh_ports).grid(row=0, column=2, padx=5)
        ttk.Button(serial_frame, text="Connect", command=self.connect_serial).grid(row=0, column=3, padx=5)
        
        servo_frame = ttk.LabelFrame(main_frame, text="Servo Controls", padding="10")
        servo_frame.pack(fill=tk.BOTH, expand=True, pady=5)
        
        self.servo_sliders = []
        self.servo_labels = []
        
        for i in range(self.num_servos):
            frame = ttk.Frame(servo_frame)
            frame.pack(fill=tk.X, pady=5)
            
            label = ttk.Label(frame, text=f"{self.servo_names[i]}: 90°", width=15)
            label.pack(side=tk.LEFT, padx=5)
            self.servo_labels.append(label)
            
            slider = ttk.Scale(
                frame, 
                from_=0, 
                to=180, 
                value=90,
                command=lambda val, idx=i: self.servo_changed(idx, val)
            )
            slider.pack(fill=tk.X, expand=True, padx=5)
            self.servo_sliders.append(slider)
            
            entry = ttk.Entry(frame, width=5)
            entry.insert(0, "90")
            entry.bind("<Return>", lambda e, idx=i: self.entry_changed(idx))
            entry.pack(side=tk.RIGHT, padx=5)
            
        ttk.Button(
            main_frame, 
            text="Reset All to 90°", 
            command=self.reset_all
        ).pack(pady=10)
        
    def refresh_ports(self):
        ports = serial.tools.list_ports.comports()
        self.port_combo['values'] = [port.device for port in ports]
        if ports:
            self.port_combo.current(0)
            
    def connect_serial(self):
        port = self.port_combo.get()
        if not port:
            messagebox.showerror("Error", "No port selected!")
            return
            
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()
            
        try:
            self.serial_connection = serial.Serial(port, 115200, timeout=1)
            messagebox.showinfo("Success", f"Connected to {port}")
        except serial.SerialException as e:
            messagebox.showerror("Error", f"Failed to connect: {e}")
    
    def servo_changed(self, servo_idx, value):
        position = int(float(value))
        self.current_positions[servo_idx] = position
        self.servo_labels[servo_idx].config(text=f"{self.servo_names[servo_idx]}: {position}°")
        
        if self.serial_connection and self.serial_connection.is_open:
            command = f"{servo_idx+1},{position}\n"
            self.serial_connection.write(command.encode())
    
    def entry_changed(self, servo_idx):
        entry = self.servo_sliders[servo_idx].master.children['!entry']
        try:
            position = int(entry.get())
            if 0 <= position <= 180:
                self.servo_sliders[servo_idx].set(position)
                self.servo_changed(servo_idx, position)
            else:
                messagebox.showerror("Error", "Position must be between 0 and 180")
        except ValueError:
            messagebox.showerror("Error", "Please enter a valid number")
    
    def reset_all(self):
        for i in range(self.num_servos):
            self.servo_sliders[i].set(90)
            self.servo_changed(i, 90)
    
    def on_closing(self):
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = RobotArmControl(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()
