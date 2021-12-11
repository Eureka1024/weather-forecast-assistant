
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
 */
#include "common.h"
#include "TFT_eSPI.h"
#include "Seeed_FS.h" //Including SD card library
#include "RawImage.h"  //Including image processing library
#include "Free_Fonts.h" //include the header file

void GetWeatherData(void);

TFT_eSPI tft;

struct weatherDef weather;
long last_time_get_data; //用于间隔获取天气数据

void setup() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial to be ready
    //Initialise SD card
    if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
        while (1);
    }
    tft.begin();
    tft.setRotation(3);
 
    //drawImage<uint8_t>("picture/lwr.bmp", 0, 0); //显示8-bit图片,16bit无法使用功能
    
    tft.fillScreen(TFT_BLACK); //Black background
    tft.setFreeFont(&FreeSansBoldOblique12pt7b); //select Free, Sans, Bold, Oblique, 12pt.
    //tft.drawString("Sans Serif 12pt",70,80);//prints string at (70,80)
    connectWiFi();
    last_time_get_data = millis();
}
 
void loop() {
    GetWeatherData();
}

//每隔20s获取温度数据
void GetWeatherData(void)
{
    if (millis()- last_time_get_data >= 10000)//20s
    {
      last_time_get_data = millis();
        
      httpRequestData(WEATHER);//请求当前天气 
      httpRequestData(AIR);//请求空气质量
      httpRequestData(THREE_DAYS);//请求未来三天的天气

      tft.drawString(weather.nowWeather,0,0);
      tft.drawString(weather.nowHumidity,0,50);
      tft.drawString(weather.nowTemperature,0,100);
    }
}