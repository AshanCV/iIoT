#include "WiFi.h"
#include <HTTPClient.h>

const char* ssid = "LPV home";
const char* password = "Gq7076@slt_123";

String Web_App_URL = "https://script.google.com/macros/s/AKfycbzZxWDRR0RSKmfvK0tIdyNQr2f89WYXvqhZe5Psqap0p2SOkQLodzuc8dQhR4i99sPMOQ/exec";

String inputStates = "";
String outputStates = "";
String sendDataURL="";

#define sda 21
#define scl 22

#define in1 34
#define in2 35
#define in3 32
#define in4 33
#define in5 25
#define in6 26

#define out1 4
#define out2 16
#define out3 17
#define out4 5
#define out5 18
#define out6 19

TaskHandle_t DataTaskHandle;


void setup() {
  Serial.begin(9600);

  pinMode(in1, INPUT);
  pinMode(in2, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);
  pinMode(in5, INPUT);
  pinMode(in6, INPUT);

  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  pinMode(out5, OUTPUT);
  pinMode(out6, OUTPUT);

  digitalWrite(out1, LOW);
  digitalWrite(out2, LOW);
  digitalWrite(out3, LOW);
  digitalWrite(out4, LOW);
  digitalWrite(out5, LOW);
  digitalWrite(out6, LOW);

  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi!");

  // Create a task for sending and receiving data on core 0
  xTaskCreatePinnedToCore(
    DataTask,       // Function to run
    "Data Task",    // Task name
    8192,           // Stack size (bytes)
    NULL,           // Task input parameter
    1,              // Priority (1 = low, 5 = high)
    &DataTaskHandle,// Task handle
    0               // Run on Core 0
  );

}

void sendDataToGoogleSheets() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    Serial.println("Sending Data to Google Sheets...");
  
    http.begin(sendDataURL.c_str());
    int httpCode = http.GET();
    if (httpCode > 0) {
      // Serial.println("Google Sheets Response: " + http.getString());
    } else {
      Serial.println("Failed to send data.");
    }
    http.end();
  }
}


void getCommandsFromGoogleSheets() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String fetchURL = Web_App_URL + "?sts=read";

    Serial.println("Fetching Commands from Google Sheets...");
    http.begin(fetchURL.c_str());
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND) {
      String redirectURL = http.getLocation();  // Alternative method for fetching redirected URL
      //   Serial.println("Redirecting to: " + redirectURL);
      http.end();

      if (redirectURL.length() > 0) {
        http.begin(redirectURL.c_str());  // Follow the new redirected URL
        httpCode = http.GET();
      }
    }

    if (httpCode > 0) {
      String command = http.getString();
      Serial.println("Received Command: " + command);

      // Parse commandO2 and commandO3
      int commaIndex = command.indexOf(",");
      String commandO2 = command.substring(0, commaIndex);
      String commandO3 = command.substring(commaIndex + 1);

      if (commandO2 != "NO_COMMAND") {
        if (commandO2 == "OUT1_ON") digitalWrite(out1, HIGH);
        if (commandO2 == "OUT1_OFF") digitalWrite(out1, LOW);
        if (commandO2 == "OUT2_ON") digitalWrite(out2, HIGH);
        if (commandO2 == "OUT2_OFF") digitalWrite(out2, LOW);
      }

      if (commandO3 != "NO_COMMAND") {
        if (commandO3 == "OUT3_ON") digitalWrite(out3, HIGH);
        if (commandO3 == "OUT3_OFF") digitalWrite(out3, LOW);
        if (commandO3 == "OUT4_ON") digitalWrite(out4, HIGH);
        if (commandO3 == "OUT4_OFF") digitalWrite(out4, LOW);
      }
    } else {
      Serial.println("Failed to fetch commands.");
    }
    http.end();
  }
}


void DataTask(void *pvParameters) {
  const unsigned long dataSendInterval = 10000;   // Send data every 10 sec
  const unsigned long commandFetchInterval = 10000; // Fetch commands every 5 sec

  unsigned long lastDataSendTime = 0;
  unsigned long lastCommandFetchTime = 0;

  while (1) {
    unsigned long currentMillis = millis();

    

    // Send data every 10 seconds
    if (currentMillis - lastDataSendTime >= dataSendInterval) {
      
       sendDataURL = Web_App_URL + "?sts=write" +
                  "&in1=" + String(analogRead(in1)) +
                  "&in2=" + String(analogRead(in2)) +
                  "&in3=" + String(analogRead(in3)) +
                  "&in4=" + String(analogRead(in4)) +
                  "&in5=" + String(analogRead(in5)) +
                  "&in6=" + String(analogRead(in6)) +
                  "&o1=" + String(digitalRead(out1)) +
                  "&o2=" + String(digitalRead(out2)) +
                  "&o3=" + String(digitalRead(out3)) +
                  "&o4=" + String(digitalRead(out4)) +
                  "&o5=" + String(digitalRead(out5)) +
                  "&o6=" + String(digitalRead(out6));
                  
      sendDataToGoogleSheets();
      lastDataSendTime = currentMillis;
    }

    // Fetch commands every 5 seconds
    if (currentMillis - lastCommandFetchTime >= commandFetchInterval) {
      getCommandsFromGoogleSheets();
      lastCommandFetchTime = currentMillis;
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);

    

  }
}


void loop() {





  int i1 = analogRead(in1);
  int i2 = analogRead(in2);
  int i3 = analogRead(in3);
  int i4 = analogRead(in4);
  int i5 = analogRead(in5);
  int i6 = analogRead(in6);


  Serial.print(i1); Serial.print("\t");
  Serial.print(i2); Serial.print("\t");
  Serial.print(i3); Serial.print("\t");
  Serial.print(i4); Serial.print("\t");
  Serial.print(i5); Serial.print("\t");
  Serial.print(i6); Serial.println("\t");

 // Serial.println(sendDataURL);

  /*
    digitalWrite(out1, HIGH); delay(100); digitalWrite(out1, LOW); delay(200);
    digitalWrite(out2, HIGH); delay(100); digitalWrite(out2, LOW); delay(200);
    digitalWrite(out3, HIGH); delay(100); digitalWrite(out3, LOW); delay(200);
    digitalWrite(out4, HIGH); delay(100); digitalWrite(out4, LOW); delay(200);
    digitalWrite(out5, HIGH); delay(100); digitalWrite(out5, LOW); delay(200);
    digitalWrite(out6, HIGH); delay(100); digitalWrite(out6, LOW); delay(1000);
  */

}