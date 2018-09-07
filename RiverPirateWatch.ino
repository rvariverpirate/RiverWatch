/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>

// Setup ESP pins for LCD display
const int rs = 13, en = 15, d4 = 12, d5 = 14, d6 = 2, d7 = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char* ssid     = "BearCave";//"Tripp's iPhone";//"BearCave";
const char* password = "Riles25Marie23Clover18Cleopatra";//"9m5vugemuhx2z";//"Riles25Marie23Clover18Cleopatra";

const char* host = "rvariverpirate.com";

// Create a URI for the request
String url = "/assets/data/RiverInfo.html";

void setup() {

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  // Set the background color
  digitalWrite(D1, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(D2, HIGH);

  
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;
int p_openIndex = 0;
int p_closeIndex = 1;
void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  int counter = 0;
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    Serial.println("\r\n");
    Serial.println(String(counter));
    String testLine = " blah <p>cat</p>";
    if(checkForTag("<p>", line)){
      p_openIndex = getHtmlTagIndex("<p>", line);
      p_closeIndex = getHtmlTagIndex("</p>", line);
      String interest = line.substring(p_openIndex, p_closeIndex);
      Serial.print(interest);
      for(int i = 0; i < 3; i++){
        scrollToLeftLargeString(interest, 250);
        }
      }
    counter ++;
  }
  
  Serial.println();
  Serial.println("closing connection");
}



// Function to scroll large texts accross the LCD display
void scrollToLeftLargeString(String str, int delayTime){
  String tempString = "                 ";
  lcd.clear();
  for(int i = 0; i < str.length() + 32; i++)
  {
    if(i < str.length()){
      tempString += str[i];// add current char to temporary string
    }
    else
    {
      tempString += " ";// let the last  16 chars scroll across the screen
    }
    if(i > 16)// string is loaded
    {
      tempString.remove(0,1);// remove the first character in the temp string
      lcd.println(tempString);
      Serial.println(tempString);
      delay(delayTime);
      lcd.clear();
    }
  }
}

int getHtmlTagIndex(String tag, String page){
  bool tagFound;
  Serial.println("Inside getHtmlTagIndex\r\n");
  bool isCloseTag = checkForChar(tag, '/');
  int textIndex = 0;
  if(page.length() > tag.length()){
    for(int i = 0; i < page.length() - (tag.length() - 1); i++){
      tagFound = true;// Start true
      
      for(int j = 0; j < tag.length(); j++){
          if(page.charAt(i+j) != tag.charAt(j)){
              // if the character is ever a mis-match set false
              tagFound = false;
            }
        }
      if(tagFound){
          Serial.println("Tag Found\r\n");
          if(isCloseTag){
              textIndex = i;
            }
          else{
            textIndex = i + tag.length();
          }
          break;
        }
      }
    }
    else{
      textIndex = -1;
      }
    Serial.println(String(textIndex));
    return textIndex;
  }

bool checkForTag(String tag, String page){
  bool tagFound;
  Serial.println("Inside checlForTag\r\n");
  Serial.println(String(page.length()));
  Serial.println("\n");
  Serial.println(String(tag.length()));
  if(page.length() > tag.length()){
    for(int i = 0; i < page.length() - (tag.length() - 1); i++){
      tagFound = true;// Start true      
      for(int j = 0; j < tag.length(); j++){
          if(page.charAt(i+j) != tag.charAt(j)){
              // if the character is ever a mis-match set false
              tagFound = false;
            }
        }
      if(tagFound){
          Serial.println("Tag Found\r\n");
          break;
        }
      }
    if(!tagFound){
      Serial.println("Tag NOT Found!\r\n");
      }
  }
  else{
      tagFound = false;
    }
  return tagFound;
}

bool checkForChar(String line, char c){
    bool containsChar = false;
    for(int i = 0; i< line.length(); i++){
        if(line.charAt(i) == c){
            containsChar = true;
            Serial.println("CharacterFound!");
            break;
          }
      }
    return containsChar;
  }
