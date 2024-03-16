from flask import Flask, request, jsonify 
import os

app = Flask(__name__)

UPLOAD_FOLDER = 'uploads'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

@app.route('/upload', methods=['POST'])
def upload_file():
	if 'file' not in request.files:
		return jsonify({"error": "No file part"}), 400

	file = request.files['file']

	if file.filename == '':
		return jsonify({"error": "No selected file"}), 400
	
	if file:
		filename = file.filename
		save_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
		file.save(save_path)
		return jsonify({"status": "success", "filename":filename}), 200

if __name__ == '__main__':
	app.run(host='192.168.0.59', port=5000)
