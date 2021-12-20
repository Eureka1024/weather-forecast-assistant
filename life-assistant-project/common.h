#ifndef _COMMON_H
#define _COMMON_H

//WIFI和http请求数据
#include "rpcWiFi.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "TFT_eSPI.h"
#include "Seeed_FS.h" //Including SD card library
#include "RawImage.h"  //Including image processing library
#include "Free_Fonts.h" //include the header file
#include <millisDelay.h>
#include "RTC_SAMD51.h"
#include "DateTime.h"
#include"Histogram.h" //include histogram library

#include "PersonalConfig.h"

enum dataType{
  WEATHER = 0,
  AIR,
  THREE_DAYS
};

enum buttonType{
  WIO_BUTTON_NONE = 0,
  WIO_5S_UP_EVENT,
  WIO_5S_DOWN_EVENT,
  WIO_5S_LEFT_EVENT,
  WIO_5S_RIGHT_EVENT,
  WIO_5S_PRESS_EVENT
};

struct futureWeatherDef{
  char weather[10];
  char humidity[5];
  char lowTemperature[5];
  char highTemperature[5];
};

struct weatherDef{
  char nowWeather[10];    //当前天气
  char nowTemperature[5]; //当前温度
  char nowHumidity[5];    //当前湿度
  char airQuality[10];    //空气质量
  char PM2_5[5];          //PM2.5
  char aqi[5];            //aqi指数
  struct futureWeatherDef futureDays[3];//未来三天的天气
};

struct timeDef{
  char nowDay[15];        //当前日期
  char nowTime[15];       //当前时间
};

extern struct weatherDef weather;
extern struct timeDef systemTime;
void connectWiFi(void);
void httpRequestData(enum dataType type);

void setTime(void);
void updateNTPtime();

void initButton();
void checkButton();
extern enum buttonType buttonState;


void initMenu(void);
void SwitchMenu(void);

#endif
