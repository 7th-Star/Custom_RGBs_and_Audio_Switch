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

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  
  u8g2.begin();

  Serial.begin(9600);
  
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  
  lastStateCLK = digitalRead(CLK);
  
  attachInterrupt(0, Encoderchange, CHANGE);
  attachInterrupt(1, Encoderchange, CHANGE);
  
  setinput();
}

void loop(void) {

  if (dplayedinput != input){
    displayinput();}
    
  currenttime = millis();
  
  if (input != currentinput && currenttime >= lastdisplaychange){
    setinput();}
    
}

void displayinput(){

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox5t_tf);
  if (input == 0) {
  playstation();}
  else if (input == 1){
  gamecube();}
  else if (input == 2){
  laserdisc();}
  else if (input == 3){
  playstation2();}
  else if (input == 4){
  saturn();}
  else if (input == 5){
  xbox();}
  else if (input == 6){
  snes();}
  else if (input == 7){
  genesis();}
  u8g2.drawFrame(0,0,128,64);
  u8g2.sendBuffer();
  dplayedinput = input;
  lastdisplaychange = millis();
  lastdisplaychange = lastdisplaychange + 700;
  Serial.print ("Input Displayed\n");
  Serial.println (lastdisplaychange);
  Serial.println (currenttime);
   
}

void playstation (){     
   
  u8g2.drawStr(15,38,"Playstation");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");  
  
}

void gamecube (){
  
  u8g2.drawStr(20,38,"Gamecube");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Y/C");
  u8g2.drawStr(7,15,"Switcher 2 Input 3");
  
 }

void laserdisc(){
  
  u8g2.drawStr(15,38,"Laser Disc");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Video A");  
  
}

void playstation2(){
  
  u8g2.drawStr(6,38,"Playstation 2");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
  
}

void saturn(){

  u8g2.drawStr(37,38,"Saturn");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
    
}

void xbox(){
  
  u8g2.drawStr(40,38,"Xbox");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
  
}

void snes(){

  u8g2.drawStr(35,38,"SNES");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
  
}

void genesis(){

  u8g2.drawStr(37,38,"Genesis");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");
  
}

void mastersystem(){

  u8g2.drawStr(6,38,"Master System");  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,"Aux RGB");

}

void Encoderchange(){

  currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    if (digitalRead(DT) != currentStateCLK) 
    {if (input > 0) {input --;}
     else {input = 6;}}
    else 
    {if (input < 6) {input ++;}
     else {input = 0;}}
  }
  
  lastStateCLK = currentStateCLK;

  
}

void setinput(){

 Serial.print ("Input Set\n");
 currentinput = input;
  
}
