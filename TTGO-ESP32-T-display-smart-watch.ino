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
  bRt2.loop();
  bLe2.loop();
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

void pressedRt(Button2& bRt) {  //right button functions
}

void releasedRt(Button2& bRt) {

}

void changedRt(Button2& bRt) {

}

void clickRt(Button2& bRt) {
  if(screen == 1){
    rtBeenPressed = true;
  }
  if(screen == 0){
    sleepTimeActive = millis();
  }
}

void longClickRt(Button2& bRt) {
  if(screen == 2){
    ledState = !ledState;
    ledRefresh = true;
  }
}

void doubleClickRt(Button2& bRt) { 
     if(screen != 2){
    screen ++;
   }   
}

void tripleClickRt(Button2& bRt) {

}

void tapRt(Button2& bRt) {
}


void pressedLe(Button2& bLe) { //left button functions
}

void releasedLe(Button2& bLe) {
}

void changedLe(Button2& bLe) {

}

void clickLe(Button2& bLe) {
  if(screen == 1){
    leBeenPressed = true;
  }
  if(screen == 0){
    sleepTimeActive = millis();
  }
}

void longClickLe(Button2& bLe) {
  if(screen == 2){
    laserState = !laserState;
    laserRefresh = true;
  }
}

void doubleClickLe(Button2& bLe) {
  if(screen != 0){
     screen --;
  }
}

void tripleClickLe(Button2& bLe) {

}

void tapLe(Button2& bLe) {
}

void clickRt2(Button2& bRt2) {
  if(screen == 0){
    sleepTimeActive = millis();
    
  }
}

void clickLe2(Button2& bLe2) {
  if(screen == 0){
    sleepTimeActive = millis();
  }
}

// Return the minimum of two values a and b
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

