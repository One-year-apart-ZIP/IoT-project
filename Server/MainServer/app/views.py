from flask import session,render_template, request, redirect, url_for, flash, send_from_directory, jsonify, Flask
from werkzeug.utils import secure_filename
from datetime import datetime
from app import app,db
from app.models import EnergyData, UploadedFile
import os
import serial

ser = serial.Serial('/dev/ttyS0', 9600)

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
            Gas=data['Gas'],
            ac=data['ac'],
            bo=data['bo'],
            hu=data['hu']
        )
        db.session.add(new_energy_data)
        db.session.commit()
        return jsonify({"message": "Energy data saved successfully"}), 201

    except KeyError as e:
        return jsonify({"message": f"Missing key in data: {e}"}), 400

    except Exception as e:
        return jsonify({"message": str(e)}), 500

"""@app.route('/api/latest-energy-data')
def latest_energy_data():
    latest_data = EnergyData.query.order_by(EnergyData.curDate.desc()).first()
    if latest_data:
        return jsonify({
           'curDate' : latest_data.curDate.strftime("%Y-%m-%d %H:%M:%S"),
           'Temp' : latest_data.Temp,
           'Hum' : latest_data.Hum,
	   'Gas' : latest_data.Gas,
           'ac' : session.get('ac', 'N/A'),
           'bo' : session.get('bo', 'N/A'),
           'hu' : session.get('hu', 'N/A')
        })
    else:
       return jsonify({'message' : 'No data available'}), 404
"""
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

DEVICE_SIGNALS = {
    'aircon_on': '0',
    'aircon_off': '1',
    'boiler_on': '2',
    'boiler_off': '3',
    'humidifier_on': '4',
    'humidifier_off': '5',
}

@app.route('/control-device', methods=['POST'])
def control_device():
    content = request.json
    command = content.get('command')
    signal = DEVICE_SIGNALS.get(command)

    if signal:
        ser.write(signal.encode())
        return {'status': 'success', 'signal_snet': signal},200
    else:
        return {'status': 'error', 'message': 'Invalid command'}, 400

@app.route('/delete-file/<filename>', methods=['POST'])
def delete_file(filename):
    file_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)

    if os.path.exists(file_path):
        os.remove(file_path)
        flash(f'{filename} has been deleted from server.', 'success')
    else:
        flash(f'{filename} not found on server.', 'error')


    file_record = UploadedFile.query.filter_by(filename=filename).first()
    if file_record:
        db.session.delete(file_record)
        db.session.commit()
        flash(f'{filename} record has been deleted from database.', 'success')

    return redirect(url_for('door_security'))

def delete_all_photos():
    directory = os.path.join(app.config['UPLOAD_FOLDER'])
    errors = []

    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        try:
            if os.path.isfile(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
        except Exception as e:
            errors.append(f"Error deleting file {filename}: {str(e)}")

    if not errors:
        try:
            num_rows_deleted = db.session.query(UploadedFile).delete()
            db.session.commit()
            flash(f"All photos and database records deleted. {num_rows_deleted} records removed.", "success")
        except Exception as e:
            db.session.rollback()
            flash(f"Error deleting records from database: {str(e)}", "error")
    else:
        for error in errors:
            flash(error, "error")

@app.route('/delete-all-photos', methods=['POST'])
def delete_all_photos_route():
    delete_all_photos()
    return redirect(url_for('door_security'))
