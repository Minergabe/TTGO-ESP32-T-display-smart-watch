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
