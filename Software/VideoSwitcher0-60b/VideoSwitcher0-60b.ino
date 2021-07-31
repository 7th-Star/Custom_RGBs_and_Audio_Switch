#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define CLK 2
#define DT 3
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

byte currentStateCLK;
byte lastStateCLK;
byte input = 0;
byte currentinput = 0;
byte dplayedinput = 1;
unsigned long lastdisplaychange = 0;
unsigned long currenttime = 0;
unsigned long encodewait = 0;
const byte x1[] = {13,16,20,3,33,34,32,25,35,35,42,17};
const char *console[] = {"Playstation","Gamecube","Laserdisc","Playstation 2","Saturn","Xbox","SNES","Genesis","MKIII","VCR","PC","Casio Loopy"};
const char *tvset[] = {"Aux RGB","Y/C","Video A","Aux RGB","Aux RGB","Aux RGB","Aux RGB","Aux RGB","Aux RGB","Video B","Y/C","Video B"};
const char *switch2[] = {"","Switcher 2 Input 2","","","","","","","","Switcher 2 Input 1","Switcher 2 Input 3","Switcher 2 Input 4"};


void setup(void) {

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  
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

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(x1[input],34);
  display.println(console[input]);
  display.setCursor(7,55);
  display.setTextSize(1);
  display.println(tvset[input]);
  display.setCursor(7,15);
  display.println(switch2[input]);
  display.display();
  lastdisplaychange = currenttime + 1000;
  dplayedinput = input;
   
}

void Encoderchange(){

  currentStateCLK = digitalRead(CLK);
  
  currenttime = millis();
  
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1 && currenttime >= encodewait){

    if (digitalRead(DT) != currentStateCLK) 
    {if (input > 0) {input --;}
     else {input = 9;}}
    else 
    {if (input < 9) {input ++;}
     else {input = 0;}}
     encodewait = currenttime + 75;
  }
  
  lastStateCLK = currentStateCLK;
  
}

void setinput(){

 
 // Serial.print ("Input Set\n");
 currentinput = input;
  
}
