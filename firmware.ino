// This #include statement was automatically added by the Particle IDE.


// #include "application.h"
#include "HttpClient.h"

/**
* Declaring the variables.
*/
unsigned int nextTime = 0;    // Next time to contact the server
HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    { "Content-Type", "application/json" },
    { "Accept" , "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

// http_header_t headers_hdb[] = {
//     {"Authorization", "Basic YWRtaW46cm9vdA=="},
//     {"Content-Type", "application/json"},
//     {"Cache-Control", "no-cache"},
//     {"Postman-Token","5cca7e7e-ba7a-c2be-d5be-56088f49cf9f"},
//     { NULL, NULL } // NOTE: Always terminate headers will NULL
// };

http_request_t request;
http_response_t response;

// http_request_t request_hdb;
// http_response_t response_hdb;

// // SWITCH
// unsigned int SLEEP = 0;
// void button_handler(system_event_t event, int duration, void* )
// {
//     if (!duration) { // just pressed
//         RGB.control(true);
//         if (SLEEP == 0) {
//             RGB.color(255,0,0);
//             SLEEP = 1; // sleep mode on
//         } else if (SLEEP == 1) {
//             RGB.color(0,255,0);
//             SLEEP = 2; // undefined mode
//         } else {
//             RGB.color(0,0,255);
//             SLEEP = 0;
//         }
//     }
//     else {    // just released
//         RGB.control(false);
//     }
// }

void setup() {
    Serial.begin(9600);
    // // SWITCH
    // System.on(button_status, button_handler);
    
    // Subscribe to the integration response event
    Particle.subscribe("hook-response/photonData", myHandler, MY_DEVICES);
    
    
    Particle.subscribe("hook-response/locationData", myHandler, MY_DEVICES);
}

void myHandler(const char *event, const char *data) {
  // Handle the integration response
}

void loop() {
    
    // // SWITCH
    // if (SLEEP == 1) {
    //     RGB.color(255,0,0);
    // }
    // if (SLEEP == 2) {
    //     RGB.color(0,255,0);
    // }

    if (nextTime > millis()) {
        return;
    }

    // // DEBUGGING
    // Serial.println();
    // Serial.println("Application>\tStart of Loop.");

    request.hostname = "c1630902.ngrok.io";
    // request.port = 8003;
    request.path = "/track";


    request.body = "{\"group\":\"harperdb\",\"username\":\"seshu\",\"location\":\"location\",\"wifi-fingerprint\":[";
    WiFiAccessPoint aps[20];
    int found = WiFi.scan(aps, 20);
    for (int i=0; i<found; i++) {
        WiFiAccessPoint& ap = aps[i];
        char mac[17];
        sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",
         ap.bssid[0] & 0xff, ap.bssid[1] & 0xff, ap.bssid[2] & 0xff,
         ap.bssid[3] & 0xff, ap.bssid[4] & 0xff, ap.bssid[5] & 0xff);
        request.body = request.body + "{\"mac\":\"" + mac + "\",";
        float f = ap.rssi;
        String sf(f, 0);
        request.body = request.body + "\"rssi\":" + sf + "}";
        
        if (i < found -1 ) {
            request.body = request.body + ",";
        }
    }
    request.body = request.body + "]}";

    http.post(request, response, headers);
    
    
    
    // Particle.variable("Fingerprint", request.body);
    
    // Particle.variable("R_status", response.status);
    
    // Particle.variable("Response", response.body);
    
    // Particle.publish("Fingerprint", request.body, PRIVATE);
    Particle.publish("photonData", data, PRIVATE);

    delay(60000);
    
    Particle.publish("locationData", response.body, PRIVATE);
    
    delay(60000);
    // Particle.publish("Response", response.body, PRIVATE);
    
    // // DEBUGGING
    // Serial.println("Fingerprint:");
    // Serial.println(request.body);
    // Serial.print("Application>\tResponse status: ");
    // Serial.println(response.status);
    // Serial.print("Application>\tHTTP Response Body: ");
    // Serial.println(response.body);
    /*
    POST  HTTP/1.1
    Host: localhost:9925
    Authorization: Basic YWRtaW46cm9vdA==
    Content-Type: application/json
    Cache-Control: no-cache
    Postman-Token: 5cca7e7e-ba7a-c2be-d5be-56088f49cf9f
    
    {
    "operation":"insert",
    "schema":"find_dev",
    "table":"request_body",
    "records": [
      {
        "request":"{\"group\":\"bigbeagle\",\"username\":\"photon\",\"location\":\"location\"}"
      }
    ]
    
    }
    */
    
    // request_hdb.hostname = "localhost";
    // request_hdb.port = 9925;
    
    // request_hdb.body = "{\"operation\":\"insert\",\"schema\":\"find_dev\",\"table\":\"request_body\",\"records\": [  {\"request\":";
    // request_hdb.body = request_hdb.body + request.body;
    // request_hdb.body  = request_hdb.body  + "}]}"; 
    
    // http.post(request_hdb, response_hdb, headers_hdb);
    
    // // Particle.variable("req_hdb", request_hdb.body);
    // // Particle.variable("res_hdb", response_hdb.body);
    // Particle.publish("Request_HDB", request_hdb.body, PRIVATE);
    // Particle.publish("Response_HDB", response_hdb.body, PRIVATE);
    
    nextTime = millis() + 6000; // sends response every 5 seconds  (2 sec delay + ~3 sec for gathering signals)
    
    // // SWITCH
    // if (SLEEP == 1) {
    //     System.sleep(3);
    // } else {
    //     delay(3000);
    // }
}
