//MAX7219 Module	ESP8266
//VCC	3.3V
//GND	GND
//DIN	D7  Data in
//CS	D8  Chip Select
//CLK	D5  Clock pin

// Including the required Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN  15
#define HOST "iot.whichway.eu"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "xxxx"            // WIFI SSID here                                   
#define WIFI_PASSWORD "xxxx"        // WIFI password here
// Create a new instance of the MD_Parola class with hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
int val = 1;
int val2 = 99;
String sendval, sendval2, postData;
struct animations
{
  textEffect_t   anim_in; // Animation type
  textEffect_t   anim_out;// Animation type
  const char *   textOut;   // Text to display
  uint16_t       speed;        // Animation speed (multiplier for library default)
  uint16_t       pause;        // pause (multiplier for library default)
  textPosition_t just;
};

animations animList[] =
{
  { PA_SCROLL_LEFT, PA_SCROLL_LEFT , "Temperatura otoczenia", 3, 0, PA_LEFT },
  { PA_PRINT, PA_SCROLL_LEFT, "15.0 C", 2, 3 , PA_CENTER },
  { PA_SCROLL_LEFT, PA_SCROLL_LEFT , "Temperatura wody", 3, 0, PA_LEFT },
  { PA_PRINT, PA_GROW_DOWN, "39.6 C", 2, 3 , PA_CENTER},
  { PA_SCROLL_LEFT, PA_SCROLL_LEFT ,"Zaparaszamy", 3, 0 ,PA_LEFT},
  { PA_SCROLL_LEFT, PA_SCROLL_LEFT , "Domaniowka Jacuzzi ", 3, 0, PA_LEFT }, 
  { PA_FADE, PA_SCROLL_LEFT, "Wskocz", 3, 1 ,PA_CENTER},
  { PA_FADE, PA_SCROLL_LEFT, "do ", 3, 1 ,PA_CENTER},
  { PA_FADE, PA_SCROLL_LEFT, "wody ", 3, 1 , PA_CENTER}
};

void setup() {
  Serial.begin(115200); 
 P.begin();
 
  for (uint8_t i=0; i<ARRAY_SIZE(animList); i++)
  {
    animList[i].speed *= P.getSpeed(); animList[i].pause *= 500;
  }

    WiFi.mode(WIFI_STA);           
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) 
    { Serial.print(".");
        delay(500); }

    Serial.println();
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP Address is : ");
    Serial.println(WiFi.localIP());    //print local IP address

}

void loop() {

                        HTTPClient http;    // http object of clas HTTPClient
                      WiFiClient wclient; // wclient object of clas HTTPClient    


                      // Convert integer variables to string
                      sendval = String(val);  
                      sendval2 = String(val2);   

                      
                      postData = "value1=" + sendval + "&value2=" + sendval2;

                   
                      http.begin(wclient, "http://xxxxx");   

                      http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header

                        
                      
                      int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
                      Serial.println("Values are, sendval = " + sendval + " and sendval2 = "+sendval2 );


                      // if connection eatablished then do this
                      if (httpCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpCode); 
                      String webpage = http.getString();    // Get html webpage output and store it in a string
                      Serial.println(webpage + "\n"); 
                      }

                      // if failed to connect then return and restart

                      else { 
                        Serial.println(httpCode); 
                        Serial.println("Failed to upload values. \n"); 
                        http.end(); 
                        return; }





                        


                      delay(3000); 
                      digitalWrite(LED_BUILTIN, LOW);
                      delay(3000);
                      digitalWrite(LED_BUILTIN, HIGH);

                      val+=1; val2+=10; // Incrementing value of variables

  
  static uint8_t i = 0;  // text effect index

  if (P.displayAnimate())// animates and returns true when an animation is completed
  {
    if (i == ARRAY_SIZE(animList))i = 0;  // reset loop index

    P.displayText(animList[i].textOut, animList[i].just, animList[i].speed,  
                  animList[i].pause, animList[i].anim_in, animList[i].anim_out);
    delay(1000);
    i++;   // then set up for next text effect
  }    



}