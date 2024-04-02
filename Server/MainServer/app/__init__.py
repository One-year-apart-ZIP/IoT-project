from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
import os

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://pi:raspberry@localhost/my_iot_db'
app.secret_key = os.environ.get('SECRET_KEY', 'optional_default_secret_key')

import pymysql
pymysql.install_as_MySQLdb()

db = SQLAlchemy(app)
migrate = Migrate(app, db)

from app import views, models
