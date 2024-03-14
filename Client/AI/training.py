import cv2
import numpy as np
from PIL import Image
import os

path = '/home/pi/ai/datasets/test'
frontal_face_recognizer = cv2.face.LBPHFaceRecognizer_create()
profile_face_recognizer = cv2.face.LBPHFaceRecognizer_create()

frontal_face_detector = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
profile_face_detector = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_profileface.xml')


def getImagesAndLabels(path, detector):
    imagePaths = [os.path.join(path, f) for f in os.listdir(path)]
    faceSamples = []
    ids = []
    for imagePath in imagePaths:
        PIL_img = Image.open(imagePath).convert('L')
        img_numpy = np.array(PIL_img, 'uint8')
        if detector == 'frontal':
            faces = frontal_face_detector.detectMultiScale(img_numpy)
        elif detector == 'profile':
            faces = profile_face_detector.detectMultiScale(img_numpy)
        for (x, y, w, h) in faces:
            faceSamples.append(img_numpy[y:y+h, x:x+w])
            ids.append(int(os.path.split(imagePath)[-1].split("_")[2].split(".")[0]))
    return faceSamples, ids

print("\n [INFO] Training frontal faces. Please wait ...")
frontal_faces, frontal_ids = getImagesAndLabels(path, 'frontal')
frontal_face_recognizer.train(frontal_faces, np.array(frontal_ids))

frontal_face_recognizer.write('/home/pi/ai/train/frontal_trainer.yml')
print("\n [INFO] {0} frontal faces trained.".format(len(np.unique(frontal_ids))))

print("\n [INFO] Training profile faces. Please wait ...")
profile_faces, profile_ids = getImagesAndLabels(path, 'profile')
profile_face_recognizer.train(profile_faces, np.array(profile_ids))

profile_face_recognizer.write('/home/pi/ai/train/profile_trainer.yml')
print("\n [INFO] {0} profile faces trained.".format(len(np.unique(profile_ids))))

print("\n [INFO] Exiting Program")
