from flask import Flask
from flask import request
from flask import Response
from flask import jsonify

import requests
import json
from harperDBUtil import *

import sys

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

    filter_file = open("filter.json").readlines()
    filter_macs  = [x.strip() for x in filter_file]

    fprints = list(content["wifi-fingerprint"])
    for fp in content["wifi-fingerprint"]:
        if (fp["mac"] in filter_macs):
            fprints.remove(fp)

    content["wifi-fingerprint"] = fprints

    resp = jsonify(content)
    resp.status_code = 200
    return resp

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
        postFindLogs(photonID, content)
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
    app.run(host='0.0.0.0', port=5000)
