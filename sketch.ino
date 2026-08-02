#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* serverUrl =
  "https://waad.freepage.cc/get_state.php";

const unsigned long requestInterval = 2000;
unsigned long previousRequestTime = 0;

String lastCommand = "";


void moveForward() {
  Serial.println("ACTION: Move Forward");
  Serial.println("Robot is moving forward");
}

void moveBackward() {
  Serial.println("ACTION: Move Backward");
  Serial.println("Robot is moving backward");
}

void turnLeft() {
  Serial.println("ACTION: Turn Left");
  Serial.println("Robot is turning left");
}

void turnRight() {
  Serial.println("ACTION: Turn Right");
  Serial.println("Robot is turning right");
}

void stopRobot() {
  Serial.println("ACTION: Stop");
  Serial.println("Robot stopped");
}


void executeCommand(String command) {
  command.trim();
  command.toUpperCase();

  if (command == "F") {
    moveForward();
  }
  else if (command == "B") {
    moveBackward();
  }
  else if (command == "L") {
    turnLeft();
  }
  else if (command == "R") {
    turnRight();
  }
  else if (command == "S") {
    stopRobot();
  }
  else {
    Serial.println("Unknown command received");
    Serial.println("Robot stopped for safety");
    stopRobot();
  }
}


void connectToWiFi() {
  Serial.println();
  Serial.print("Connecting to WiFi");

  WiFi.begin(ssid, password);

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    attempts++;

    if (attempts >= 30) {
      Serial.println();
      Serial.println("WiFi connection failed. Retrying...");

      WiFi.disconnect();
      delay(1000);
      WiFi.begin(ssid, password);

      attempts = 0;
    }
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("--------------------------------");
}


void getRobotCommand() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected");
    connectToWiFi();
  }

  WiFiClientSecure client;

  client.setInsecure();

  HTTPClient http;

  Serial.println();
  Serial.println("Sending HTTP request...");
  Serial.print("URL: ");
  Serial.println(serverUrl);

  bool connectionStarted = http.begin(client, serverUrl);

  if (!connectionStarted) {
    Serial.println("Could not start HTTP connection");
    return;
  }

  http.addHeader("Cache-Control", "no-cache");
  http.addHeader("Accept", "text/plain");
  http.addHeader("User-Agent", "ESP32-Wokwi");

  http.setTimeout(10000);

  int httpCode = http.GET();

  Serial.print("HTTP Code: ");
  Serial.println(httpCode);

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();

    response.trim();
    response.toUpperCase();

    if (
  response.indexOf("<HTML") >= 0 ||
  response.indexOf("<html") >= 0 ||
  response.indexOf("<SCRIPT") >= 0 ||
  response.indexOf("<script") >= 0
) {
  Serial.println("InfinityFree security page detected.");
  Serial.println("ESP32 reached the server successfully.");
  Serial.println("The server requires JavaScript and cookies.");
  Serial.println("Expected command: F, B, L, R, or S.");
  http.end();
  return;
}

Serial.print("Server response: ");
Serial.println(response);

    if (
      response == "F" ||
      response == "B" ||
      response == "L" ||
      response == "R" ||
      response == "S"
    ) {
      Serial.print("Command received: ");
      Serial.println(response);

      if (response != lastCommand) {
        executeCommand(response);
        lastCommand = response;
      }
      else {
        Serial.println("Command has not changed");
      }
    }
    else {
      Serial.println("Invalid response received");
      Serial.println("Expected: F, B, L, R, or S");

      if (
        response.indexOf("<HTML") >= 0 ||
        response.indexOf("<!DOCTYPE") >= 0
      ) {
        Serial.println(
          "The hosting server returned an HTML page instead of the command."
        );
      }
    }
  }
  else if (httpCode > 0) {
    String response = http.getString();

    Serial.println("HTTP request reached the server but failed.");

    Serial.print("Response: ");
    Serial.println(response);
  }
  else {
    Serial.print("Request failed: ");
    Serial.println(http.errorToString(httpCode));
  }

  http.end();

  Serial.println("--------------------------------");
}


void setup() {
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP32 Robot Control");
  Serial.println("================================");

  connectToWiFi();

  getRobotCommand();

  previousRequestTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  if (
    currentTime - previousRequestTime >= requestInterval
  ) {
    previousRequestTime = currentTime;

    getRobotCommand();
  }
}