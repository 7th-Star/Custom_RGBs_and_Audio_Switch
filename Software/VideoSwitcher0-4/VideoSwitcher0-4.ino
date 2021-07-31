#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#define CLK 2
#define DT 3

int currentStateCLK;
int lastStateCLK;
int input = 0;
int currentinput = 0;
int dplayedinput = 1;
unsigned long lastdisplaychange = 0;
unsigned long currenttime = 0;
unsigned long encodewait = 0;

// const char *serdisplay[] = {"playstation","gamecube","laserdisc","playstation2","saturn","xbox","snes","genesis","mastersystem"}; 

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  
  u8g2.begin();

  // Serial.begin(9600);
  
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  
  lastStateCLK = digitalRead(CLK);
  
  attachInterrupt(0, Encoderchange, CHANGE);
  attachInterrupt(1, Encoderchange, CHANGE);

  displayinput();
  setinput();
}

void loop(void) {

  currenttime = millis();
  
  if (dplayedinput != input){
    displayinput();}  
 
  if (input != currentinput && currenttime > lastdisplaychange){
    setinput();}
  
}

void playstation (){     
   
  u8g2.drawStr(13,34,"Playstation");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");  
  
}

void gamecube (){
  
  u8g2.drawStr(16,38,"Gamecube");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Y/C");
  u8g2.drawStr(7,15,"Switcher 2 Input 3");
  
 }

void laserdisc(){
  
  u8g2.drawStr(13,34,"Laser Disc");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Video A");  
  
}

void playstation2(){
  
  u8g2.drawStr(3,34,"Playstation 2");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
  
}

void saturn(){

  u8g2.drawStr(33,34,"Saturn");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
    
}

void xbox(){
  
  u8g2.drawStr(34,34,"Xbox");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
  
}

void snes(){

  u8g2.drawStr(32,34,"SNES");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
  
}

void genesis(){

  u8g2.drawStr(25,34,"Genesis");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
  
}

void mastersystem(){

  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(6,34,"Master System");    
  u8g2.drawStr(7,55,"Aux RGB");

}

void (*selectinput[])() = {playstation,gamecube,laserdisc,playstation2,saturn,xbox,snes,genesis,mastersystem};


void displayinput(){

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox5t_tf);
  (*selectinput[input])();
  u8g2.sendBuffer();
  lastdisplaychange = currenttime + 1000;
  dplayedinput = input;
  //Serial.print ("Input Displayed\n");
  // Serial.println (lastdisplaychange);
  // Serial.println (currenttime);
  // Serial.println (input);
  // Serial.println (currentinput);
   
}

void Encoderchange(){

  currentStateCLK = digitalRead(CLK);
  
  currenttime = millis();
  
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1 && currenttime >= encodewait){

    if (digitalRead(DT) != currentStateCLK) 
    {if (input > 0) {input --;}
     else {input = 8;}}
    else 
    {if (input < 8) {input ++;}
     else {input = 0;}}
     encodewait = currenttime + 75;
  }
  
  lastStateCLK = currentStateCLK;
  // Serial.print ("interrupt\n");
  // Serial.println (encodewait);
  // Serial.println (currentinput);
  // Serial.println (input);

  
}

void setinput(){

 
 // Serial.print ("Input Set\n");
 u8g2.drawFrame(0,0,128,64);
 u8g2.sendBuffer();
 // Serial.println (serdisplay[input]);
 currentinput = input;
  
}
