/*
 * Copyright (c) 2021, Eureka1024 <eureka1024@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-12-12     Eureka1024    the first version
 */

/*
 * Light rain   //小雨
 * Overcast     //阴天
 * Clear        //晴
 */
#include "common.h"

void nowWeatherGUI(void);
void ForecastWeatherGUI(void);

TFT_eSPI tft;
int8_t  menu_index = 0;   //当前状态
long last_time_lcd;
long last_time_lcd1;
long last_time_sec;

/* 菜单界面切换控制函数 */
void SwitchMenu(void)
{
    if (buttonState != WIO_BUTTON_NONE) //如果有按键按下
    {
        if (buttonState == WIO_5S_LEFT_EVENT)//向左
        {
            menu_index -= 1;
            if (menu_index < 0) menu_index = 1;
        }
        else if (buttonState == WIO_5S_RIGHT_EVENT) //向右
        {
            menu_index += 1;
            if (menu_index > 1) menu_index = 0;
        }

        buttonState = WIO_BUTTON_NONE;//清除状态

        last_time_lcd -= 20000;//快速减少刷新时间
    }

    switch (menu_index)
    {
        case 0:         //主界面选项
            nowWeatherGUI();
            break;

        case 1:         //天气预报选项  
            ForecastWeatherGUI();
            break;

        default:
            break;
    }
}


void initMenu(void)
{
    menu_index = 0; //默认在主界面
    last_time_lcd = millis();
    last_time_lcd1 = millis();
    last_time_sec = millis();
    //Initialise SD card
    if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
        while (1);
    }
    tft.begin();
    tft.setRotation(3); //设置方向
}


void nowWeatherGUI(void){

    if (millis() - last_time_sec >= 1000)//1s
    {
        last_time_sec = millis();

        //时间
        //覆盖时间，相当于更新
        drawImage<uint8_t>("picture/timeBackground.bmp", 0, 45);
        tft.setTextColor(TFT_BLACK);
        tft.setFreeFont(FSSBO9);
        tft.drawString(systemTime.nowDay,1,10);
        tft.setTextColor(TFT_PURPLE);
        tft.setFreeFont(&FreeSansBoldOblique24pt7b);
        tft.drawString(systemTime.nowTime,0,45);
    }

    if (millis() - last_time_lcd >= 20000)//20s
    {
        last_time_lcd = millis();
        String strLcd;

        //背景图片
        drawImage<uint8_t>("picture/Background.bmp", 0, 0); 

        //时间
        drawImage<uint8_t>("picture/timeBackground.bmp", 0, 45);//覆盖时间，相当于更新
        tft.setTextColor(TFT_BLACK);
        tft.setFreeFont(FSSBO9);
        tft.drawString(systemTime.nowDay,1,10);
        tft.setTextColor(TFT_PURPLE);
        tft.setFreeFont(&FreeSansBoldOblique24pt7b);
        tft.drawString(systemTime.nowTime,0,45);

        //天气
        String filePath = "picture/";
        filePath += weather.nowWeather;
        filePath += ".bmp";
        drawImage<uint8_t>(filePath.c_str(), 220, 0); //天气图片
        tft.setTextColor(TFT_ORANGE); 
        tft.setFreeFont(&FreeSansBoldOblique9pt7b); //select Free, Sans, Bold, Oblique, 12pt.
        strLcd = weather.nowWeather;
        // strLcd = "Sunny";
        tft.drawString(strLcd,230,70);

        //城市
        tft.setTextColor(TFT_BLACK);   //sets the text colour to black
        tft.setFreeFont(FS9); //select Free, Serif, 9pt.
        tft.drawString("Shenzhen",245,105);

        //温度
        tft.setTextColor(TFT_BLUE);
        tft.setFreeFont(FMBO18); //18pt
        strLcd = weather.nowTemperature;
        // strLcd = "25";
        tft.drawString(strLcd,8,115);

        //湿度
        tft.setTextColor(TFT_BLACK);   //sets the text colour to black
        strLcd = weather.nowHumidity;
        // strLcd = "20";
        tft.drawString(strLcd,100,115);

        //空气质量
        tft.setFreeFont(&FreeSansBoldOblique9pt7b);
        strLcd = "aqi: ";
        strLcd += weather.PM2_5;
        tft.drawString(strLcd,15,150);

        strLcd = "pm2.5: ";
        strLcd += weather.aqi;
        strLcd += " ug/m3";
        tft.drawString(strLcd,15,170);

        strLcd = "air quality: ";
        tft.drawString(strLcd,15,190);

        tft.setFreeFont(&FreeSansBoldOblique12pt7b);
        strLcd = weather.airQuality;
        // strLcd += "Good";
        tft.drawString(strLcd,110,190);
    }
}

void ForecastWeatherGUI(void)
{
    if (millis() - last_time_lcd1 >= 10000)//1s
    {
        last_time_lcd1 = millis();

        String strLcd;
        long i = 0;

        //背景图片
        drawImage<uint8_t>("picture/Sn.bmp", 0, 0); 

        for (int index=0; index<3; index++)
        {
            if(index == 0) i=0;
            if(index == 1) i=80;
            if(index == 2) i=160;
            //日期
            tft.setTextColor(TFT_PURPLE);
            tft.setFreeFont(&FreeSansBoldOblique12pt7b);
            strLcd = weather.futureDays[index].date;
            tft.drawString(strLcd,0,0+i);
            tft.setFreeFont(&FreeSansBoldOblique9pt7b);
            //湿度
            strLcd = "Humidity: ";
            strLcd += weather.futureDays[index].humidity;
            strLcd += "%";
            tft.drawString(strLcd,0,30+i);
            //温度
            strLcd = "Temperature: ";
            strLcd += weather.futureDays[index].lowTemperature;
            strLcd += "~";
            strLcd += weather.futureDays[index].highTemperature;
            strLcd += " C";
            tft.drawString(strLcd,0,50+i);
            //天气
            String filePath = "picture/";
            filePath += weather.futureDays[index].weather;
            filePath += ".bmp";
            drawImage<uint8_t>(filePath.c_str(), 200, 0+i); //天气图片
            strLcd = weather.futureDays[index].weather;
            tft.drawString(strLcd,220,50+i);
        }

    }
}