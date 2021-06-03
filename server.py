from flask import Flask, render_template, make_response, flash
from flask import request
from flask import Response
from datetime import datetime

brightness = ""
dist = ""
brightness_data = []
distance_data = []
time_dist = 0

app = Flask(__name__)
app.secret_key = "hello"
@app.route("/")
def hello():
    global brightness
    global dist
    brightness = request.args.get("brightness")
    dist = request.args.get("dist")
    print("Light Value: " + str(brightness) + "\n")
    print("Distance: " + str(dist) + "\n")
    return brightness
    #s += "Distance: "+ str(dist) + "\n"
    #resp = render_template('home.html', brightness=str(brightness), dist=str(dist))
    #resp = make_response(render_template('home.html', brightness=str(brightness), dist=str(dist)))
    #return resp
    #return s

@app.route("/web")
def web():
    global brightness
    global dist
    global brightness_data
    global distance_data   
    global time_dist  
    if float(brightness) >= 150:
        flash("Surrounding is too dark. Turn on the lights to prevent eye strain.")
    if float(dist) <= 100:
        time_dist += 1
        if time_dist > 5:
            flash("Too close to the screen for too long. Move back to prevent eye strain.")
    else:
        time_dist = 0
    now = datetime.now()
    curr_time = now.strftime("%H:%M:%S")
    brightness_data.append((curr_time, float(brightness)))
    b_labels = [row[0] for row in brightness_data]
    b_values = [row[1] for row in brightness_data]
    distance_data.append((curr_time, float(dist)))
    d_labels = [row[0] for row in distance_data]
    d_values = [row[1] for row in distance_data]
    resp = make_response(render_template('home.html',d_labels=d_labels, d_values=d_values,b_labels=b_labels, b_values=b_values, brightness = str(brightness), dist=str(dist)))
    return resp

if __name__ == '__main__':
    app.run()

