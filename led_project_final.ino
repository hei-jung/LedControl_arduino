
#include <FastLED.h>  //LED 라이브러리
#include <Wire.h>

#define NUMPIXELS 30  //총 LED 개수
#define PIN 6 //입력핀 설정

#define R0 10000
#define T0 25 //room temperature
#define B 4200  //the coefficient of the thermistor
#define SERISR 10000  //seris resistor 10K

CRGB LEDs[NUMPIXELS];
CRGB color = CRGB::Purple;  //default 색상
CRGB blank = CRGB::Black; //꺼진 상태

#include <SoftwareSerial.h> //블루투스 사용하기 위해 포함

#include <Time.h> //시간 표시 기능 위해 포함
#include <TimeLib.h>

int delayval = 100; //주기
int TxPin = 2;
int RxPin = 3;
//int ledPin = 8; //데이터 수신 확인용 led
SoftwareSerial BTSerial(TxPin, RxPin);
char BluetoothData;

int cCount = 0; //colour count
int bCount = 0; //brightness count

void setup() {

  FastLED.delay(3000);  //LED 상태 초기화
  FastLED.addLeds<WS2812B, PIN, GRB>(LEDs, NUMPIXELS); //Check if your LED strip is an RGB or GRB ver. (3rd param)
  
  Serial.begin(9600);
  setTime(17,23,0,23,11,18);  //시간 세팅(2018년 11월 23일 13시 33분 0초 기준)

  /*
   * 한 번 시간 세팅하면 전원이 연결돼있는 한 자동 시간 업데이트 가능
   */
  
  BTSerial.begin(9600); //Bluetooth Module(HC-05)의 기본 Baud Rate = 9600
//  pinMode(ledPin,OUTPUT); //데이터 수신 확인용 led - 출력모드(led 빛의 on/off 상태로 수신 여부 확인)
}

void loop() {  
  sayHello(); //HELLO
  if(BTSerial.available())  //안드로이드에서 데이터가 보내진 경우
  {
      BluetoothData = (char)BTSerial.read();  //안드로이드 사용자가 보낸 데이터를 읽어들임

      switch(BluetoothData){
        case '1':
          displayClock_12();  //시간 12시간 모드로 출력
          delay(3*1000);
          break;
          
        case '2':
          displayClock_24();  //시간 24시간 모드로 출력
          delay(3*1000);
          break;
          
        case '3':
          displayTemperature(); //현재 방 온도 출력
          delay(3*1000);
          break;
          
        case '4':
          displayDate();  //오늘의 날짜 출력
          delay(3*1000);
          break;

        case '5':
          changeColour(); //LED 조명 색깔 바꾸기
          break;
          
        case '6':
          changeBrightness(); //LED 조명 밝기 바꾸기
          break;
          
        default:  break;
      }
  }
}

void displayClock_12(){
  BTSerial.print("CLOCK in 12h mode");
  //digitalWrite(ledPin,HIGH);

  int h = hour();
  if(h > 12)  h = h - 12;

  int hl = h / 10;
  int hr = h % 10;
  int ml = minute() / 10;
  int mr = minute() % 10;

  displaySegments(23, hl);
  displaySegments(16, hr);
  displaySegments(7, ml);
  displaySegments(0, mr);
  LEDs[14] = color; //위에 점 출력
  LEDs[15] = color; //아래 점 출력

  FastLED.show();
}

void displayClock_24(){
  BTSerial.print("CLOCK in 24h mode");
  //digitalWrite(ledPin,LOW);

  int hl = hour() / 10;
  int hr = hour() % 10;
  int ml = minute() / 10;
  int mr = minute() % 10;

  displaySegments(23, hl);
  displaySegments(16, hr);
  displaySegments(7, ml);
  displaySegments(0, mr);
  LEDs[14] = color; //위에 점 출력
  LEDs[15] = color; //아래 점 출력

  FastLED.show();
}

void printDigits(int digits){
  BTSerial.print(": ");
  if(digits < 10)
    BTSerial.print('0');
  BTSerial.print(digits);
} //시간 refresh

