#include <Adafruit_NeoPixel.h>
#include <stdio.h>
#include <DS1302.h>

//sprawdzić czy ta biblioteka jest niezbędna:
//#ifdef __AVR__
//  #include <avr/power.h>
//#endif

#define PIN            8
#define NUMPIXELS      60
#define BRIGHTNESS 50 
#define DELAYVAL 500
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

#define kCePin  5  // Chip Enable
#define kIoPin   6  // Input/Output
#define kSclkPin 7  // Serial Clock

byte yhour1;
byte ymin1;
byte hour1;
byte min1;
byte seconds;
byte control=0;

 DS1302 rtc(kCePin, kIoPin, kSclkPin); //RTC module 

//Colors sec, min, h
void printTime() {
  Time t = rtc.time(); // < time from RTC module 
  hour1=t.hr;
  min1=t.min;
  seconds=t.sec; 
  if(hour1>=12)
  {
    hour1=hour1-12; //resetting the hours
  }
  pixels.setPixelColor(5*hour1, pixels.Color(25,25,255)); //colors of hour
  pixels.setPixelColor(min1, pixels.Color(0,200,10)); //colors of minutes
  pixels.setPixelColor(seconds, pixels.Color(255,0,0)); //colors of seconds
  
  pixels.show();

  //animations of seconds:
  if(seconds==0)
   { 
    pixels.setPixelColor(min1-1, pixels.Color(0,0,0));
    pixels.setPixelColor(min1, pixels.Color(0,200,10));
     pixels.show();
    for(int i=1;i<60;i++)
      {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
    }
//animations of minutes
   if(min1==0)
  { 
    pixels.setPixelColor(59, pixels.Color(0,0,0)); //cleaning led's
    }
}

//animation, button "up"
void animation()
{
  colorWipe(pixels.Color(255, 0, 0), 25); // Red
  colorWipe(pixels.Color(0, 255, 0), 25); // Green
  colorWipe(pixels.Color(0, 0, 255), 25);
    theaterChase(pixels.Color(127, 127, 127), 50); // White
  theaterChase(pixels.Color(127, 0, 0), 50); // Red
  theaterChase(pixels.Color(0, 0, 127), 50);
  for(int i=1;i<60;i++)
      {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
  
 }
 // colorWipe animation - to understand....
 void colorWipe(uint32_t c, uint8_t wait) {     // c = color of leds
  for(uint16_t i=0; i<pixels.numPixels(); i++) {    // t = time 
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}
//theaterChade animation - to understand....
  void theaterChase(uint32_t c, uint8_t wait) {         // c = color of leds
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, c);    //turn every third pixel (q)
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {        // t = time 
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//RTC:
  void newhour1()
  {
 //control=0;
    hour1=yhour1;
    min1=ymin1;
     Time t(2021, 5, 23,yhour1, ymin1, 0, Time::kFriday); //start time
    rtc.time(t);
    
    }
  
//buttons:
 int clockSet =10;
 int up=11;
 int down=12;

void setup() {
  Serial.begin(9600);
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1); //definition of a microcontroller clock
#endif
pixels.begin();
  pixels.show();
pixels.begin();

pinMode( clockSet,INPUT); //button clocset

 //to understand...
  rtc.writeProtect(false);
  rtc.halt(false);


  //my led's
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // INITIALIZE NeoPixel pixels object (REQUIRED)
}

//time change: 
void loop() {

  if (control==1)
  {
    pixels.setPixelColor(5*yhour1, pixels.Color(25,25,255));
    pixels.show();
  delay(100);
  pixels.setPixelColor(5*yhour1, pixels.Color(0,0,0));
  pixels.show();
  delay(100);
if (digitalRead(up)==HIGH)
{
  yhour1++;
  while(digitalRead(up)==HIGH);
  if(yhour1==12)
  {
    yhour1=0; //reset hour
  }
}
if (digitalRead(down)==HIGH)
{
  yhour1--;
  while(digitalRead(down)==HIGH);
  if(yhour1==255)
  {
    yhour1=11;
  }
}
 
  pixels.setPixelColor(5*yhour1, pixels.Color(25,25,255));
  pixels.show();
  delay(100);
  pixels.setPixelColor(5*yhour1, pixels.Color(0,0,0));
  pixels.show();
  delay(100);
  newhour1();
  if(digitalRead(clockSet)==HIGH)
  { 
  while(digitalRead(clockSet)==HIGH)
  { pixels.setPixelColor(control, pixels.Color(255,255,255));
   pixels.show();
   }
   pixels.setPixelColor(control, pixels.Color(0,0,0));
   pixels.show();
   control++;
   yhour1=hour1;ymin1=min1;}
  
    }
 if (control==2)
  {
    if(digitalRead(clockSet)==HIGH)
  { 
    control++;
  while(digitalRead(clockSet)==HIGH)
  {    }
   
   yhour1=hour1;ymin1=min1;
   }
   
  if (digitalRead(up)==HIGH)
{
  ymin1++;
  while(digitalRead(up)==HIGH);
  if(ymin1==60)
  {
    yhour1=0; //reset minutes
  }
}
if (digitalRead(down)==HIGH)
{
  ymin1--;
  while(digitalRead(down)==HIGH);
  if(ymin1==255)
  {
    ymin1=59;
  }
} 
  pixels.setPixelColor(ymin1, pixels.Color(0,205,10));
  pixels.show();
  newhour1();
  delay(100);
  pixels.setPixelColor(ymin1, pixels.Color(0,0,0));
     pixels.show();
     delay(100); 
    }
  if (control==3)
  {
    control=0;
     newhour1();
  }
  while (control==0)
  {
    if(digitalRead(up)==HIGH)
  {
    animation ();
  }
    if (digitalRead(down) == HIGH)
  {
      //my animation - in progress....
        pixels.clear(); // Set all pixel colors to 'off'
        for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
         pixels.setPixelColor(i, pixels.Color(0, 150, 0));
         pixels.show();  
         delay(200);
            if( i<59)
            {
           pixels.setPixelColor(i, pixels.Color(0,0,0));
             }}
  }
    if(digitalRead(clockSet)==HIGH)
  { 
    control++;
  while(digitalRead(clockSet)==HIGH);
  yhour1=hour1;ymin1=min1;}
  printTime();
  }
}
