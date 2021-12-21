
/*
 * name:life assistant
 * dependent libraries：
 *    Wio Terminal板级支持包
 *    Seeed_Arduino_rpcWiFi
 *    Seeed_Arduino_rpcUnified
 *    Seeed_Arduino_mbedtls
 *    Seeed_Arduino_FS
 *    Seeed_Arduino_SFUD
 *    ArduinoJson
 *    Seeed_Arduino_RTC
 *    millisDelay
 *    Seeed_Arduino_Histogram
 *    MsTimer2
 */
#include "common.h"

void GetWeatherData(void);

struct weatherDef weather;
long last_time_get_data; //用于间隔获取天气数据

void setup() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial to be ready

    connectWiFi();
    last_time_get_data = millis(); //用于获取温度的间隔时间标记

    setTime();
    initButton();

    //先获取一次天气数据
    httpRequestData(WEATHER);//请求当前天气 
    httpRequestData(AIR);//请求空气质量
    httpRequestData(THREE_DAYS);//请求未来三天的天气

    initMenu();
}
 
void loop() {
    checkButton();
    SwitchMenu();
    GetWeatherData();
    updateNTPtime();
}


//每隔20s获取温度数据
void GetWeatherData(void)
{
    if (millis() - last_time_get_data >= 80000)//80s
    {
      last_time_get_data = millis();
        
      httpRequestData(WEATHER);//请求当前天气 
      httpRequestData(AIR);//请求空气质量
      httpRequestData(THREE_DAYS);//请求未来三天的天气
    }
}
