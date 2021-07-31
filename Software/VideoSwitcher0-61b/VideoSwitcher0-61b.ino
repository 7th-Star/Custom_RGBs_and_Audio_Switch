#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#define CLK 2
#define DT 3

byte currentStateCLK;
byte lastStateCLK;
byte input = 0;
byte currentinput = 0;
byte dplayedinput = 1;
unsigned long lastdisplaychange = 0;
unsigned long currenttime = 0;
unsigned long encodewait = 0;
const byte x1[] = {13,16,20,3,33,34,32,25,30,35,42,35,5,7,3};
const char console_1[] PROGMEM = "Playstation";
const char console_2[] PROGMEM = "Playstation";
const char console_3[] PROGMEM = "Playstation";
const char console_4[] PROGMEM = "Playstation";
const char console_5[] PROGMEM = "Playstation";
const char console_6[] PROGMEM = "Playstation";
const char console_7[] PROGMEM = "Playstation";
const char console_8[] PROGMEM = "Playstation";
const char console_9[] PROGMEM = "Playstation";
const char console_10[] PROGMEM = "Playstation";
const char console_11[] PROGMEM = "Playstation";
const char console_12[] PROGMEM = "Playstation";
const char console_13[] PROGMEM = "Playstation";
const char console_14[] PROGMEM = "Playstation";
PGM_P const console[] PROGMEM = {console_1,console_2,console_3,console_4,console_5,console_6,console_7,console_8,console_9,console_10,console_11,console_12,console_13,
console_14};
// const char *console[] = {"Playstation","Gamecube","Laserdisc","Playstation 2","Saturn","Xbox","SNES","Genesis","MKIII","VCR","PC","Casio Loopy","PC Engine","NES","KanekoSupnva"};
const char *tvset[] = {"Aux RGB","Y/C","Video A","Aux RGB","Aux RGB","Aux RGB","Aux RGB","Aux RGB","Aux RGB","Video B","Y/C","Video B","Video B","Video B","Aux RGB"};
const char *switch2[] = {"","Switcher 2  Input 2","","","","","","","","Switcher 2  Input 1","Switcher 2  Input 3","Switcher 2  Input 4","Switcher 2  Input 5",
"Switcher 2  Input 6",""};

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
  char buffer[strlen_P(console[input])];
  strcpy_P(buffer, (PGM_P)pgm_read_word(&console[input]));
  // Serial.println(buffer);
  u8g2.drawStr(x1[input],38,buffer);  
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.drawStr(7,55,tvset[input]);
  u8g2.drawStr(7,15,switch2[input]);
  u8g2.sendBuffer();
  lastdisplaychange = currenttime + 1000;
  dplayedinput = input;
   
}

void Encoderchange(){

  currentStateCLK = digitalRead(CLK);
  
  currenttime = millis();
  
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1 && currenttime >= encodewait){

    if (digitalRead(DT) != currentStateCLK) 
    {if (input > 0) {input --;}
     else {input = 14;}}
    else 
    {if (input < 14) {input ++;}
     else {input = 0;}}
     encodewait = currenttime + 75;
  }
  
  lastStateCLK = currentStateCLK;
  
}

void setinput(){

 
 // Serial.print ("Input Set\n");
 u8g2.drawFrame(0,0,128,64);
 u8g2.sendBuffer();
 currentinput = input;
  
}
