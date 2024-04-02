import serial
import requests
import json

ser = serial.Serial('/dev/ttyS0', 9600)

while 1:
	data = ser.readline().decode('utf-8').rstrip()
	if data is not None:
		parsed_data = json.loads(data)
		print(parsed_data)
		response = requests.post('http://192.168.137.91:5000/api/receive-energy-data', json=parsed_data)
		print(response.json())
