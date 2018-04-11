from flask import Flask
from flask import request
from flask import Response
from flask import jsonify

import requests
import json
from harperDBUtil import *

app = Flask(__name__)

#CONFIG
FIND_URL = "http://localhost:18003/track"


@app.route("/")
def hello():
    return "Hello World!"


@app.route("/track_location", methods=['POST'])
def track_location():
    if (request.is_json):
        content = request.get_json()

    photonID = content["photonID"]
    #POST to FIND
    try:
        r = requests.post(FIND_URL, data =json.dumps(content))
        resp = r.json()
        location = resp["location"]
        location_prob = resp["rf"]
        print(resp["location"])
        print("\n")
    except:
        errText = "Track Request failed. FIND URL: " + FIND_URL
        print(errText)
        resp = jsonify(errText)
        resp.status_code = 500
        return resp



    #POST to HDB
    try:
        postLocation(photonID, location, location_prob)
    except:
        errText = "Harper DB insert failed. PhotonID: " + photonID + ". Location: " + location
        print(errText)
        resp = jsonify(errText)
        resp.status_code = 500
        return resp

    resp = jsonify(location)
    resp.status_code = 200
    return resp
    

if __name__ == "__main__":
    app.run()
