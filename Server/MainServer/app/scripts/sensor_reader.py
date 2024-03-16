import serial
import requests
import json

dummy_data = '{"Temp": "25.3", "Hum": "60.5", "Gas": "03.3"}'
print(dummy_data)
# ser = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)
# data = ser.readline().decode('utf-8').rstrip()
parsed_data = json.loads(dummy_data)
response = requests.post('http://192.168.0.59:5000/api/receive-energy-data', json=parsed_data)
print(response.json()) 