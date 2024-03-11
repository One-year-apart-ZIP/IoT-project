import cv2
import os

cam = cv2.VideoCapture(0)
cam.set(3, 640)
cam.set(4, 480)
face_detector1 = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
face_detector2 = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_profileface.xml')

face_id = input('\n enter user id end press <return> ==> ')  # 저장할 파일 이름 설정
print("\n [INFO] Initializing face capture. Look the camera and wait ...")

count1 = 0
count2 = 0

while True:  # 정면 사진 50장 저장
    ret, img = cam.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces1 = face_detector1.detectMultiScale(gray, 1.3, 5)
    for (x,y,w,h) in faces1:
        cv2.rectangle(img, (x,y), (x+w,y+h), (255,0,0), 2)
        count1 += 1
        cv2.imwrite("/home/yk/openvino_notebooks/datasets/test/" + str(face_id) + '_' + "f" + '_' + str(count1) + ".jpg", gray[y:y+h,x:x+w])
        cv2.imshow('image', img)

    k = cv2.waitKey(100) & 0xff
    if k == 27:
        break
    elif count1 >= 50:
        break

while True:  # 측면 사진 50장 저장
    ret, img = cam.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces2 = face_detector2.detectMultiScale(gray, 1.3, 5)
    for (x,y,w,h) in faces2:
        cv2.rectangle(img, (x,y), (x+w,y+h), (255,0,0), 2)
        count2 += 1
        cv2.imwrite("/home/yk/openvino_notebooks/datasets/test/" + str(face_id) + '_' + "p" + '_'  + str(count2) + ".jpg", gray[y:y+h,x:x+w])
        cv2.imshow('image', img)

    k = cv2.waitKey(100) & 0xff
    if k == 27:
        break
    elif count2 >= 50:
        break

print("\n [INFO] Exiting Program and cleanup stuff")
cam.release()
cv2.destroyAllWindows()