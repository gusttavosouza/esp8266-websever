#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char ssid[] = "";
const char password[] = "";

//Cria o WebServer
ESP8266WebServer server(3333);

// Print infos
void printNetworkInfo() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

// HTTP Response Factory
void sendServerSucessReponse(String response) {
  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", response); 
}

void sendServerErrorReponse(String response) {
  DynamicJsonDocument jsonDoc(200);
  jsonDoc["error"] = true;
  jsonDoc["message"] = response;
  String jsonStr;
  server.sendHeader("Content-Type", "application/json");
  server.send(400, "application/json", response); 
}

// Functions called by Endpoints 
void routeNotFound() {
  sendServerErrorReponse("Resource not found.")
}

void routeSimpleGet() {
  DynamicJsonDocument jsonDoc(200);
  jsonDoc["temperatura"] = 45.0;
  String jsonStr;
  serializeJson(jsonDoc, jsonStr);
  sendServerSucessReponse(jsonStr)
}

void routeSimpleGetWithParam() {
  String parametro = server.pathArg(0);
  
  DynamicJsonDocument jsonDoc(200);
  jsonDoc["temperatura"] = 45.0;
  String jsonStr;
  serializeJson(jsonDoc, jsonStr);
  sendServerSucessReponse(jsonStr)
}

void routeSimplePost() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, body);

    // Recover body values
    String nome = jsonDoc["nome"].as<String>();
    int idade = jsonDoc["idade"].as<int>();

    // Uses body data
    String resposta = "Nome: " + nome + ", Idade: " + String(idade);
    sendServerReponse(resposta)
  } else {
    sendServerErrorReponse("Bad request.");
  }
}

// Initialize the Server and create Endpoints
void connectNetowork(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(1000);

  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected!");
}

void initializeServer() {
  // Create endpoints
  server.on("/simples-get", HTTP_GET, routeSimpleGet);
  server.on("/simple-get-params/([^/]+)", HTTP_GET, routeSimpleGetWithParam);
  server.on("/simple-post", HTTP_POST, routeSimplePost);
  server.onNotFound(routeNotFound);

  // Initialize server
  server.begin();
  Serial.println("HTTP server started");
}

// Main
void setup() {
  Serial.begin(115200);
  connectNetowork();
  initializeServer();
}

void loop() {
  printNetworkInfo();
  server.handleClient();
}