//====================================================================================
//   Opens the image file and prime the Jpeg decoder
//====================================================================================
void drawJpeg(const char *filename, int xpos, int ypos) {

  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");

  // Open the named file (the Jpeg decoder library will close it after rendering image)
  fs::File jpegFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library

  //ESP32 always seems to return 1 for jpegFile so this null trap does not work
  if ( !jpegFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  // Use one of the three following methods to initialise the decoder,
  // the filename can be a String or character array type:

  //boolean decoded = JpegDec.decodeFsFile(jpegFile); // Pass a SPIFFS file handle to the decoder,
  //boolean decoded = JpegDec.decodeSdFile(jpegFile); // or pass the SD file handle to the decoder,
  boolean decoded = JpegDec.decodeFsFile(filename);  // or pass the filename (leading / distinguishes SPIFFS files)

  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();

    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}

//====================================================================================
//   Decode and render the Jpeg image onto the TFT screen
//====================================================================================
void jpegRender(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t  *pImg;
  int16_t mcu_w = JpegDec.MCUWidth;
  int16_t mcu_h = JpegDec.MCUHeight;
  int32_t max_x = JpegDec.width;
  int32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  int32_t min_w = minimum(mcu_w, max_x % mcu_w);
  int32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  int32_t win_w = mcu_w;
  int32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while ( JpegDec.readSwappedBytes()) { // Swapped byte order read

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      for (int h = 1; h < win_h-1; h++)
      {
        memcpy(pImg + h * win_w, pImg + (h + 1) * mcu_w, win_w << 1);
      }
    }

    // draw image MCU block only if it will fit on the screen
    if ( mcu_x < tft.width() && mcu_y < tft.height())
    {
      // Now push the image block to the screen
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    }

    else if ( ( mcu_y + win_h) >= tft.height()) JpegDec.abort();

  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime; // Calculate the time it took

  // print the results to the serial port
  Serial.print  ("Total render time was    : "); Serial.print(drawTime); Serial.println(" ms");
  Serial.println("=====================================");

}

//====================================================================================
//   Print information decoded from the Jpeg image
//====================================================================================
void jpegInfo() {

  Serial.println("===============");
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print  ("Width      :"); Serial.println(JpegDec.width);
  Serial.print  ("Height     :"); Serial.println(JpegDec.height);
  Serial.print  ("Components :"); Serial.println(JpegDec.comps);
  Serial.print  ("MCU / row  :"); Serial.println(JpegDec.MCUSPerRow);
  Serial.print  ("MCU / col  :"); Serial.println(JpegDec.MCUSPerCol);
  Serial.print  ("Scan type  :"); Serial.println(JpegDec.scanType);
  Serial.print  ("MCU width  :"); Serial.println(JpegDec.MCUWidth);
  Serial.print  ("MCU height :"); Serial.println(JpegDec.MCUHeight);
  Serial.println("===============");
  Serial.println("");
}

//====================================================================================
//   Open a Jpeg file and send it to the Serial port in a C array compatible format
//====================================================================================
void createArray(const char *filename) {

  // Open the named file
  fs::File jpgFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //  File jpgFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library

  if ( !jpgFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  uint8_t data;
  byte line_len = 0;
  Serial.println("");
  Serial.println("// Generated by a JPEGDecoder library example sketch:");
  Serial.println("// https://github.com/Bodmer/JPEGDecoder");
  Serial.println("");
  Serial.println("#if defined(__AVR__)");
  Serial.println("  #include <avr/pgmspace.h>");
  Serial.println("#endif");
  Serial.println("");
  Serial.print  ("const uint8_t ");
  while (*filename != '.') Serial.print(*filename++);
  Serial.println("[] PROGMEM = {"); // PROGMEM added for AVR processors, it is ignored by Due

  while ( jpgFile.available()) {

    data = jpgFile.read();
    Serial.print("0x"); if (abs(data) < 16) Serial.print("0");
    Serial.print(data, HEX); Serial.print(",");// Add value and comma
    line_len++;
    if ( line_len >= 32) {
      line_len = 0;
      Serial.println();
    }

  }

  Serial.println("};\r\n");
  jpgFile.close();
}
//====================================================================================

//====================================================================================
//                 Print a SPIFFS directory list (root directory)
//====================================================================================
#ifdef ESP8266
void listFiles(void) {
  Serial.println();
  Serial.println("SPIFFS files found:");

  fs::Dir dir = SPIFFS.openDir("/"); // Root directory
  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  while (dir.next()) {
    String fileName = dir.fileName();
    Serial.print(fileName);
    int spaces = 21 - fileName.length(); // Tabulate nicely
    while (spaces--) Serial.print(" ");

    fs::File f = dir.openFile("r");
    String fileSize = (String) f.size();
    spaces = 10 - fileSize.length(); // Tabulate nicely
    while (spaces--) Serial.print(" ");
    Serial.println(fileSize + " bytes");
  }

  Serial.println(line);
  Serial.println();
  delay(1000);
}
#endif

//====================================================================================

#ifdef ESP32

void listFiles(void) {
  listDir(SPIFFS, "/", 0);
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {

  Serial.println();
  Serial.println("SPIFFS files found:");

  Serial.printf("Listing directory: %s\n", "/");
  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  fs::File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      int spaces = 20 - fileName.length(); // Tabulate nicely
      while (spaces--) Serial.print(" ");
      String fileSize = (String) file.size();
      spaces = 10 - fileSize.length(); // Tabulate nicely
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }

  Serial.println(line);
  Serial.println();

}
#endif
void tellTime() {
  bRt.loop();
  bLe.loop();
  
  oldSecond = newSecond; // old time is time after loop
  oldMinute = newMinute;
  oldHour = newHour;
  oldTemp = newTemp;
  DateTime now = rtc.now();
  newSecond = now.second();// newTime is assigned to current second
  newMinute = now.minute();
  newHour = now.hour();
  newTemp = rtc.getTemperature();

  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.print(daysOfTheWeek[now.dayOfTheWeek()]);
  tft.setTextColor(TFT_WHITE); 
  tft.setRotation(0);
  tft.setCursor(10, 30);
  tft.setTextSize(4);

  if (now.hour() < 10) {
    tft.print("0");
  }

  tft.print(now.hour());
  tft.print(":");

  if (now.minute() < 10) {
    tft.print("0");
  }

  tft.print(now.minute());
  
  tft.setCursor(10, 80);
  tft.setTextSize(3);
  
  if (now.second() < 10) {
    tft.print("0");
  }

  tft.print(now.second());

  tft.setTextSize(2);
  tft.setCursor(72, 85);
  tft.print(rtc.getTemperature());
  

  if (oldSecond != newSecond) {
    //tft.fillRect(10, 70, 35, 22, TFT_BLACK);
    drawJpeg("/secondsSquare.jpg", 10 , 80);
    
      

    
  }

  if (oldMinute != newMinute) {
    //tft.fillRect(78, 20, 50, 30, TFT_BLACK);
    drawJpeg("/minutesSquare.jpg", 78 , 30);
  }

  if (oldHour != newHour) {
    drawJpeg("/wallPaper.jpg", 0 , 0);
  }

  if(oldTemp != newTemp){
    //tft.fillRect(72, 85, 60, 16, TFT_GREEN);
    drawJpeg("/tempSquare.jpg", 72 , 85);
  }
  sleeps();

  


}
void battery(){
  int oldCharge = charge;
  float batteryVoltage = ((float)analogRead(34) / 4095.0) * 2.0 * 3.3 * (1100 / 1000.0);
  
  if(batteryVoltage > 3.91 && batteryVoltage < 4.35 || antiFlicker == 1){
    charge = 100;
    antiFlicker = 1;
  tft.drawLine(batOx, batOy, batOx, batOy+17, TFT_WHITE);
  tft.drawLine(batOx+1, batOy, batOx+1, batOy+17, TFT_WHITE);
  
  tft.drawLine(batOx, batOy, batOx+31, batOy, TFT_WHITE);
  tft.drawLine(batOx, batOy+1, batOx+31, batOy+1, TFT_WHITE);
  
  tft.drawLine(batOx+30, batOy, batOx+30, batOy+5, TFT_WHITE);
  tft.drawLine(batOx+31, batOy, batOx+31, batOy+5, TFT_WHITE);

  tft.drawLine(batOx, batOy+16, batOx+31, batOy+16, TFT_WHITE);
  tft.drawLine(batOx, batOy+17, batOx+31, batOy+17, TFT_WHITE);
  
  tft.drawLine(batOx+30, batOy+17, batOx+30, batOy+12, TFT_WHITE);
  tft.drawLine(batOx+31, batOy+17, batOx+31, batOy+12, TFT_WHITE);

  tft.drawLine(batOx+32, batOy+4, batOx+32, batOy+5, TFT_WHITE);
  tft.drawLine(batOx+32, batOy+12, batOx+32, batOy+13, TFT_WHITE);
  
  tft.drawLine(batOx+33, batOy+4, batOx+33, batOy+13, TFT_WHITE);
  tft.drawLine(batOx+34, batOy+4, batOx+34, batOy+13, TFT_WHITE);

  tft.fillRect(batOx+4, batOy+3, 6, 12, TFT_WHITE);
  
  tft.fillRect(batOx+12, batOy+3, 6, 12, TFT_WHITE); // 66%
  

  tft.fillRect(batOx+20, batOy+3, 6, 12, TFT_WHITE); // 100%
  }
  else if(batteryVoltage < 3.91 && batteryVoltage > 3.78 || antiFlicker == 2){
    charge = 66;
    antiFlicker = 2;
  tft.drawLine(batOx, batOy, batOx, batOy+17, TFT_WHITE);
  tft.drawLine(batOx+1, batOy, batOx+1, batOy+17, TFT_WHITE);
  
  tft.drawLine(batOx, batOy, batOx+31, batOy, TFT_WHITE);
  tft.drawLine(batOx, batOy+1, batOx+31, batOy+1, TFT_WHITE);
  
  tft.drawLine(batOx+30, batOy, batOx+30, batOy+5, TFT_WHITE);
  tft.drawLine(batOx+31, batOy, batOx+31, batOy+5, TFT_WHITE);

  tft.drawLine(batOx, batOy+16, batOx+31, batOy+16, TFT_WHITE);
  tft.drawLine(batOx, batOy+17, batOx+31, batOy+17, TFT_WHITE);
  
  tft.drawLine(batOx+30, batOy+17, batOx+30, batOy+12, TFT_WHITE);
  tft.drawLine(batOx+31, batOy+17, batOx+31, batOy+12, TFT_WHITE);

  tft.drawLine(batOx+32, batOy+4, batOx+32, batOy+5, TFT_WHITE);
  tft.drawLine(batOx+32, batOy+12, batOx+32, batOy+13, TFT_WHITE);
  
  tft.drawLine(batOx+33, batOy+4, batOx+33, batOy+13, TFT_WHITE);
  tft.drawLine(batOx+34, batOy+4, batOx+34, batOy+13, TFT_WHITE);

  tft.fillRect(batOx+4, batOy+3, 6, 12, TFT_WHITE);
  
 
  tft.fillRect(batOx+12, batOy+3, 6, 12, TFT_WHITE); // 66%
  }
  
  else if(batteryVoltage < 3.78 && batteryVoltage || antiFlicker == 3 ){
    charge = 33;
    antiFlicker = 3;
  tft.drawLine(batOx, batOy, batOx, batOy+17, TFT_WHITE);
  tft.drawLine(batOx+1, batOy, batOx+1, batOy+17, TFT_WHITE);
  
  tft.drawLine(batOx, batOy, batOx+31, batOy, TFT_WHITE);
  tft.drawLine(batOx, batOy+1, batOx+31, batOy+1, TFT_WHITE);
  
  tft.drawLine(batOx+30, batOy, batOx+30, batOy+5, TFT_WHITE);
  tft.drawLine(batOx+31, batOy, batOx+31, batOy+5, TFT_WHITE);

  tft.drawLine(batOx, batOy+16, batOx+31, batOy+16, TFT_WHITE);
  tft.drawLine(batOx, batOy+17, batOx+31, batOy+17, TFT_WHITE);
  
  tft.drawLine(batOx+30, batOy+17, batOx+30, batOy+12, TFT_WHITE);
  tft.drawLine(batOx+31, batOy+17, batOx+31, batOy+12, TFT_WHITE);

  tft.drawLine(batOx+32, batOy+4, batOx+32, batOy+5, TFT_WHITE);
  tft.drawLine(batOx+32, batOy+12, batOx+32, batOy+13, TFT_WHITE);
  
  tft.drawLine(batOx+33, batOy+4, batOx+33, batOy+13, TFT_WHITE);
  tft.drawLine(batOx+34, batOy+4, batOx+34, batOy+13, TFT_WHITE);

  tft.fillRect(batOx+4, batOy+3, 6, 12, TFT_WHITE);
  }
  Serial.println(charge);
  if(batteryVoltage > 4.35){
      tft.setTextSize(1);
      tft.setCursor(0, 230);
      tft.print("Connected");
      charge = 101;
      antiFlicker = 0;
  }
  

  
  if(oldCharge != charge){
   drawJpeg("/wallPaper.jpg", 0 , 0); 
  }

}
void peripherals(){
  
  bRt.loop();
  bLe.loop();
  
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.print("Peripherals");
  tft.setCursor(0, 30);
  tft.print("LED:");
  tft.setCursor(0, 60);
  tft.print("Laser:");

  if(ledState == HIGH){
  tft.setCursor(80, 30);
  tft.print("ON");
  }

  else if(ledState == LOW){
  tft.setCursor(80, 30);
  tft.print("OFF");   
  }

  if(laserState == HIGH){
  tft.setCursor(80, 60);
  tft.print("ON");
  }

  else if(laserState == LOW){
  tft.setCursor(80, 60);
  tft.print("OFF");   
  }

  if(ledRefresh){
    //tft.fillRect(80, 30, 35, 18, TFT_GREEN);
    drawJpeg("/ledSquare.jpg", 80 , 30);
    ledRefresh = false;
  }

  if(laserRefresh){
    //tft.fillRect(80, 60, 35, 18, TFT_GREEN);
    drawJpeg("/laserSquare.jpg", 80 , 60);
    laserRefresh = false;
  }



}
void sleeps(){
  

  bRt.loop();
  bLe.loop(); 
   
   
    sleepTimeLazy = (millis()- sleepTimeActive);
    
    if(sleepTimeLazy > 30000 ){
      digitalWrite(bl, LOW);
    }
    else{
      digitalWrite(bl, HIGH);
    }
  
}
void stopwatch(){
  
  bRt.loop();
  bLe.loop();

  tUntilPressed = millis();
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.print("StopWatch");
  tft.setCursor(0, 30);
  tft.print("Hours:");
  tft.setCursor(0, 50);
  tft.print(clockTimeHour);
  tft.setCursor(0, 80);
  tft.print("Minutes:");
  tft.setCursor(0, 100);
  tft.print(clockTimeMin);
  tft.setCursor(0, 130);
  tft.print("Seconds:");
  tft.setCursor(0, 150);
  tft.print(clockTime);
  while(rtBeenPressed){
    leBeenPressed = false;
    bRt.loop();
    bLe.loop();
    
    clockTimeHourOld = clockTimeHour;
    clockTimeMinOld = clockTimeMin;
    clockTimeOld = clockTime;
    
    tAtPressed = millis();
    
    clockTime = (tAtPressed-tUntilPressed)/1000.00;
    clockTimeInt = (tAtPressed-tUntilPressed)/1000;
    
    if(clockTime >= 60){
      clockTimeMin ++;
      tUntilPressed = millis();
    }
    if(clockTimeMin >= 60){
      clockTimeHour ++;
      clockTimeMin = 0;
    }
    
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.print("StopWatch");
  tft.setCursor(0, 30);
  tft.print("Hours:");
  tft.setCursor(0, 50);
  tft.print(clockTimeHour);
  tft.setCursor(0, 80);
  tft.print("Minutes:");
  tft.setCursor(0, 100);
  tft.print(clockTimeMin);
  tft.setCursor(0, 130);
  tft.print("Seconds:");
  tft.setCursor(0, 150);
  tft.print(clockTime);

    if(clockTimeOld != clockTimeInt ){
      //tft.fillRect(0, 150, 62, 18, TFT_GREEN);
      drawJpeg("/swSecondSquare.jpg", 0 , 150);
    }

    if(clockTimeMinOld != clockTimeMin ){
      //tft.fillRect(0, 100, 30, 18, TFT_GREEN);
      drawJpeg("/swMinuteSquare.jpg", 0 , 100);
    }

    if(clockTimeHourOld != clockTimeHour ){
      //tft.fillRect(0, 50, 50, 18, TFT_GREEN);
      drawJpeg("/swHourSquare.jpg", 0 , 50);
    }
    
    if(leBeenPressed){
      rtBeenPressed = false;
      leBeenPressed = false;
      drawJpeg("/swSecondSquare.jpg", 0 , 150);
      drawJpeg("/swMinutesSquare.jpg", 0 , 100);
      drawJpeg("/swHourSquare.jpg", 0 , 50);
      
    }
  }
}
 
