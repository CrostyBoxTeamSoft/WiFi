#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

char ssid[] = "Crosty Box";        // your network SSID (name)
char pass[] = "CrostyPass";    // your network password (use for WPA, or use as key for WEP)

char aStatut = "A";

String uSSID;
String uPass;

byte mac[6];

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
  /*
  Serial.print("Creating access point named: ");
  Serial.println(ssid);*/

  // Create open network. Change this line if you want to create an WEP network:
  /*status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }*/

  while ( status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println("Bangladesh");                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
  status = WiFi.begin("Bangladesh", "Ferdous99");
  }

  // wait 10 seconds for connection:
  //delay(10000);

  // start the web server on port 80
  //lServer.begin();

  printWiFiStatus();

  postRequest("/arduino", sendInfo());

}

void loop() {

  
  
  /*
  wifi = lServer.available();
  if(wifi)
  {
    Serial.println("New client");
    String request = "";
    while(wifi.connected())
    {
      if(wifi.available())
      {
        messageReceived(wifi);
      }
    }
    Serial.println("Client disconnected");
   }*/

   
   
}

void getRequest(String path)
{
  Serial.println("Get request");
    client.println("GET "+path+" HTTP/1.1");

    client.println("Host: "+HOST);

    client.println("Connection: close");

    client.println();
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
void listNetworks()
{
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
    Serial.print(thisNet);
    Serial.print(") ");
    
    Serial.print(WiFi.SSID(thisNet));

    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));

    Serial.print(" dBm");

    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));

  }
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

String sendInfo()
{
  DynamicJsonDocument doc(200);
  doc["mac"] = WiFi.macAddress(mac);
  doc["ip"] = String(WiFi.localIP());

  String json;
  serializeJson(doc, json);
  return json;
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

void ouvrirTrappe()
{
  Serial.println("Ouverture de la trappe");
}

void fermerTrappe()
{
  Serial.println("Fermeture de la trappe");
}

void getPoids()
{
  Serial.println("Recuperation du poids");
}

void camera()
{
  Serial.println("Allumage de la camera");
}

void hautParleur()
{
  Serial.println("Haut parleur allume");
}

void microphone()
{
  Serial.println("Microphone allumee");
}

void messageReceived(WiFiClient client)
{
  String request = client.readString();
  Serial.println("Request = "+request);
  if(request.startsWith("GET")) 
  {
    Serial.println("GET request received");
    parseGet(request);
  }

  else
  {
    Serial.println("This was not a GET request");
  }
}

void parseGet(String request)
{
  String firstLine = "";

  for(int i = 0; i<request.length(); i++)
  {
    if(request.charAt(i) != '\r')
    {
      firstLine += request.charAt(i);
    }

    else
    {
      break;
    }
  }

  char* tokened;
  char flChar[firstLine.length()];
  firstLine.toCharArray(flChar, firstLine.length());
  tokened = strtok(flChar, " &?");

  String ssid;
  String pass;
  while(tokened != NULL)
  {
    if(String(tokened).startsWith("ssid="))
    {
      uSSID = String(tokened).substring(5);
    }

    if(String(tokened).startsWith("pass="))
    {
      uPass = String(tokened).substring(5);
    }
    tokened = strtok(NULL, " &?");
  }

  wifi.println("HTTP/1.1 200 OK");
  wifi.println();

  delay(50);

  connectWifi(uSSID, uPass);
}

void connectWifi(String ssid, String pass)
{
  
  while(status!=WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID : ");
    Serial.println(ssid);
    status = WiFi.begin(ssid.c_str(), pass.c_str());
  }

  Serial.println("Connected to "+ssid);
 // aStatut = "S";  //Arduino en mode serveur
  printWiFiStatus();
  postRequest("/arduino", sendInfo());
  
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
