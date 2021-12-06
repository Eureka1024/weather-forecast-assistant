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

struct weatherDef weather;


void setup() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial to be ready
    connectWiFi();
}
 
void loop() {

  delay(6000);
  //请求当前天气
  httpRequestData(WEATHER);
  delay(6000);
  //请求空气质量
  httpRequestData(AIR);
  delay(6000);
  //请求未来三天的天气
  httpRequestData(THREE_DAYS);

  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
  delay(6000);
}