void displayTemperature(){
  BTSerial.print("Room TEMPERATURE is ");
  //digitalWrite(ledPin,HIGH);

  float thermistorRValue;

  thermistorRValue = 1023.0/analogRead(0)-1;
  thermistorRValue = SERISR/thermistorRValue;
  
  float temperature;

  temperature = thermistorRValue/R0;  //R/R0
  temperature = log(temperature);
  temperature = temperature/B;
  temperature = temperature+1.0/(T0+273.15);
  temperature = 1.0/temperature;
  temperature -= 273.15;  //kelvins to C

  int tmp1 = temperature/10;
  int tmp2 = ((int)temperature)%10;
  
  displaySegments(23,tmp1);
  displaySegments(16,tmp2);
  displaySegments(7,10);
  displaySegments(0,11);
  LEDs[14] = blank;
  LEDs[15] = blank; //: 출력 안 함

  FastLED.show();

  BTSerial.print(temperature);
  BTSerial.print(" Celsius");
  BTSerial.println();
  delay(100*28);
}

void displayDate(){
  //digitalWrite(ledPin,LOW);

  int ml = month() / 10;
  int mr = month() % 10;
  int dl = day() / 10;
  int dr = day() % 10;

  displaySegments(23,ml);
  displaySegments(16,mr);
  displaySegments(7,dl);
  displaySegments(0,dr);
  LEDs[14] = color; //아래 점만 출력
  LEDs[15] = blank;

  FastLED.show();

  BTSerial.print("Today is ");
  BTSerial.print(month());
  BTSerial.print("/");
  BTSerial.println(day());
}

void changeColour(){
  cCount++; //블루투스 데이터가 '5'일 때(LED 색상 조정 버튼 누를 때)마다 증가시킨다
  //digitalWrite(ledPin,HIGH);

  if(cCount==1){
    color = CRGB::Purple;
  }
  else if(cCount==2){
    color = CRGB::Red;
  }
  else if(cCount==3){
    color = CRGB::Orange;
  }
  else if(cCount==4){
    color = CRGB::Yellow;
  }
  else if(cCount==5){
    color = CRGB::Green;
  }
  else if(cCount==6){
    color = CRGB::Blue;
  }
  else if(cCount==7){
    color = CRGB::Indigo;
  }
  else if(cCount==8){
    color = CRGB::Aquamarine;
  }
  else if(cCount==9){
    color = CRGB::Coral;
  }
  else if(cCount==10){
    color = CRGB::Brown;
    cCount = 0;
  }

  FastLED.showColor(color);
  delay(delayval);
}

void changeBrightness(){
  bCount++; //블루투스 데이터가 '6'일 때(LED 밝기 조절 버튼 누를 때)마다 증가시킨다
  BTSerial.println("CHANGE BRIGHTNESS");
  //digitalWrite(ledPin,LOW);
  uint8_t bright = 0;

  if(bCount==1){
    bright = 3;
    FastLED.setBrightness(bright);
  }
  else if(bCount==2){
    bright = 15;
    FastLED.setBrightness(bright);
  }
  else if(bCount==3){
    bright = 65;
    FastLED.setBrightness(bright);
  }
  else if(bCount==4){
    bright = 128;
    FastLED.setBrightness(bright);
  }
  else if(bCount==5){
    bright = 255;
    FastLED.setBrightness(bright);
    bCount = 0;
  }

  FastLED.showColor(color,bright);
  delay(delayval);
}

void displaySegments(int startindex, int number){

  byte numbers[] = {
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01101111, //9
    0b01100011, //º 10
    0b00111001, //C 11
    0b01110110, //H 12
    0b01111011, //e 13
    0b00110110, //ll  14
    0b01011100, //o 15
  };

  for(int i = 0; i < 7; i++){
    LEDs[i + startindex] = ((numbers[number] & 1 << i) == 1 << i) ? color : CRGB::Black;
  }
}

void sayHello(){
  displaySegments(23,12);
  displaySegments(16,13);
  displaySegments(7,14);
  displaySegments(0,0);
  LEDs[14]=blank;
  LEDs[15]=blank; //: 출력 안 함

  FastLED.show();
}
