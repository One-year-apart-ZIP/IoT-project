from app import db
from datetime import datetime

class UploadedFile(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    filename = db.Column(db.String(120), nullable=False)
    filepath = db.Column(db.String(120), nullable=False)
    timestamp = db.Column(db.DateTime, index=True, default=datetime.utcnow)

    def __repr__(self):
        return '<UploadedFile {}>'.format(self.filename)

class EnergyData(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    curDate = db.Column(db.String(255), default=lambda: datetime.now().strftime("%Y%m%d-%H%M%S"))
    Temp = db.Column(db.Float, nullable=False)
    Hum = db.Column(db.Float, nullable=False)
    Gas = db.Column(db.Float, nullable=False)

    def __repr__(self):
        return f'<EnergyData {self.id}>'
