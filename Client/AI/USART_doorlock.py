import serial  # 시리얼 통신을 위한 모듈
import subprocess  # 외부 프로세스 실행을 위한 모듈

# 시리얼 포트 설정 및 시리얼 객체 생성
ser = serial.Serial('/dev/ttyS0', baudrate=9600, timeout=1)

try:
    while True:  # 무한 루프 시작
        # 시리얼 포트로부터 데이터를 읽어들임
        data = ser.readline().decode('utf-8').rstrip()

        # 읽어들인 데이터가 'Capture video!'인 경우
        if data == 'Capture video!':
            print("Received ", data)  # 수신된 데이터를 출력
            # 외부 프로세스 실행 (python 스크립트 실행)
            subprocess.run(["python", "/home/pi/openvino/recognition.py"])
            print("Recognition process completed")  # 외부 프로세스 실행 완료 메시지 출력

except KeyboardInterrupt:  # 사용자가 Ctrl+C를 누르면 KeyboardInterrupt 예외가 발생하여 루프를 종료함
    ser.close()  # 시리얼 포트 닫기
