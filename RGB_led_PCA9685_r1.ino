//https://github.com/MrBoBo4kA/RGB_led_PCA9685

#include <Wire.h>
#include <EEPROM.h>

#define UP 0
#define SHIFT 100
#define DOWN 200

int x=0;
byte n=0;
unsigned long lm=0;
unsigned long cm;

bool dir=0;  // direction

word br=2047; // brightness 0-4095
byte s=15; // speed

byte k[201]; // 0-100-0
int kn = 0; 
word rgb[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //current color

byte sc=0; //start color 0-R 1-G 2-B
byte scp[]={0,0,0}; //color shift

void setup() {
  //Serial.begin(57600); for debug and tilt angle adjustment
  Wire.begin();
  Wire.setClock(400000);
  Wire.beginTransmission(0x40);
  Wire.write(0x00); //mode 0 set
  Wire.write(B10100000); //reset, AI on
  Wire.endTransmission();
  Wire.beginTransmission(0x40);
  Wire.write(0x01); //mode 1 set
  Wire.write(0x10); //no external driver used, OE input
  Wire.endTransmission();
  Wire.beginTransmission(0x40);
  Wire.write(0xFA); //all set off
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.write(0x00);  
  Wire.endTransmission();

  for(x=0;x<=100;x++) //point value chart 0-100 [0-100] (% of max brightness)
    {k[x]=x;}
  for(x=101;x<=200;x++) //point value chart 100-0 [101-200] (% of max brightness)
    {k[x]=k[x-1]-1;}

  pinMode(4, INPUT_PULLUP); //s++
  pinMode(5, INPUT_PULLUP); //br++
  pinMode(6, INPUT_PULLUP); //br--
  pinMode(7, INPUT_PULLUP); //s--

  if(EEPROM.read(0)==235)
    {
      br=word(EEPROM.read(3),EEPROM.read(2));
      s=EEPROM.read(1);
    }
  else
    {
      EEPROM.write(3,highByte(br));
      EEPROM.write(2,lowByte(br));
      EEPROM.write(1,s);
      EEPROM.write(0,235);
    }

}

void loop() {
  sc++; //change the color of the starting
  if(sc>2){sc=0;}

  switch (sc)
    {
    case 0:
      scp[0]=0; scp[1]=5; scp[2]=10; break;
    case 1:
      scp[0]=5; scp[1]=10; scp[2]=0; break;
    case 2:
      scp[0]=10; scp[1]=0; scp[2]=5; break;
    }

for(x=0;x<101;x++)
  {
    calc(scp[0],UP);
    check();
    out();
    delay(s);
  }

for(x=0;x<101;x++)
  {
    calc(scp[0],SHIFT);
    calc(scp[1],UP);
    check();
    out();
    delay(s);
  }

for(x=0;x<101;x++)
  {
    calc(scp[0],DOWN);
    calc(scp[1],SHIFT);
    calc(scp[2],UP);
    check();
    out();
    delay(s);
  }

for(x=0;x<101;x++)
  {
    calc(scp[0],UP);
    calc(scp[1],DOWN);
    calc(scp[2],SHIFT);
    check();
    out();
    delay(s);
  }

for(x=0;x<101;x++)
  {
    calc(scp[0],SHIFT);
    calc(scp[2],DOWN);
    check();
    out();
    delay(s);
  }

for(x=0;x<101;x++)
  {
    calc(scp[0],DOWN);
    check();
    out();
    delay(s);
  }

delay(100);

dir=!dir;

if(digitalRead(4)&&digitalRead(5)&&digitalRead(6)&&digitalRead(7)==HIGH)
  {
    EEPROM.update(3,highByte(br));
    EEPROM.update(2,lowByte(br));
    EEPROM.update(1,s);
    EEPROM.update(0,235);
/*
    Serial.println("EEPROM");
    Serial.println(EEPROM.read(3));
    Serial.println(EEPROM.read(2));
    Serial.println(EEPROM.read(1));
    Serial.println(EEPROM.read(0));
    Serial.println("END");
*/    
  }

}
void calc(byte z, int y) //z-color, y-shift
  {
    if(dir==1)
      {  
        for(n=0;n<5;n++)
          {
            kn=x-n*25+y;
            kn=constrain(kn, 0, 200);
            rgb[n+z]=map(k[kn],0,100,0,br);
          }
      }
    else
      {
        for(n=0;n<5;n++)
          {
            kn=x-n*25+y;
            kn=constrain(kn, 0, 200);
            rgb[4-n+z]=map(k[kn],0,100,0,br);
          }        
      }
  }

void check()
  {
    cm=millis();
    if(cm-lm>400)
      {
      if(digitalRead(5)==LOW) //br++
        {br*=1.1; if(br>4095){br=4095;}
        //Serial.println(br);
        }
      if(digitalRead(6)==LOW) //br--
        {br*=0.9; if(br<127){br=127;}
        //Serial.println(br);
        }
      if(digitalRead(4)==LOW) //s++
        {s++; if(s>31){s=31;}
        //Serial.println(s);
        }
      if(digitalRead(7)==LOW) //s--
        {s--; if(s<1){s=1;}
        //Serial.println(s);
        }
      lm=cm;
      }
  }

void out()
  {
    for(n=0;n<5;n++)
      {
        Wire.beginTransmission(0x40);
        Wire.write(0x06+n*0x0C);
        Wire.write(0x00); //outR
        Wire.write(0x00);
        Wire.write(lowByte(rgb[0+n]));
        Wire.write(highByte(rgb[0+n]));
        Wire.write(0x00); //outG
        Wire.write(0x00);
        Wire.write(lowByte(rgb[5+n]));
        Wire.write(highByte(rgb[5+n]));
        Wire.write(0x00); //outB
        Wire.write(0x00);
        Wire.write(lowByte(rgb[10+n]));
        Wire.write(highByte(rgb[10+n]));
        Wire.endTransmission();
      }
  }
