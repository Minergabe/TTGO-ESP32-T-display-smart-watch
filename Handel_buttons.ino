
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
