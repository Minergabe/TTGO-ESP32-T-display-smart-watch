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
