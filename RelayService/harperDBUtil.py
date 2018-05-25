import requests
import json
import datetime

HDB_URL = "http://localhost:9925/"

def getHeaders():
    headers = {
        'content-type': "application/json",
        'authorization': "Basic SERCX0FETUlOOmhhcnBlcg==",
        'cache-control': "no-cache",
        }
    return headers

def getSelectPayload():
    payload = {}
    payload["operation"] = 'sql'
    payload["sql"] = "select * from track.patient_info"
    return json.dumps(payload)

def getInsertPayload(table, photonID, location, location_prob):
    payload = {}
    payload["operation"] = "insert"
    payload["schema"] = "track"
    payload["table"] = table

    records = []
    record = {}
    record["photon_id"] = photonID
    record["location"] = location
    record["location_prob"] = location_prob
    record["timestamp"] = str(datetime.datetime.now())
    records.append(record)
    payload["records"] = records

    return json.dumps(payload)

def getLogsInsertPayload(table, photonID, fingerprint):
    payload = {}
    payload["operation"] = "insert"
    payload["schema"] = "track"
    payload["table"] = table

    records = []
    record = {}
    record["photon_id"] = photonID
    record["fingerprint"] = fingerprint
    record["timestamp"] = str(datetime.datetime.now())
    records.append(record)
    payload["records"] = records

    return json.dumps(payload)

def getPatientInfo():
    payload = getSelectPayload()
    headers = getHeaders()
    response = requests.request("POST", HDB_URL, data=payload, headers=headers)
    pList = response.json()
    print(pList)

def postLocation(photonID, location, location_prob):
    payload = getInsertPayload("photon_location", photonID, location, location_prob)
    headers = getHeaders()
    response = requests.request("POST", HDB_URL, data=payload, headers=headers)
    print(response.text)

def postFindLogs(photonID, fingerprint):
    payload = getLogsInsertPayload("find_logs", photonID, fingerprint)
    headers = getHeaders()
    response = requests.request("POST", HDB_URL, data=payload, headers=headers)
    print(response.text)
