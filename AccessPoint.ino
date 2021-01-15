/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will create a new access point (with no password).
  It will then launch a new server and print out the IP address
  to the Serial monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 13.

  If the IP address of your board is yourAddress:
    http://yourAddress/H turns the LED on
    http://yourAddress/L turns it off

  created 25 Nov 2012
  by Tom Igoe
  adapted to WiFi AP by Adafruit
 */

#include <SPI.h>
#include <WiFiNINA.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Crosty Box";        // your network SSID (name)
char pass[] = "CrostyPass";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

String uSSID;
String uPass;

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // by default the local IP address of will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  printWiFiStatus();
}


void loop() {
  
  // compare the previous status to the current status
  client = server.available();

  if(client)
  {
    Serial.println("New client");
    String request = "";
    while(client.connected())
    {
      if(client.available())
      {
        request = client.readString();
        delay(10);
      }

      Serial.println("Request = "+request);
    }

    Serial.println("Client disconnected");
    connectWifi("Bangladesh", "Ferdous99");
  }

  
    
  /*
    Serial.println("Wifi SSID = ");
    delay(5000);
    
    if(Serial.available()>0)
    {      
      uSSID = Serial.readString();
      Serial.println("SSID typed = "+uSSID);
      
    }

    Serial.println("Password = ");
    delay(5000);
    
    if(Serial.available()>0)
    {      
      uPass = Serial.readString();
      Serial.println("Password typed = "+uPass);
      
    }
    
    if(status == WL_CONNECTED)
    {
      Serial.println("Connected to Wifi !");
      while(1);
    }
    
    */
  
}

void connectWifi(String ssid, String pass)
{
  
  while(status!=WL_CONNECTED)
  {
    Serial.print("Attempting to connect so SSID : ");
    Serial.println(ssid);
    status = WiFi.begin(ssid.c_str(), pass.c_str());
  }

  Serial.println("Connected to "+ssid);

  
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
