#include "application.h"
#include "HttpClient/HttpClient.h"

/**
* Declaring the variables.
*/
unsigned int nextTime = 0;    // Next time to contact the server
HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t relayHeaders[] = {
    { "Content-Type", "application/json" },
    { "Accept" , "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t relayRequest;
http_response_t relayResponse;

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

    Serial.println("Send track request ro Relat Service");
    // Change hostname
    relayRequest.hostname = "c5617543.ngrok.io";
    relayRequest.port = 80;
    relayRequest.path = "/track_location";

    relayRequest.body = "{\"photonID\":\"" + Spark.deviceID() + "\" , \"group\":\"umbc\",\"username\":\"user\",\"wifi-fingerprint\":[";
    WiFiAccessPoint aps[20];
    int found = WiFi.scan(aps, 20);
    for (int i=0; i<found; i++) {
        WiFiAccessPoint& ap = aps[i];
        char mac[17];
        sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",
         ap.bssid[0] & 0xff, ap.bssid[1] & 0xff, ap.bssid[2] & 0xff,
         ap.bssid[3] & 0xff, ap.bssid[4] & 0xff, ap.bssid[5] & 0xff);
        relayRequest.body = relayRequest.body + "{\"mac\":\"" + mac + "\",";
        float f = ap.rssi;
        String sf(f, 0);
        relayRequest.body = relayRequest.body + "\"rssi\":" + sf + "}";

        if (i < found -1 ) {
            relayRequest.body = relayRequest.body + ",";
        }
    }
    relayRequest.body = relayRequest.body + "]}";

    Serial.print("Track>\tRequest: ");
    Serial.println(relayRequest.body);

    http.post(relayRequest, relayResponse, relayHeaders);

    // DEBUGGING
    // Serial.println("Fingerprint:");
    // Serial.println(request.body);
    Serial.print("Track>\tResponse status: ");
    Serial.println(relayResponse.status);
    Serial.print("Track>\tHTTP Response Body: ");
    Serial.println(relayResponse.body);

    // add your code here
    // location info will be in findResponse.body

    nextTime = millis() + 30000; // sends response every 15 seconds

}
