import cv2
import time
import os
import requests

url = 'http://192.168.137.91:5000/upload'

# Load frontal face recognizer
frontal_recognizer = cv2.face.LBPHFaceRecognizer_create()
frontal_recognizer.read('/home/pi/ai/train/frontal_trainer.yml')

# Load profile face recognizer
profile_recognizer = cv2.face.LBPHFaceRecognizer_create()
profile_recognizer.read('/home/pi/ai/train/profile_trainer.yml')

# Load frontal face cascade classifier
frontal_cascadePath = cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
frontal_faceCascade = cv2.CascadeClassifier(frontal_cascadePath)

# Load profile face cascade classifier
profile_cascadePath = cv2.data.haarcascades + 'haarcascade_profileface.xml'
profile_faceCascade = cv2.CascadeClassifier(profile_cascadePath)

font = cv2.FONT_HERSHEY_SIMPLEX

id = 0

# Mapping of IDs to names for frontal faces
frontal_id_to_names = ['jeonghwan', 'Thief']

# Mapping of IDs to names for profile faces
profile_id_to_names = ['jeonghwan', 'Thief']

cam = cv2.VideoCapture(0)
cam.set(3, 640)
cam.set(4, 480)
cam_count = 0
cnt_f_o = 0
cnt_f_x = 0
cnt_p_o = 0
cnt_p_x = 0

minW = 0.1 * cam.get(3)
minH = 0.1 * cam.get(4)

record_start_time = None
record_duration = 200
video_writer = None
image_save_dir = '/home/pi/ai/intruder_face'

while True:
    cam_count += 1
    ret, img = cam.read()

    if not ret:
        print('not found')

    if cam_count < 30:
        continue

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    current_time = time.time()

    # Detect frontal faces
    frontal_faces = frontal_faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=6,
        minSize=(int(minW), int(minH)),
    )

    # Detect profile faces
    profile_faces = profile_faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=6,
        minSize=(int(minW), int(minH)),
    )

    if record_start_time is not None and current_time - record_start_time >= record_duration:
        record_start_time = None
        if video_writer is not None:
            video_writer.release()

    # Draw rectangles and recognize frontal faces
    if cam_count % 5 == 0:
        for (x, y, w, h) in frontal_faces:
            id, confidence = frontal_recognizer.predict(gray[y:y + h, x:x + w])
            print((100-confidence))
            if confidence < 60:
                id = frontal_id_to_names[id]
                confidence_text = " {0}%_f".format(round(100 - confidence))
                cnt_f_o += 1

            else:
                id = frontal_id_to_names[1]
                confidence_text = " {0}%_f".format(round(100 - confidence))
                cnt_f_x += 1

            if id == 'Thief':
                cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 255), 2)
                cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (255, 0, 255), 2)
                cv2.putText(img, str(confidence_text), (x + 5, y + h - 2), font, 1, (255, 0, 255), 2)
                image_filename = os.path.join(image_save_dir, f"intruder_frontal_face_{time.time()}_{cnt_f_x}.jpg")
                cv2.imwrite(image_filename, img)

                if record_start_time is None:
                    record_start_time = time.time()
                    video_filename = f"/home/pi/ai/intruder_rec/intruder_frontal_{time.time()}_{cnt_f_x}.avi"
                    video_writer = cv2.VideoWriter(video_filename, cv2.VideoWriter_fourcc(*'MJPG'), 20, (640, 480))

            else: # id = 'jeonghwan'
                cv2.rectangle(img, (x, y), (x + w, y + h), (100, 0, 40), 2)
                cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (100, 0, 40), 2)
                cv2.putText(img, str(confidence_text), (x + 5, y + h -2), font, 1, (100, 0, 40), 2)

            if video_writer is not None:
                video_writer.write(img)
            break

    if cnt_f_o == 10 or cnt_f_x == 10:
        print("cnt_f_o : ", cnt_f_o, "cnt_f_x : ", cnt_f_x)
        if cnt_f_x == 10:
            for cnt_f_x in range(3, 7):
                files = {'file': open(image_filename, 'rb')}
                response = requests.post(url, files=files)
        break


    # Draw rectangles and recognize profile faces
    if cam_count % 5 == 0:
        for (x, y, w, h) in profile_faces:
            id, confidence = profile_recognizer.predict(gray[y:y + h, x:x + w])
            print((100-confidence))
            if confidence < 45:
                id = profile_id_to_names[id]
                confidence_text = " {0}%_p".format(round(100 - confidence))
                cnt_p_o += 1

            else:
                id = frontal_id_to_names[1]
                confidence_text = " {0}%_p".format(round(100 - confidence))
                cnt_p_x += 1

            if id == 'Thief':
                cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 255), 2)
                cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (255, 0, 255), 2)
                cv2.putText(img, str(confidence_text), (x + 5, y + h - 2), font, 1, (255, 0, 255), 2)
                image_filename = os.path.join(image_save_dir, f"intruder_profile_face_{time.time()}_{cnt_p_x}.jpg")
                cv2.imwrite(image_filename, img)

                if record_start_time is None:
                    record_start_time = time.time()
                    video_filename = f"/home/pi/ai/intruder_rec/intruder_profile_{time.time()}_{cnt_p_x}.avi"
                    video_writer = cv2.VideoWriter(video_filename, cv2.VideoWriter_fourcc(*'MJPG'), 20, (640, 480))

            else: # id = jeonghwan
                cv2.rectangle(img, (x, y), (x + w, y + h), (100, 0, 40), 2)
                cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (100, 0, 40), 2)
                cv2.putText(img, str(confidence_text), (x + 5, y + h - 2), font, 1, (100, 0, 40), 2)

            if video_writer is not None:
                video_writer.write(img)
            break

    if cnt_p_o == 9 or cnt_p_x == 9:
        print("cnt_p_o : ", cnt_p_o, "cnt_p_x : ", cnt_p_x)
        if cnt_p_x == 9:
            for cnt_p_x in range(3, 7):
                files = {'file': open(image_filename, 'rb')}
                response = requests.post(url, files=files)
        break

    cv2.imshow('camera', img)
    k = cv2.waitKey(33) & 0xff
    if k == 27:
        break

print("cnt_f_o : ", cnt_f_o, "cnt_f_x : ", cnt_f_x)
print("cnt_p_o : ", cnt_p_o, "cnt_p_x : ", cnt_p_x)

if video_writer is not None:
    video_writer.release()

print("\n [INFO] Exiting Program and cleanup stuff")
cam.release()
cv2.destroyAllWindows()
