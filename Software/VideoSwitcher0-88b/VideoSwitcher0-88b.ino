#include <Arduino.h>
#include <U8g2lib.h>
#include <EEPROM.h>
#include <Wire.h>
#include "inputpics.h"
#define CLK 2
#define DT 3
#define SW 4

byte currentStateCLK;
byte lastStateCLK;
byte input = EEPROM.read(0);
byte inputset = 0;
byte dplayedinput = 1;
byte outputselect = EEPROM.read(1);
byte button = 0;
byte fx = EEPROM.read(2);
byte fxsetnum = 0;
unsigned long lastdisplaychange = 0;
unsigned long encodewait = 0;
unsigned long lastbutton = 0;


const byte demuxout[] PROGMEM = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000};



U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {

  //Serial.begin(9600);
  
  u8g2.begin();

  if (input < 0 || input > 16){
  input = 0;  
  }
  
  if (outputselect != 0 && outputselect != 1){
  outputselect = 0;
  }

  if (fx != 0 || fx != 1){
  fx = 0;
  }
  
  DDRB = B00111111;
  DDRD = B00000010;
    
  lastStateCLK = digitalRead(CLK);
  
  attachInterrupt(0, Encoderchange, CHANGE);
  attachInterrupt(1, Encoderchange, CHANGE);

  bootscreen();
  displayinput();
  setinput();
}

const uint8_t *outfxlockset[] = {aoutlock_pic,aoutunlock_pic,boutlock_pic,boutunlock_pic};
const uint8_t *intputdis[] = {playstation_pic,snes_pic,mastersystem_pic,xbox_pic,saturn_pic,playstation2_pic,genesis_pic,
kaneko_pic,open_pic,loopy_pic,nes_pic,pc_pic,pcengine_pic,vcr_pic,gamecube_pic,laserdisc_pic};
const uint8_t *intputdis2[] = {playstation_pic,snes_pic,mastersystem_pic,xbox_pic,saturn_pic,playstation2_pic,genesis_pic,
kaneko_pic,open_pic,loopy_pic,nes_pic,pc_pic,pcengine_pic,vcr_pic,gamecube_pic,laserdisc_pic};
const uint8_t *switchsel[] = {aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic,aux1_pic};

// void (*selectinput[])() = {playstation,snes,mastersystem,xbox,saturn,playstation2,genesis,kanekosupernova,openslot,loopy,nes,pc,pcengine,vcr,gamecube,laserdisc};

void loop(void) {
  
  if (dplayedinput != input){
   displayinput();
   inputset = 0;}  
 
  if (inputset != 1 && millis() - lastdisplaychange > 1000){ 
   setinput();}

  button = digitalRead(SW);

  if (button == LOW){
        if (millis() - lastbutton > 50) {
     outputset();}
    lastbutton = millis();}
  
  if (millis() - lastdisplaychange > 10000){
    u8g2.firstPage();
  do {
  } while ( u8g2.nextPage() ); 
  }
}

void outputset(){
  
  outputselect = !outputselect;
  EEPROM.write(1, outputselect); 
  displayinput();
  inputset = 1;
  
}

void displayinput(){

  fxoutunlockset();
  PORTB = PORTB & B11011111;
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(0,0, 128,16, intputdis[input]);
    u8g2.drawXBMP(0,17, 128,16, intputdis2[input]);
    u8g2.drawXBMP(0,33, 128,16, switchsel[input]);
    u8g2.drawXBMP(0,49, 128,16, outfxlockset[fxsetnum]);
  } while ( u8g2.nextPage() );   
  lastdisplaychange = millis();
  dplayedinput = input;
     
}

void fxoutunlockset(){

  if (outputselect == 0 && inputset == 1 && fx == 0) {
     fxsetnum = 0;
}

  if (outputselect == 1 && inputset == 1 && fx == 0) {
     fxsetnum = 2;
}

  if (outputselect == 1 && inputset == 0 && fx == 0) {
     fxsetnum = 3;
}

  if (outputselect == 0 && inputset == 0 && fx == 0) {
     fxsetnum = 1;
}  
}

void Encoderchange(){

  currentStateCLK = digitalRead(CLK);
  
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1 && millis() > encodewait){
    
    if (digitalRead(DT) != currentStateCLK) 
    {if (input > 0) {input --;}
     else {input = 15;}}
    else 
    {if (input < 15) {input ++;}
     else {input = 0;}}
     encodewait = millis()+ 50;
  }
     lastStateCLK = currentStateCLK;
}

void setinput(){

 byte buffer5 pgm_read_byte(&demuxout[input]);
 PORTB = buffer5;
 PORTB = PORTB | B00100000;
 EEPROM.write(0, input); 
 inputset = 1;  
 displayinput();

}

void bootscreen(){
 
   u8g2.firstPage();
  do {
    u8g2.drawXBMP(0,0, 128,64, boot_pic);
  } while ( u8g2.nextPage() );
 delay(3000);
      
}
