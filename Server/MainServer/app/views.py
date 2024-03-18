from flask import render_template, request, redirect, url_for, flash, send_from_directory, jsonify, Flask
from werkzeug.utils import secure_filename
from datetime import datetime
from app import app,db
from app.models import EnergyData, UploadedFile
import os

UPLOAD_FOLDER = '/home/pi/IoT_Project/app/uploads'
ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg', 'gif', 'avi'}

app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/')
def index():
    now = datetime.now()
    current_time = now.strftime("%Y-%m-%d %H:%M:%S")
    return render_template('index.html', current_date_time=current_time)

@app.route('/door_security')
def door_security():
    files = UploadedFile.query.all()
    print(files)
    return render_template('door_security.html', files=files)

@app.route('/energy_management')
def energy_management():
    energy_data = EnergyData.query.order_by(EnergyData.curDate.desc()).limit(10).all()
    print(energy_data)
    return render_template('energy_management.html', energy_data=energy_data)

@app.route('/upload', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        if 'file' not in request.files:
            flash('No file part')
            return redirect(request.url)
        file = request.files['file']
        if file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
            file.save(filepath)
            new_file = UploadedFile(filename=filename, filepath=filepath)
            db.session.add(new_file)
            db.session.commit()
            return redirect(url_for('uploaded_file', filename=filename))
    return render_template('upload.html')

@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)

@app.route('/api/receive-energy-data', methods=['POST'])
def receive_energy_data():
    data = request.get_json()
    if not data:
        return jsonify({"message": "No data provided"}), 400

    try:
        new_energy_data = EnergyData(
            Temp=data['Temp'],
            Hum=data['Hum'],
            Gas=data['Gas']
        )
        db.session.add(new_energy_data)
        db.session.commit()
        return jsonify({"message": "Energy data saved successfully"}), 201

    except KeyError as e:
        return jsonify({"message": f"Missing key in data: {e}"}), 400

    except Exception as e:
        return jsonify({"message": str(e)}), 500

# additional sentence
@app.route('/api/latest-file', methods=['GET'])
def get_latest_file():
    files = os.listdir(UPLOAD_FOLDER)

    if not files:
        return jsonify({'message' : 'No files found'}), 404

    latest_file = max(files, key=lambda x: os.path.getmtime(os.path.join(UPLOAD_FOLDER, x)))

    latest_file_path = os.path.join(UPLOAD_FOLDER, latest_file)

    latest_file_timestamp = datetime.fromtimestamp(os.path.getmtime(latest_file_path)).strftime('%Y-%m-%d %H:%M:%S')

    return jsonify({'filename' : latest_file, 'timestamp' : latest_file_timestamp})