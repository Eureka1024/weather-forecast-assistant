#ifndef _COMMON_H
#define _COMMON_H

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
  const char* nowWeather;     //当前天气
  const char* nowTemperature; //当前温度
  const char* nowHumidity;    //当前湿度
  const char* airQuality;     //空气质量
  struct futureWeatherDef futureDays[3];//未来三天的天气
};


extern struct weatherDef weather;

void connectWiFi(void);
void httpRequestData(enum dataType type);
#endif