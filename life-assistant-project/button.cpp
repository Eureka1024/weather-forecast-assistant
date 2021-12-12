#include "common.h"

enum buttonType buttonState;
long last_time_check_button;

void initButton() {
    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);
    pinMode(WIO_5S_LEFT, INPUT_PULLUP);
    pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);

    buttonState = WIO_BUTTON_NONE;
    last_time_check_button = millis(); //用于获取温度的间隔时间标记
}
 
void checkButton() {
    if (millis()- last_time_check_button >= 200){ //200ms
        last_time_check_button = millis();

        if (digitalRead(WIO_5S_UP) == LOW) {
            Serial.println("5 Way Up");
            buttonState = WIO_5S_UP_EVENT;
        }
        else if (digitalRead(WIO_5S_DOWN) == LOW) {
            Serial.println("5 Way Down");
            buttonState = WIO_5S_DOWN_EVENT;
        }
        else if (digitalRead(WIO_5S_LEFT) == LOW) {
            Serial.println("5 Way Left");
            buttonState = WIO_5S_LEFT_EVENT;
        }
        else if (digitalRead(WIO_5S_RIGHT) == LOW) {
            Serial.println("5 Way Right");
            buttonState = WIO_5S_RIGHT_EVENT;
        }
        else if (digitalRead(WIO_5S_PRESS) == LOW) {
            Serial.println("5 Way Press");
            buttonState = WIO_5S_PRESS_EVENT;
        }
    }
}