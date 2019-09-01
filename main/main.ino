
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);

int dataEMG;
int dataACC;
 
//Web/Server address to read/write from 
const char *host = "192.168.1.45";   

const char* ssid = "MOVISTAR_C8C7";
const char* password = "iy8iphSzj6s9qPbJtkCQ";

String idEMG = "EMG1";
String idACC = "ACC1";

//=======================================================================
//                    Power on setup
//=======================================================================
 
void setup() {
  delay(1000);
  Serial.begin(115200);
  s.begin(9600);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {

  emg();
  delay(200);
  acc();
  delay(3000);  //Send a request every 3 seconds
 
}

//=======================================================================
//                   Functions for the sensors
//=======================================================================

void emg(){ //function to get the data from the EMG and send it to the REST API

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
 
    JSONencoder["idSensor"] = idEMG;
    dataEMG = analogRead(A0); 
    if(dataEMG > 30){
      dataEMG = 30;
    }
    String valor = (String) dataEMG;

    JSONencoder["valor"] = valor;
    
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://192.168.1.45:8080/api/fechas");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    
    String payload = http.getString();                                        //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
  
}

void acc(){ //function to get the data from the step counter and send it to the REST API

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
 
    JSONencoder["idSensor"] = idACC;

    dataACC=s.read();
    Serial.println(dataACC);

    String valor = (String) dataACC;

    JSONencoder["valor"] = valor;
    
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://192.168.1.45:8080/api/fechas");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    
    String payload = http.getString();                                        //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
  s.end();
  s.begin(9600);
  
}
//=======================================================================
