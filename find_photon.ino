#include "application.h"
#include "HttpClient/HttpClient.h"

/**
* Declaring the variables.
*/
unsigned int nextTime = 0;    // Next time to contact the server
HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t findHeaders[] = {
    { "Content-Type", "application/json" },
    { "Accept" , "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t findRequest;
http_response_t findResponse;

http_header_t hdbHeaders[] = {
    { "Content-Type", "application/json" },
    { "Accept" , "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t hdbRequest;
http_response_t hdbResponse;

void setup() {
    Serial.begin(9600);
}

void loop() {

    if (nextTime > millis()) {
        return;
    }

    // DEBUGGING
    Serial.println();
    Serial.println("Application>\tStart of Loop.");

    Serial.println("Send track request");
    // Change hostname
    findRequest.hostname = "b68b9b19.ngrok.io";
    findRequest.port = 80;
    findRequest.path = "/track";


    findRequest.body = "{\"group\":\"home\",\"username\":\"user\",\"wifi-fingerprint\":[";
    WiFiAccessPoint aps[20];
    int found = WiFi.scan(aps, 20);
    for (int i=0; i<found; i++) {
        WiFiAccessPoint& ap = aps[i];
        char mac[17];
        sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",
         ap.bssid[0] & 0xff, ap.bssid[1] & 0xff, ap.bssid[2] & 0xff,
         ap.bssid[3] & 0xff, ap.bssid[4] & 0xff, ap.bssid[5] & 0xff);
        findRequest.body = findRequest.body + "{\"mac\":\"" + mac + "\",";
        float f = ap.rssi;
        String sf(f, 0);
        findRequest.body = findRequest.body + "\"rssi\":" + sf + "}";

        if (i < found -1 ) {
            findRequest.body = findRequest.body + ",";
        }
    }
    findRequest.body = findRequest.body + "]}";

    http.post(findRequest, findResponse, findHeaders);

    // DEBUGGING
    // Serial.println("Fingerprint:");
    // Serial.println(request.body);
    Serial.print("FIND track>\tResponse status: ");
    Serial.println(findResponse.status);
    Serial.print("FIND track>\tHTTP Response Body: ");
    Serial.println(findResponse.body);


    // HarperDB request
    Serial.println("Write track response to HDB with timestamp");
    hdbRequest.hostname = "b68b9b19.ngrok.io";
    hdbRequest.port = 80;
    hdbRequest.path = "/track";

    // add your code here
    // location info will be in findResponse.body

    nextTime = millis() + 15000; // sends response every 15 seconds

}
