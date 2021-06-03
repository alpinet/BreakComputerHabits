// This #include statement was automatically added by the Particle IDE.

#include "application.h"
#include "HttpClient.h"

int max_brightness = 0;
int min_brightness = 10000;
int lightSensor = A0;


const int trigPin = D2;           //connects to the trigger pin on the distance sensor
const int echoPin = D3;           //connects to the echo pin on the distance sensor

HttpClient http;
// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
// { "Content-Type", "application/json" },
// { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
    };
    
http_request_t request;
http_response_t response;

float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);   //the trigger pin will output pulses of electricity
    pinMode(echoPin, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

}

void loop() {
    //light sensor calibration
    int counter = 0;
    int on = 1;
    while(true){
        if (millis() % 1000 == 0) {
            counter++;
            Serial.print("Calibrating: ");
            Serial.println(counter);
            delay(1);
        }
        if (millis() % 500 == 0) {
            if (on == 1) {
                on = 0;
            }
            else {
                on = 1;
            }
            delay(1);
        }
        if (counter > 10) {
            break;
        }
        int value = analogRead(A0);
        if (value > max_brightness) {
            max_brightness = value;
        }
        if (value < min_brightness) {
            min_brightness = value;
        }

    }


    while(true) {
        //distance sensor
        float distance = 0;         //stores the distance measured by the distance sensor
        float brightness = 0;       //stores the brightness measured by the light sensor
    
        distance = getDistance();   //variable to store the distance measured by the sensor (inches)
        Serial.println("Distance: " + String(distance,1) + " in");
        
        //light sensor
        int value = analogRead(A0);
        brightness = map(value,min_brightness,max_brightness,0,179);
        Serial.println("Brightness Value: " + String(brightness,1));

        Serial.println("Application>\tStart of Loop.");
        // Request path and body can be set at runtime or at setup.
        request.hostname = "18.224.96.15";
        Serial.println("aaaaaaaa");
        //request.hostname = "209.17.96.42";
        request.port = 5000;
        Serial.println("bbbbbbb");
        request.path = "/?brightness=" + String(brightness) + "&dist=" + String(distance);
        Serial.println("ccccccc");
        //request.path = "/?brightness=" + String(brightness, 1) + "&dist=" + String(distance,1);
        // The library also supports sending a body with your request:
        //request.body = "{\"key\":\"value\"}";
        // Get request
        http.get(request, response, headers);
        Serial.println("ddddddd");
        Serial.print("Application>\tResponse status: ");
        Serial.println(response.status);
        Serial.print("Application>\tHTTP Response Body: ");
        Serial.println(response.body);
        delay(1000);
    }
}
