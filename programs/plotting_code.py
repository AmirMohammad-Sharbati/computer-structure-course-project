import serial
import time
import matplotlib.pyplot as plt

# This is for communication between python script and wokwi simulation
ser = serial.serial_for_url('rfc2217://localhost:4000', baudrate=115200)
time.sleep(2)  

temperature = []
humidity = []

# Enable interactive plotting
plt.ion()
fig, (temp_ax, hum_ax) = plt.subplots(2, 1)
temp_ax.set_title('Real-Time Temperature and Humidity')
temp_ax.set_ylabel('Temperature (C)')
hum_ax.set_ylabel('Humidity (%)')

# Buffer for pairing temperature and humidity readings
temp = None
hum = None

while True:
    try:
        # Reading data from the serial port
        data = ser.readline().decode('utf-8').strip()

        # Parsing temperature and humidity separately
        if "Temperature" in data:
            temp = float(data.split(":")[1].strip())
        elif "Humidity" in data:
            hum = float(data.split(":")[1].strip())

        # Only update the plot when both temperature and humidity are available
        if temp is not None and hum is not None:
            temperature.append(temp)
            humidity.append(hum)
            
            if temp > 50:
                ser.write(b"WARNING\n")  # Send warning to Arduino via serial
                print("Warning: High Temperature! Sent to Arduino.")

            # Clearing and updating the temperature & humidity plot
            temp_ax.clear()
            temp_ax.plot(temperature, label='Temperature (C)', color='r')
            temp_ax.set_ylabel('Temperature (C)')
            temp_ax.legend(loc='upper right')

            hum_ax.clear()
            hum_ax.plot(humidity, label='Humidity (%)', color='b')
            hum_ax.set_ylabel('Humidity (%)')
            hum_ax.legend(loc='upper right')

            # Redraw the plot
            plt.pause(0.05)

            # Reseting temp and hum for the next pair of values
            temp = None
            hum = None

    except Exception as e:
        print(f"Error: {e}")
        break

ser.close()
