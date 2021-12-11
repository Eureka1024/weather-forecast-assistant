#ifndef _COMMON_H
#define _COMMON_H

//WIFI和http请求数据
#include "rpcWiFi.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include "PersonalConfig.h"

enum dataType{
  WEATHER = 0,
  AIR,
  THREE_DAYS
};

struct futureWeatherDef{
  const char* weather;
  const char* humidity;
  const char* lowTemperature;
  const char* highTemperature;
};

struct weatherDef{
  char nowWeather[10];     //当前天气
  char nowTemperature[5]; //当前温度
  char nowHumidity[5];    //当前湿度
  char airQuality[10];     //空气质量
  struct futureWeatherDef futureDays[3];//未来三天的天气
};


extern struct weatherDef weather;

void connectWiFi(void);
void httpRequestData(enum dataType type);
#endif
