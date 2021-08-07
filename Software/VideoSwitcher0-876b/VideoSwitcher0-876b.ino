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
unsigned long lastdisplaychange = 0;
unsigned long encodewait = 0;
unsigned long lastbutton = 0;

const byte demuxout[] PROGMEM = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000};

//const char *outfxlockset[] = {"aoutlock_pic","aoutunlock_pic","boutlock_pic","boutunlock_pic"};

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

void playstation (){     

  if (outputselect == 0 && inputset == 1 && fx == 0) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,16, playstation_pic);
    u8g2.drawXBMP(0,22, 128,16, aux1_pic);
    u8g2.drawXBMP(0,49, 128,16, aoutlock_pic);
  } while ( u8g2.nextPage() ); 
  
}

  if (outputselect == 1 && inputset == 1 && fx == 0) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,16, playstation_pic);
    u8g2.drawXBMP(0,22, 128,16, aux1_pic);
    u8g2.drawXBMP(0,49, 128,16, boutlock_pic);
  } while ( u8g2.nextPage() ); 
  
}

  if (outputselect == 1 && inputset == 0 && fx == 0) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,16, playstation_pic);
    u8g2.drawXBMP(0,22, 128,16, aux1_pic);
    u8g2.drawXBMP(0,49, 128,16, boutunlock_pic);
  } while ( u8g2.nextPage() ); 
  
}

  if (outputselect == 0 && inputset == 0 && fx == 0) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,16, playstation_pic);
    u8g2.drawXBMP(0,22, 128,16, aux1_pic);
    u8g2.drawXBMP(0,49, 128,16, aoutunlock_pic);
  } while ( u8g2.nextPage() );
} 
  
}

void gamecube (){
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,64, gamecube_pic);
  } while ( u8g2.nextPage() );
  
 }

void laserdisc(){

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,64, laserdisc_pic);
  } while ( u8g2.nextPage() ); 
  
}

void playstation2(){
 
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,64, playstation2_pic);
  } while ( u8g2.nextPage() );
  
}

void saturn(){

  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,64, saturn_pic);
  } while ( u8g2.nextPage() );

}

void xbox(){
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,64, xbox_pic);
  } while ( u8g2.nextPage() );
  
}

void snes(){

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,64, snes_pic);
  } while ( u8g2.nextPage() );
  
}

void genesis(){

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,64, genesis_pic);
  } while ( u8g2.nextPage() );
  
}

void mastersystem(){
 
  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawXBMP(0,0, 128,64, mastersystem_pic);
  } while ( u8g2.nextPage() );

}

void kanekosupernova(){
 
  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawXBMP(0,0, 128,64, kaneko_pic);
  } while ( u8g2.nextPage() );

}

void openslot(){
 
  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawXBMP(0,0, 128,64, open_pic);
  } while ( u8g2.nextPage() );

}

void loopy(){
 
  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawXBMP(0,0, 128,64, loopy_pic);
  } while ( u8g2.nextPage() );

}

void nes(){
 
  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawXBMP(0,0, 128,64, nes_pic);
  } while ( u8g2.nextPage() );

}

void pc(){
 
  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawXBMP(0,0, 128,64, pc_pic);
  } while ( u8g2.nextPage() );

}

void pcengine(){
 
  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawXBMP(0,0, 128,64, pcengine_pic);
  } while ( u8g2.nextPage() );

}

void vcr(){
 
  u8g2.firstPage();
  do {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawXBMP(0,0, 128,64, vcr_pic);
  } while ( u8g2.nextPage() );

}

void (*selectinput[])() = {playstation,snes,mastersystem,xbox,saturn,playstation2,genesis,kanekosupernova,openslot,loopy,nes,pc,pcengine,vcr,gamecube,laserdisc};

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
    u8g2.setFont(u8g2_font_ncenB14_tr);
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
  
  PORTB = PORTB & B11011111;
  (*selectinput[input])();
  lastdisplaychange = millis();
  dplayedinput = input;
  
   
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
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawXBMP(0,0, 128,64, boot_pic);
  } while ( u8g2.nextPage() );
 delay(3000);
      
}
