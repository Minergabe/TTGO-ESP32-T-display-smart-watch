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
