#include <Arduino.h>
//#include <U8g2lib.h>
#include <EEPROM.h>
#include <Wire.h>
#define CLK 2
#define DT 3

byte currentStateCLK;
byte lastStateCLK;
byte input = EEPROM.read(0);
byte currentinput = 0;
byte dplayedinput = 1;
unsigned long lastdisplaychange = 0;
unsigned long currenttime = 0;
unsigned long encodewait = 0;

const byte demuxout[] PROGMEM = {B00100000,B00000000,B00100000,B00000000,B00100000,B00000000,B00100000,B00000000,B00100000,B00000000,B00100000,B00100000,B00000000,B00100000,
B00000000,};

const byte x1[] PROGMEM = {13,16,20,3,33,34,32,25,8,35,42,5,15,38,14};

const char console_1[] PROGMEM = "Playstation";
const char console_2[] PROGMEM = "Gamecube";
const char console_3[] PROGMEM = "Laserdisc";
const char console_4[] PROGMEM = "Playstation 2";
const char console_5[] PROGMEM = "Saturn";
const char console_6[] PROGMEM = "Xbox";
const char console_7[] PROGMEM = "SNES";
const char console_8[] PROGMEM = "Genesis";
const char console_9[] PROGMEM = "Master System";
const char console_10[] PROGMEM = "VCR";
const char console_11[] PROGMEM = "PC";
const char console_12[] PROGMEM = "Casio Loopy";
const char console_13[] PROGMEM = "PC Engine";
const char console_14[] PROGMEM = "NES";
const char console_15[] PROGMEM = "Super Nova";

PGM_P const console[] PROGMEM = {console_1,console_2,console_3,console_4,console_5,console_6,console_7,console_8,console_9,console_10,console_11,console_12,console_13,
console_14,console_15};

const char tvset_1[] PROGMEM = "AUX RGB 0";
const char tvset_2[] PROGMEM = "Y/C 1";
const char tvset_3[] PROGMEM = "Video A 2";
const char tvset_4[] PROGMEM = "AUX RGB 3";
const char tvset_5[] PROGMEM = "AUX RGB 4";
const char tvset_6[] PROGMEM = "AUX RGB 5";
const char tvset_7[] PROGMEM = "AUX RGB 6";
const char tvset_8[] PROGMEM = "AUX RGB 7";
const char tvset_9[] PROGMEM = "AUX RGB 8";
const char tvset_10[] PROGMEM = "Video B 9";
const char tvset_11[] PROGMEM = "Video B 10";
const char tvset_12[] PROGMEM = "Video B 11";
const char tvset_13[] PROGMEM = "Video B 12";
const char tvset_14[] PROGMEM = "Video B 13";
const char tvset_15[] PROGMEM = "AUX RGB   System";

PGM_P const tvset[] PROGMEM = {tvset_1,tvset_2,tvset_3,tvset_4,tvset_5,tvset_6,tvset_7,tvset_8,tvset_9,tvset_10,tvset_11,tvset_12,tvset_13,tvset_14,tvset_15};

const char switchb_1[] PROGMEM = "";
const char switchb_2[] PROGMEM = "Switcher 2  Input 1";
const char switchb_3[] PROGMEM = "";
const char switchb_4[] PROGMEM = "";
const char switchb_5[] PROGMEM = "";
const char switchb_6[] PROGMEM = "";
const char switchb_7[] PROGMEM = "";
const char switchb_8[] PROGMEM = "";
const char switchb_9[] PROGMEM = "";
const char switchb_10[] PROGMEM = "Switcher 2  Input 2";
const char switchb_11[] PROGMEM = "Switcher 2  Input 3";
const char switchb_12[] PROGMEM = "Switcher 2  Input 4";
const char switchb_13[] PROGMEM = "Switcher 2  Input 5";
const char switchb_14[] PROGMEM = "Switcher 2  Input 6";
const char switchb_15[] PROGMEM = "Kaneko";

PGM_P const switchb[] PROGMEM = {switchb_1,switchb_2,switchb_3,switchb_4,switchb_5,switchb_6,switchb_7,switchb_8,switchb_9,switchb_10,switchb_11,switchb_12,switchb_13,
switchb_14,switchb_15};

//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  
  //u8g2.begin();

  if (input < 0 || input > 14){
  input = 0;  
  }

  //Serial.begin(9600);

  DDRB = B00111111;
  DDRD = B00000010;

  //pinMode(CLK,INPUT);
  //pinMode(DT,INPUT);
    
  lastStateCLK = digitalRead(CLK);
  
  attachInterrupt(0, Encoderchange, CHANGE);
  attachInterrupt(1, Encoderchange, CHANGE);

  //bootscreen();
  displayinput();
  setinput();
}

void loop(void) {

  currenttime = millis();
  
  if (dplayedinput != input){
    displayinput();}  
 
  if (input != currentinput && currenttime > lastdisplaychange){
    setinput();}

  if (currenttime + 1001 - lastdisplaychange > 10000){
    //u8g2.clearBuffer();
    //u8g2.sendBuffer();
  }

  //input++; //Repeat all inputs
  //delay(1000);
  
}

void displayinput(){


  //u8g2.clearBuffer();
  
  if (input == 8) {
  //u8g2.setFont(u8g2_font_crox4t_tf);
  } 
  else if (input == 14) {
  //u8g2.setFont(u8g2_font_helvB12_tf);
  }
  else{ 
  //u8g2.setFont(u8g2_font_crox5t_tf);   
  }

  byte buffer6 pgm_read_byte(&x1[input]);
  char buffer1[strlen_P(console[input])];
  strcpy_P(buffer1, (PGM_P)pgm_read_word(&console[input]));
  //u8g2.drawStr(buffer6,38,buffer1);
 
  //u8g2.setFont(u8g2_font_t0_12_mr); 
  char buffer2[strlen_P(tvset[input])];
  strcpy_P(buffer2, (PGM_P)pgm_read_word(&tvset[input]));
  //u8g2.drawStr(7,55,buffer2);  

  //u8g2.setFont(u8g2_font_t0_12_mr);
  char buffer3[strlen_P(switchb[input])];
  strcpy_P(buffer3, (PGM_P)pgm_read_word(&switchb[input]));
  //u8g2.drawStr(7,15,buffer3);

  //Serial.println(input);
  //u8g2.sendBuffer();
  lastdisplaychange = currenttime + 1000;
  dplayedinput = input;
   
}

void Encoderchange(){

  currenttime = millis();

  currentStateCLK = digitalRead(CLK);
  
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1 && currenttime >= encodewait){
    
    if (digitalRead(DT) != currentStateCLK) 
    {if (input > 0) {input --;}
     else {input = 14;}}
    else 
    {if (input < 14) {input ++;}
     else {input = 0;}}
     encodewait = currenttime + 350;
  }
     lastStateCLK = currentStateCLK;
}

void setinput(){

 byte buffer5 pgm_read_byte(&demuxout[input]);
 PORTB = buffer5;
 EEPROM.write(0, input);   
 //u8g2.drawFrame(0,0,128,64);
 //u8g2.sendBuffer();
 currentinput = input;
  
}

//void bootscreen(){
 
 //u8g2.setFont(u8g2_font_t0_12_mr); 
 //u8g2.drawStr(15,20,"Custom Video");
 //u8g2.drawStr(25,35,"Switch");
 //u8g2.drawStr(50,55,"V.75b");
 //u8g2.sendBuffer();
 //delay(3000);
      
//}
