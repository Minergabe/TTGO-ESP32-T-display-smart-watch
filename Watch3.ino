#define FS_NO_GLOBALS
#include <FS.h>
#include <Arduino.h>


#ifdef ESP32
  #include "SPIFFS.h" // ESP32 only
#endif

#include <JPEGDecoder.h>
#include "RTClib.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "Button2.h";


#define BUTTON_PINRT  35
#define BUTTON_PINLE  0
#define BUTTON_PINRT2  27
#define BUTTON_PINLE2  13
#define led 26
#define laser 25
#define bl 4 

Button2 bRt = Button2(BUTTON_PINRT);
Button2 bLe = Button2(BUTTON_PINLE);
Button2 bRt2 = Button2(BUTTON_PINRT2);
Button2 bLe2 = Button2(BUTTON_PINLE2);

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
RTC_DS3231 rtc;



char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
float oldSecond;
float newSecond;

int oldMinute;
int newMinute;

int oldHour;
int newHour;

float oldTemp;
float newTemp;

bool newDay;

int screen;
int oldScreen;

int ledState;
int laserState;
bool ledRefresh;
bool laserRefresh;

unsigned long tUntilPressed;
unsigned long tAtPressed;

unsigned long sleepTimeActive;
unsigned long sleepTimeLazy;

float clockTime = 0.000;
int clockTimeInt;
int clockTimeMin;
int clockTimeHour;
int clockTimeOld;
int clockTimeMinOld;
int clockTimeHourOld;

bool rtBeenPressed;
bool leBeenPressed;

int charge;

int batOx = 0; 
int batOy = 220;
int antiFlicker = 1;

void setup() {

Serial.begin(250000);

pinMode(led, OUTPUT);
pinMode(laser, OUTPUT);
pinMode(bl, OUTPUT);



#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.flush();
    abort();
  }



  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  

  tft.begin();
  tft.init();   

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nInitialisation done.");
  listFiles(); // Lists the files so you can see what is in the SPIFFS

  tft.fillScreen(TFT_BLACK);
  drawJpeg("/wallPaper.jpg", 0 , 0);

  bRt.setReleasedHandler(releasedRt); // initialise right button
  bRt.setClickHandler(clickRt);
  bRt.setLongClickHandler(longClickRt);
  bRt.setDoubleClickHandler(doubleClickRt);
  bRt.setTripleClickHandler(tripleClickRt);

  bLe.setReleasedHandler(releasedLe);  // initialise left button
  bLe.setClickHandler(clickLe);
  bLe.setLongClickHandler(longClickLe);
  bLe.setDoubleClickHandler(doubleClickLe);
  bLe.setTripleClickHandler(tripleClickLe);

  bRt2.setClickHandler(clickRt2);
  
  bLe2.setClickHandler(clickLe2);

  
}

void loop() {
  oldScreen = screen;
  bRt.loop();
  bLe.loop();
switch (screen) {
  case 0:
  tellTime();
    break;
  case 1:
    stopwatch();
    break;
  case 2:
    peripherals();
    break;


}
  if(oldScreen != screen){
    drawJpeg("/wallPaper.jpg", 0 , 0);
    sleepTimeActive = millis();
  }
  digitalWrite(led, ledState);
  digitalWrite(laser, laserState);
  battery();
  

}
 
