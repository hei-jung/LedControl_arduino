# LedControl_arduino

This was written for my final project. I've got the idea from a [YouTube video](https://youtu.be/1aNHF5tcTpw).<br>
[유튜브 영상](https://youtu.be/1aNHF5tcTpw)에서 아이디어를 따와, 진행한 캡스톤 기말 프로젝트.


## 1. Introduction

> Arduino code for controlling LEDs of DIY FND

직접 만든 FND의 LED를 제어하기 위한 아두이노 코드


## 2. Function

[안드로이드](https://github.com/heijung1201/LedControl)에서 다음 기능을 사용할 수 있도록 블루투스 통신하여 LED 제어.

```
(1) 현재 시간(12시간) | Display Current Local Time(12 hours mode)
(2) 현재 시간(24시간) | Display Current Local Time(24 hours mode)
(3) 현재 기온 표시    | Display Room Temperature
(4) 오늘 날짜 표시    | Display Today's Date
(5) LED 색상 조정     | Change LED Colour
(6) LED 밝기 조절     | Change LED Brightness
```

└ Connect Arduino board with [Android App](https://github.com/heijung1201/LedControl) through Bluetooth.
LEDs will be controlled by the Arduino board.


## 3. Implementation

- For LED control
  - IDE: Arduino IDE
  - Language: C++
- Circuit
![회로도](https://user-images.githubusercontent.com/40985307/92224141-5c7af800-eedc-11ea-9de5-ed11bd999630.jpg)

