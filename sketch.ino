// Include modules to use
#include "DHT.h"
#include "ESP8266WiFi.h"

// Definitions for using the Temp sensor
#define DHTPIN 5 // Pin D0 
#define DHTTYPE DHT22 // DHT11/DHT22
DHT dht(DHTPIN, DHTTYPE);

// Declaration of Variables
// Temp Sensor
float hum;  //Stores humidity value
float temp; //Stores temperature value
//WiFi configuration
const char* ssid   = "wifi";
const char* wifiPW = "123456";
WiFiClient client;
//IPAddress server(192, 168, 1 ,1);
char server[]="192.168.1.100";
int serverport = 80;
IPAddress wifiIP;

// the setup function runs once when you press reset or power the board
void setup() {
  // Serial configuration
  Serial.begin(9600);
  Serial.println("Read Temperature and upload the data");
  
  // Temp Sensor Configuration 
  dht.begin();
  
  // WiFi Setup
  WiFi.mode(WIFI_STA);
  Serial.print("Connect to Wi-Fi ");
  Serial.print(ssid);
  Serial.print(" ");
  // Start Wi-Fi connection
  WiFi.begin(ssid, wifiPW);
  // Wait till Wi-Fi connection is established
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  wifiIP = WiFi.localIP();
  Serial.println(wifiIP); 
}

void httpconnect() {
  int clientconnect = client.connect(server, serverport);
  if (clientconnect == 1) 
  {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /index.html HTTP/1.1");    
    client.print("Host: "); 
    client.println(server);
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println();
    while(client.connected()) {
      while(client.available()){
        char c = client.read();
        Serial.print(c);
      }
    }    
  }
  else 
  {
    Serial.println("Could not establish connection to Server");
  }
  Serial.println("Stop client");
  client.stop();  
}

// the loop function runs over and over again forever
void loop() {
 delay(15000);
 httpconnect(); 
 // Wait a few seconds between measurements.
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("\n");
  Serial.print("\nHumidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

}
