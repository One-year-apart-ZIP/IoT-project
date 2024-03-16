from flask import Flask
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://pi:raspberry@localhost/my_iot_db'

import pymysql
pymysql.install_as_MySQLdb()

db = SQLAlchemy(app)

from app import views, models
