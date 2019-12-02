
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
SoftwareSerial s(D4,D3);

int dataEMG;
int dataACC;
 
const char *host = "192.168.1.45"; //Ip para acceder al servidor Rest creado

String ssid; //nombre del router wifi al que se conecta el modulo NodeMCU
String password; //contraseña del router wifi al que se conecta el modulo NodeMCU

String idEMG = "EMG1";
String idACC = "ACC1";

String codigo = "312" //codigo para identificar el dispositivo

#define CS_PIN D8; //pin conectado al CS del shield SD
//=======================================================================
//                    Power on setup
//=======================================================================
 
void setup() {
  delay(1000);
  Serial.begin(115200);
  s.begin(9600);

   Serial.print("Initializing SD card...");

  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

   // abre el fichero que contiene el nombre de la red wifi
  myFile = SD.open("ssid.txt");
  if (myFile) {
    Serial.println("ssid.txt:");

    // lee del fichero hasta que no haya nada más en él
    while (myFile.available()) {
      ssid += (char)myFile.read();
    }
    // cierra el fichero
    myFile.close();
  } else {
    // si el fichero no se abre muestra un error
    Serial.println("error opening ssid.txt");
  }

   // abre el fichero que contiene la contraseña de la red wifi
  myFile = SD.open("password.txt");
  if (myFile) {
    Serial.println("password.txt:");

    // lee del fichero hasta que no haya nada más en él
    while (myFile.available()) {
      password += (char)myFile.read();
    }
    // cierra el fichero
    myFile.close();
  } else {
    // si el fichero no se abre muestra un error
    Serial.println("error opening password.txt");
  }

  delay(500);
  WiFi.mode(WIFI_OFF);        //Previene problemas de reconexión (necesitar demasiado tiempo para conectarse)
  delay(1000);
  WiFi.mode(WIFI_STA);        //Esta línea oculta la visualización de ESP como punto de acceso wifi
  
  WiFi.begin(ssid, password);     //Conexión al router WiFi
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

    JSONencoder["codigo"] = codigo;
    
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

    JSONencoder["codigo"] = codigo;
   
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
