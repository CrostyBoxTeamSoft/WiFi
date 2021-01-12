#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

DynamicJsonDocument  doc(200);

char ssid[] = "Bangladesh";
char pass[] = "Ferdous99";

int status = WL_IDLE_STATUS;

IPAddress server(91,174,249,33); 
int port = 4545;

String HOST = "91.174.249.33";

String USER_AGENT = "Arduino/1.0";
String CONTENT_TYPE = "application/json";

WiFiServer lServer(80);


WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  /*while(!Serial)
  {
    Serial.println("Wait for serial port to connect");
    delay(1000);
  }

  if(WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Could not connect to the wifi module");
    while(true);
  }

  String fv = WiFi.firmwareVersion();

  if(fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please update the firmware");
  }

  Serial.println("Scanning available networks");
  listNetworks();*/

  while(status!=WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID : ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    delay(10000);
  }

  /*Serial.println("Connected to Wifi");
  printWifiStatus();*/

  Serial.println("\nStarting connecting to server");
  
  if(client.connect(server,port))
  {
    Serial.println("Connected to server");

    postRequest("/network", listNetworks());

   int statusCode = client.responseStatusCode();
   String response = client.responseBody();

   Serial.print("Status code : ");
   Serial.println(statusCode);
   Serial.print("Response : ");
   Serial.println(response);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
   /*
   while (client.available()) {

    char c = client.read();

    Serial.write(c);

  }*/

   if (!client.connected()) {

    Serial.println();

    Serial.println("disconnecting from server.");

    client.stop();

    // do nothing forevermore:

    while (true);

  }
}

void getRequest(String path)
{
    client.println("GET "+path+" HTTP/1.1");

    client.println("Host: "+HOST);

    client.println("Connection: close");

    client.println();
}

void postRequest(String path, String postData)
{
    client.println("POST "+path+" HTTP/1.1");
    client.println("Host: "+HOST);
    client.println("User-Agent: "+USER_AGENT);
    client.println("Connection: close");
    client.println("Content-Type: "+CONTENT_TYPE);
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
}


void printWifiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your board's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}

//List all the surronding network available
String listNetworks()
{
  String data;
  DynamicJsonDocument listNetwork(200);
  Serial.println("**  Scan Networks **");

  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    Serial.println("Couldn't find a wifi connection");
    while(true);
  }

  Serial.print("Number of network found : ");
  Serial.println(numSsid);

  for(int thisNet = 0; thisNet<numSsid; thisNet++)
  {
    DynamicJsonDocument netData(200);
    Serial.print(thisNet);
    Serial.print(") ");
    
    Serial.print(WiFi.SSID(thisNet));
    netData["Name"]=WiFi.SSID(thisNet);

    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    netData["Signal"]=WiFi.RSSI(thisNet);

    Serial.print(" dBm");

    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
    netData["Encryption"]=WiFi.encryptionType(thisNet);

    listNetwork["Network"][thisNet] = netData;
  }

  serializeJson(listNetwork, data);
  return data;
}

void printEncryptionType(int thisType)
{
  switch (thisType) {

    case ENC_TYPE_WEP:

      Serial.println("WEP");

      break;

    case ENC_TYPE_TKIP:

      Serial.println("WPA");

      break;

    case ENC_TYPE_CCMP:

      Serial.println("WPA2");

      break;

    case ENC_TYPE_NONE:

      Serial.println("None");

      break;

    case ENC_TYPE_AUTO:

      Serial.println("Auto");

      break;

    case ENC_TYPE_UNKNOWN:

    default:

      Serial.println("Unknown");

      break;

  }
}

String createClient()
{
  DynamicJsonDocument doc(200);

  doc["pseudo"]="Arduino Pseudo";
  doc["mdp"]="ArduinoMdp";
  doc["confirmer"]="ArduinoMdp";
  doc["email"]="arduino@mail.com";

  String json;

  serializeJson(doc, json);
  
  return json;
}
