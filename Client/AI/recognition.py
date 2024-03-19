import cv2
import time
import os

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
frontal_id_to_names = ['jeonghwan', 'intruder']

# Mapping of IDs to names for profile faces
profile_id_to_names = ['jeonghwan', 'intruder']

cam = cv2.VideoCapture(0)
cam.set(3, 640)
cam.set(4, 480)

minW = 0.1 * cam.get(3)
minH = 0.1 * cam.get(4)

prev_detection_time = 0
record_start_time = None
record_duration = 20 # about 30seconds
video_writer = None
image_save_dir = '/home/pi/ai/intruder_face'

while True:
    ret, img = cam.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    ### 추가
    current_time = time.time()
    elapsed_time = current_time - prev_detection_time

    # Detect frontal faces
    frontal_faces = frontal_faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.2,
        minNeighbors=5,
        minSize=(int(minW), int(minH)),
    )

    # Detect profile faces
    profile_faces = profile_faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.2,
        minNeighbors=5,
        minSize=(int(minW), int(minH)),
    )

    prev_detection_time = current_time

    if record_start_time is not None and current_time - record_start_time >= record_duration:
        record_start_time = None
        if video_writer is not None:
            video_writer.release()

    # Draw rectangles and recognize frontal faces
    for (x, y, w, h) in frontal_faces:
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
        id, confidence = frontal_recognizer.predict(gray[y:y + h, x:x + w])
        
        if confidence < 70:
            id = frontal_id_to_names[id]
            confidence_text = " {0}%_f".format(round(100 - confidence))
        else:
            id = frontal_id_to_names[1]
            confidence_text = " "

        cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (255, 255, 255), 2)
        cv2.putText(img, str(confidence_text), (x + 5, y + h - 2), font, 1, (255, 255, 0), 1)

        ### 추가
        if id == 'intruder':
            image_filename = os.path.join(image_save_dir, f"intruder_face_{int(current_time)}.jpg")
            cv2.imwrite(image_filename, img)

            if record_start_time is None:
                record_start_time = time.time()
                vidoe_filename = f"/home/pi/ai/intruder_rec/intruder_frontal_{int(record_start_time)}.avi"
                video_writer = cv2.VideoWriter(vidoe_filename, cv2.VideoWriter_fourcc(*'XVID'), 20, (640, 480))

        if video_writer is not None:
            video_writer.write(img)

    # Draw rectangles and recognize profile faces
    for (x, y, w, h) in profile_faces:                                                                                          
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
        id, confidence = profile_recognizer.predict(gray[y:y + h, x:x + w])

        if confidence < 70:
            id = profile_id_to_names[id]
            confidence_text = " {0}%_p".format(round(100 - confidence))
        else:
            id = frontal_id_to_names[1]
            confidence_text = " "

        cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (255, 255, 255), 2)
        cv2.putText(img, str(confidence_text), (x + 5, y + h - 2), font, 1, (255, 255, 0), 1)

        ### 추가
        if id == 'intruder':
            image_filename = os.path.join(image_save_dir, f"intruder_face_{int(current_time)}.jpg")
            cv2.imwrite(image_filename, img)

            if record_start_time is None:
                record_start_time = time.time()
                vidoe_filename = f"/home/pi/ai/intruder_rec/intruder_profile_{int(record_start_time)}.avi"
                video_writer = cv2.VideoWriter(vidoe_filename, cv2.VideoWriter_fourcc(*'XVID'), 20, (640, 480))

        if video_writer is not None:
            video_writer.write(img)

    cv2.imshow('camera', img)
    k = cv2.waitKey(10) & 0xff
    if k == 27:
        break

### 추가
if video_writer is not None:
    video_writer.release()

print("\n [INFO] Exiting Program and cleanup stuff")
cam.release()
cv2.destroyAllWindows()
