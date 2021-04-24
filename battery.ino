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
