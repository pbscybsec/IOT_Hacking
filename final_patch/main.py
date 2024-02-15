from flask import Flask, request, session, render_template_string, redirect, url_for, flash
import os
import RPi.GPIO as GPIO
from itsdangerous import URLSafeTimedSerializer
import atexit

app = Flask(__name__)
app.secret_key = os.urandom(24)
serializer = URLSafeTimedSerializer(app.secret_key)

# Setup GPIO
GPIO.setmode(GPIO.BCM)
ledPin = 17
GPIO.setup(ledPin, GPIO.OUT)

#atexit.register(GPIO.cleanup)

def setup_gpio():
    GPIO.setup(ledPin, GPIO.OUT)

# Define a simple user for demonstration purposes
users = {'admin': 'ufxx'}

# Helper function to generate a CSRF token
def generate_csrf_token():
    if '_csrf_token' not in session:
        session['_csrf_token'] = serializer.dumps(str(os.urandom(24)))
    return session['_csrf_token']

app.jinja_env.globals['csrf_token'] = generate_csrf_token

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if username in users and users[username] == password:
            session['logged_in'] = True
            flash('You were successfully logged in')
            return redirect(url_for('index'))
        else:
            flash('Invalid credentials')
    return render_template_string('''
        <!DOCTYPE html>
        <html>
        <body>
            <form action="" method="post">
                <p>Username:<br><input type="text" name="username" /></p>
                <p>Password:<br><input type="password" name="password" /></p>
                <input type="hidden" name="_csrf_token" value="{{ csrf_token() }}">
                <p><input type="submit" value="Login"></p>
            </form>
        </body>
        </html>
    ''')

@app.route('/logout')
def logout():
    session.pop('logged_in', None)
    flash('You were logged out')
    return redirect(url_for('login'))

@app.route('/', methods=['GET', 'POST'])
def index():
    if not session.get('logged_in'):
        return redirect(url_for('login'))
    else:
        GPIO.setmode(GPIO.BCM)
        setup_gpio()
        message = ''
        if request.method == 'POST':
            token = session.get('_csrf_token', None)
            if not token or token != request.form.get('_csrf_token'):
                message = 'CSRF token missing or incorrect.'
            else:
                action = request.form.get('action')
                if action == 'ON':
                    GPIO.output(ledPin, GPIO.HIGH)
                    message = 'LED is on'
                elif action == 'OFF':
                    GPIO.output(ledPin, GPIO.LOW)
                    message = 'LED is off'
        return render_template_string("""
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="utf-8" /> 
                <title>LED Control</title>
            </head>
            <body>
                <p><a href="/logout">Logout</a></p>
                LED Control:<br>
                <form method="post" action="/">
                        <input type="hidden" name="_csrf_token" value="{{ csrf_token() }}">
                        <input type="submit" value="ON" name="action">
                        <input type="submit" value="OFF" name="action">
                </form>
                {{ message }}
            </body>
            </html>
        """, message=message)


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
