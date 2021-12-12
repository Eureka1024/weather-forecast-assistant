
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
 */
#include "common.h"
#include "TFT_eSPI.h"
#include "Seeed_FS.h" //Including SD card library
#include "RawImage.h"  //Including image processing library
#include "Free_Fonts.h" //include the header file
#include <millisDelay.h>
#include "RTC_SAMD51.h"
#include "DateTime.h"

void GetWeatherData(void);
void initLCD(void);
void displayLCD(void);

TFT_eSPI tft;

struct weatherDef weather;
long last_time_get_data; //用于间隔获取天气数据
long last_time_lcd;
void setup() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial to be ready

    connectWiFi();
    last_time_get_data = millis(); //用于获取温度的间隔时间标记
    last_time_lcd = millis(); 
    setTime();
    initLCD();
    initButton();
}
 
void loop() {
    checkButton();
//     GetWeatherData();
//     updateNTPtime();
//     displayLCD();
}

void initLCD(void){
    //Initialise SD card
    if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
        while (1);
    }

    tft.begin();
    tft.setRotation(3);
 
    //drawImage<uint8_t>("picture/lwr.bmp", 0, 0); //显示8-bit图片,16bit无法使用功能
    
    tft.fillScreen(TFT_BLACK); //Black background

    tft.setFreeFont(&FreeSansBoldOblique18pt7b); //select Free, Sans, Bold, Oblique, 24pt.
    tft.drawString("Shenzhen",150,140);
    tft.setFreeFont(&FreeSansBoldOblique12pt7b); //select Free, Sans, Bold, Oblique, 12pt.
}

void displayLCD(void){
    if (millis()- last_time_lcd >= 1000)//1s
    {
        last_time_lcd = millis();
        //天气
        String strLcd = "Weather: ";
        strLcd += weather.nowWeather;
        tft.drawString(strLcd,0,0);

        //温度
        strLcd = "Temperature: ";
        strLcd += weather.nowTemperature;
        tft.drawString(strLcd,0,30);

        //湿度
        strLcd = "Humidity: ";
        strLcd += weather.nowHumidity;
        tft.drawString(strLcd,0,60);

        //空气质量
        strLcd = "Air Quality: ";
        strLcd += weather.airQuality;
        tft.drawString(strLcd,0,90);

        //时间
        tft.drawString(systemTime.nowDay,180,180);
        tft.drawString(systemTime.nowTime,190,210);
    }
}

//每隔20s获取温度数据
void GetWeatherData(void)
{
    if (millis()- last_time_get_data >= 20000)//20s
    {
      last_time_get_data = millis();
        
      httpRequestData(WEATHER);//请求当前天气 
      httpRequestData(AIR);//请求空气质量
      httpRequestData(THREE_DAYS);//请求未来三天的天气
    }
}