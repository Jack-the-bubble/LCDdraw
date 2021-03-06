/*

  HelloWorld.pde
  
  "Hello World!" example code.
  
  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, http://code.google.com/p/u8glib/
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/


#include "U8glib.h"

U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

#define WYS 52
#define SZER 8

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

int adc_key_in;
int key=-1;
int oldkey=-1;

char bufx[9];//sprawdx, czy nie będzie się zaqpychać przy sprintf!
char bufy[9];
int x=0;
int y=0;

int rad=0;
short Tabyx[SZER][WYS];
//int Tabx[126];
//int Taby[52];

// Convert ADC value to key number
//         4
//         |
//   0 --  1 -- 3
//         |
//         2
int get_key(unsigned int input)
{   
    if (input < 100) return 0;
    else  if (input < 300) return 1;
    else  if (input < 500) return 2;
    else  if (input < 700) return 3;
    else  if (input < 900) return 4;    
    else  return -1;
}

void uiStep(void) {
  
  adc_key_in = analogRead(0);    // read the value from the sensor  
  key = get_key(adc_key_in);   // convert into key press  
  if (key != oldkey)    // if keypress is detected
   {
    delay(50);    // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);     // convert into key press
    if (key != oldkey)        
    {     
      oldkey = key;
      if (key >=0){
             //Serial.println(key);
             if ( key == 0 )
               {
                uiKeyCodeFirst = KEY_BACK;
                if(x>0) {
                  x--;
                }
                else if(x==0 && y<5){
                  drawReset();
                }
               }
             else if ( key == 1 )
               {
                uiKeyCodeFirst = KEY_SELECT;
                //tu bedzie rysowanie punktu
                drawPoint(x, y);
               }
               
             else if ( key == 2 )
               {
                uiKeyCodeFirst = KEY_NEXT;
                if(y<WYS){
                  y++;
                }
                
               }
             else if ( key == 3 )
               {
                uiKeyCodeFirst = KEY_SELECT;
                if(x<SZER*16){
                  x++;
                }
               }
               
             else if ( key == 4 )
               {
                uiKeyCodeFirst = KEY_PREV;
                if(y>0){
                 y--; 
                }
                
               }
             else 
               uiKeyCodeFirst = KEY_NONE;
  
             uiKeyCode = uiKeyCodeFirst;           
      }
    }
  }
 delay(50);
}




void drawPoint(int x, int y){
  int komorkaX, bitX;
    komorkaX=x/16;
    bitX=x%16;
    //komorkaY=y/16;
    //bitY=y%16;

    Tabyx[komorkaX][y] |=1<<bitX;
    printTab();
  //Tabx[x]=1;
    
  
  //Taby[y]=1;
  drawRefresh();
  
}

void printTab(void){
  int a, b;
  for(b=0;b<WYS;b++){
    for(a=0;a<SZER;a++){
      Serial.print(Tabyx[a][b]);
    }
    Serial.println();
  }
  Serial.println("\n");
}

void drawRefresh(void){
  int a=0;
  int b=1;
  int c=1;
  int i=0;
  for(b=1;b<WYS;b++){
  //Serial.println("sprawdzam");
  i=0;
  for(a=0;a<SZER;a++){
    if((Tabyx[a][b] & 1)==1){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+1, b+11, rad);
    }
    if((Tabyx[a][b] & 2)==2){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+2, b+11, rad);
    }
    if((Tabyx[a][b] & 4)==4){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+3, b+11, rad);
    }
    if((Tabyx[a][b] & 8)==8){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+4, b+11, rad);
    }
    if((Tabyx[a][b] & 16)==16){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+5, b+11, rad);
    }
    if((Tabyx[a][b] & 32)==32){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+6, b+11, rad);
    }
    if((Tabyx[a][b] & 64)==64){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+7, b+11, rad);
    }
    if((Tabyx[a][b] & 128)==128){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+8, b+11, rad);
    }
    if((Tabyx[a][b] & 256)==256){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+9, b+11, rad);
    }
    if((Tabyx[a][b] & 512)==512){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+10, b+11, rad);
    }
    if((Tabyx[a][b] & 1024)==1024){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+11, b+11, rad);
    }
    if((Tabyx[a][b] & 2048)==2048){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+12, b+11, rad);
    }
    if((Tabyx[a][b] & 4096)==4096){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+13, b+11, rad);
    }
    if((Tabyx[a][b] & 8192)==8192){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+14, b+11, rad);
    }
    if((Tabyx[a][b] & 16384)==16384){
      Serial.println("rysuje");
      u8g.drawCircle(a*16+15, b+11, rad);
    }
    if((Tabyx[a][b] & -32768)==-32768){
      Serial.println("rysuje");
      u8g.drawCircle((a+1)*16, b+11, rad);
    }
    
  }
    
  }
  
  /*for (;b<WYS;b++){
    for(a=0;a<SZER;a++){
      for(c=1, i=0;c<=16400;c*=2, i++){
        if((Tabyx[a][b] & c) == c){
          u8g.drawCircle(a*16+i, b, 3);
        }
      }
    }
  }*/
  
  //Serial.println("skonczylem");
  /*
  for(a=0;a<126;a++){
    for(b=0;b<52;b++){
      if(Tabx[b]==1){
        if(Taby[a]==1){
          u8g.drawCircle(a, b+10, 3);
        }
      }
    }
  }*/
}

void drawReset(){
  int a;
  int b;
  for(a=0; a<SZER; a++){
    for(b=0;b< WYS; b++){
      Tabyx[a][b]=0;
    //Taby[b]=0;
  }
    //Tabx[a]=0;
  }
  
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);

  u8g.drawStr(0, 10, "X=    Y=");
  sprintf(bufx, "%d", x);
  u8g.drawStr( 15, 10, bufx);
  
  sprintf(bufy, "%d", y);
  u8g.drawStr( 65, 10, bufy);
  
  u8g.drawFrame(0,11,128,53);
  
  drawRefresh();

  
}

void setup(void) {
  Serial.begin(9600);
  
  // flip screen, if required
   u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(1);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
    uiStep();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(100);
}

