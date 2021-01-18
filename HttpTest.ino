/*
  Simple POST client for ArduinoHttpClient library
  Connects to server once every five seconds, sends a POST request
  and a request body
  created 14 Feb 2016
  modified 22 Jan 2019
  by Tom Igoe
  
  this example is in the public domain
 */
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = "Bangladesh";
char pass[] = "Ferdous99";

IPAddress server(91,174,249,33); 
int port = 4545;

String HOST = "91.174.249.33";

String USER_AGENT = "Arduino/1.0";
String CONTENT_TYPE = "application/json";

byte mac[6];

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println("making POST request");
  String postData = sendInfo();
  Serial.println("Post Data = "+postData);

  if(client.connect(server, port)
)  {  
      postRequest("/arduino/", postData);
      // read the status code and body of the response
      int statusCode = client.responseStatusCode();
      String response = client.responseBody();
    
      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
  }

  

  Serial.println("Wait five seconds");
  delay(5000);
}

void loop() {
  
}

void postRequest(String path, String postData)
{
    Serial.println("Post request");
    client.println("POST "+path+" HTTP/1.1");
    client.println("Host: "+HOST);
    client.println("User-Agent: "+USER_AGENT);
    client.println("Connection: close");
    client.println("Content-Type: "+CONTENT_TYPE);
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);

    int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

String sendInfo()
{
  DynamicJsonDocument doc(200);
  WiFi.macAddress(mac);
  //doc["mac"] = mac;
  doc["ip"] = String(WiFi.localIP());

  String json;
  serializeJson(doc, json);
  return json;
}
