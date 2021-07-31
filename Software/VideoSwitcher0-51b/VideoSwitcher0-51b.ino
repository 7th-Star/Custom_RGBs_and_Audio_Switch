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
const int x1[] = {13,16,20,3,33,34,32,25,35};
const char *console[] = {"Playstation","Gamecube","Laserdisc","Playstation 2","Saturn","Xbox","SNES","Genesis","MKIII"};
const char *tvset[] = {"Aux RGB","Y/C","Video A","Aux RGB","Aux RGB","Aux RGB","Aux RGB","Aux RGB","Aux RGB"};
const char *switch2[] = {"","Switcher 2 Input 2","","","","","","",""};

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

void displayinput(){

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox5t_tf);
  u8g2.drawStr(x1[input],34,console[input]);  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,tvset[input]);
  u8g2.drawStr(7,15,switch2[input]);
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
