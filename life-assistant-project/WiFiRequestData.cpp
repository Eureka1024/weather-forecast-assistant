
#include "common.h"

const char* ssid = WIFI_SSID; //wifi 账号
const char* password = WIFI_PASSWORD; //wifi 密码
const char* host = "api.seniverse.com";     //将要连接的服务器地址  
const int httpPort = 80;                    //将要连接的服务器端口 

// 心知天气HTTP请求所需信息
String privateKey = PRIVATE_KEY;      //填写心知天气的私钥
String location = "shenzhen";         // 城市
WiFiClient client;
void parseInfo(WiFiClient client, enum dataType type);

void connectWiFi(void){
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
 
  Serial.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    WiFi.begin(ssid, password);
  }
  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println (WiFi.localIP()); // prints out the device's IP address
}


// 向心知天气服务器服务器请求信息并对信息进行解析
void httpRequestData(enum dataType type){
  String reqRes;
  if(type == WEATHER){
    //请求当前天气
    reqRes = "/v3/weather/now.json?key=" + privateKey +
                    + "&location=" + location + "&language=en";
  }
  else if (type == AIR){
    //请求当前城市的空气质量
    reqRes = "/v3/air/now.json?key=" + privateKey +
                  + "&location=" + location + "&language=en";
  }
  else{
    //请求未来三天的天气情况
    reqRes = "/v3/weather/daily.json?key=" + privateKey +
                  + "&location=" + location + "&start=1&days=3"+ "&language=en";
  }
                             
  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  Serial.println(""); 
  Serial.print("Connecting to "); 
  Serial.print(host);
 
  // 尝试连接服务器
  if (client.connect(host, httpPort)){
    Serial.println(" Success!");
 
    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);  
 
    // 获取并显示服务器响应状态行 
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);
 
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) {
      Serial.println("Found Header End. Start Parsing.");
    }  
    // 利用ArduinoJson库解析心知天气响应信息
    parseInfo(client, type); 
  } else {
    Serial.println(" connection failed!");
  }   
  //断开客户端与服务器连接工作
  client.stop(); 
}


/*
 *利用ArduinoJson库解析心知天气响应信息
 */
void parseInfo(WiFiClient client, enum dataType type){
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 1500;//原来210,要分配够
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);
  if(type == WEATHER){
    JsonObject results_0 = doc["results"][0];
    JsonObject results_0_now = results_0["now"];

    strcpy(weather.nowWeather, results_0_now["text"]);//"天气"
    strcpy(weather.nowTemperature, results_0_now["temperature"]);//"温度"
    strcpy(weather.nowHumidity, results_0_now["humidity"]);// "湿度"

    Serial.println(F("======Weahter Now======="));
    Serial.print(F("Weather Now: "));
    Serial.println(weather.nowWeather);
    Serial.print(F("Temperature: "));
    Serial.println(weather.nowTemperature);
    Serial.print(F("humidity: "));
    Serial.println(weather.nowHumidity);
    Serial.println(F("========================"));
  }
  else if (type == AIR){
    JsonObject results_0 = doc["results"][0];
    JsonObject results_0_air = results_0["air"];
    JsonObject results_0_air_city = results_0_air["city"];
    const char* results_0_air_city_aqi = results_0_air_city["aqi"]; // "空气质量指数(AQI)是描述空气质量状况的定量指数"
    const char* results_0_air_city_pm25 = results_0_air_city["pm25"];    //PM2.5颗粒物（粒径小于等于2.5μm）1小时平均值。单位：μg/m³

    strcpy(weather.airQuality, results_0_air_city["quality"]);//空气质量类别，有“优、良、轻度污染、中度污染、重度污染、严重污染”6类

    Serial.println(F("======Air Now======="));
    Serial.print(F("AQI Now: "));
    Serial.println(results_0_air_city_aqi);
    Serial.print(F("PM2.5: "));
    Serial.println(results_0_air_city_pm25);
    Serial.print(F("air quality: "));
    Serial.println(weather.airQuality);
    Serial.println(F("========================"));
  }
  else if (type == THREE_DAYS){
    JsonObject results_0 = doc["results"][0];
    JsonArray results_0_daily_array = results_0["daily"]; //注意是数组

    JsonObject results_daily;
    //未来三天
    for (int index = 0; index < 3; index++){
      results_daily = results_0_daily_array[index];
      weather.futureDays[index].weather = results_daily["text_day"]; //天气
      weather.futureDays[index].highTemperature = results_daily["high"];   //最高温    
      weather.futureDays[index].lowTemperature  = results_daily["low"];    //最低温
      weather.futureDays[index].humidity  = results_daily["humidity"];//湿度

      Serial.print(F("====== 第"));
      Serial.print(index);
      Serial.println(F("天======="));
      Serial.print(F("text_day: "));
      Serial.println(weather.futureDays[index].weather);
      Serial.print(F("high: "));
      Serial.println(weather.futureDays[index].highTemperature);
      Serial.print(F("low: "));
      Serial.println(weather.futureDays[index].lowTemperature);
      Serial.print(F("humidity: "));
      Serial.println(weather.futureDays[index].humidity);
    }
  }
  else{
    Serial.println("type error");
  }
}
