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

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN  15

// Create a new instance of the MD_Parola class with hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

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
 P.begin();
 
  for (uint8_t i=0; i<ARRAY_SIZE(animList); i++)
  {
    animList[i].speed *= P.getSpeed(); animList[i].pause *= 500;
  }

}

void loop() {
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