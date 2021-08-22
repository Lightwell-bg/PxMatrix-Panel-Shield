#include <Arduino.h>
#include "config.h" 
#include "display.h"
#include "eepromfun.h"
#include "serialevent.h"

void setup() {
  EEPROM.begin(512);
  EEPROM.get(addrSpeed, valSpeedCom);
  uint32_t valSpeedComEEPROM = valSpeedCom;
  if  (valSpeedCom != 9600 && valSpeedCom != 19200 && valSpeedCom != 38400 && valSpeedCom != 57600 && valSpeedCom != 74880 && valSpeedCom != 115200) {
    valSpeedCom = 9600;  
  }
  Serial.begin(valSpeedCom); // скорость 9600 38400
  EEPROM.get(addrSpeed+4, valBrightness);
  EEPROM.get(addrSpeed+5, valTimeShow);
  Serial.print("valBrightness ");Serial.println(valBrightness); //****************************************
  Serial.print("valTimeShow ");Serial.println(valTimeShow); //****************************************
  Serial.print("valSpeedCom ");Serial.println(valSpeedCom); //****************************************
  Serial.print("valSpeedComEEPROM from EEPROM ");Serial.println(valSpeedComEEPROM); //****************************************
  initDisplay();
  scroll_text(13, 50, "LIGHTWELL", 96, 96, 250);
  String info = "com: " + String(valSpeedCom) + ", br: " + String(valBrightness) + ", d: " + String(valTimeShow/1000);
  scroll_text(13 ,50, info ,96, 96, 250);
}

void loop() {
  serialEvent();
  if (stateCommun == 1) { //Данные от PC получены, отображение
    display.clearDisplay();
    //display.setFont(&FreeSansBold9pt7b);
    display.setFont(&arial_black10pt7b);
    //Serial.print("countResSym ");Serial.println(countResSym); //****************************************    
    if (countResSym <= 3) { //Отображаем только если число принятых символов не больше 3 (больше быть не может)
      uint8_t speedInt = 0;
      switch (countResSym) {//Преобразуем полученные символы в число
        case 3: 
          speedInt = (dataFromPC[0]-'0')*100 + (dataFromPC[1]-'0')*10 + (dataFromPC[2]-'0');
        break;
        case 2: 
          speedInt = (dataFromPC[0]-'0')*10 + (dataFromPC[1]-'0');
        break;
        case 1: 
          speedInt = (dataFromPC[0]-'0');
        break; 
        default:
        break;             
      }
      (speedInt < 10 ? recievedMsg = "0" : recievedMsg = "");
      for (byte i=0; i < countResSym; i++) {  
        recievedMsg += (char)dataFromPC[i]; 
        //Serial.print("dataFromPC["); Serial.print(i);Serial.print("]");Serial.println((char)dataFromPC[i]); //******************************************
        if (dataFromPC[i] < 48 || dataFromPC[i] > 57) { //Отображаем только цифры, если ошибка, то пробелы.
          recievedMsg = "--";  
        }
      }
      //Serial.print("recievedMsg ");Serial.println(recievedMsg); //******************************************
      if (speedInt > MAX_SPEED) {
        display.setTextColor(myRED);
        display.setCursor(2,14);
        display.print(recievedMsg);
        display.setFont(&FreeMono7pt8b);
        display.setTextColor(myWHITE);
        display.setCursor(30,13);
        display.print(F("km/h"));       
      }
      else {
        display.setTextColor(myGREEN);
        display.setCursor(2,14);
        display.print(recievedMsg);
        display.setFont(&FreeMono7pt8b);
        display.setTextColor(myWHITE);
        display.setCursor(30,13);
        display.print(F("km/h"));            
      }
    }
    countResSym = 0;
    stateCommun = 0;
  }

  if (stateCommun == 2) { //System Setup
    display.setFont(&FreeMono9pt7b);
    display.clearDisplay();
    display.setTextColor(myBLUE);
    display.setCursor(0,13);
    switch (dataFromPC[0]-'0') {//Преобразуем команду в число
      case 1: //Установка яркости
        valBrightness  = (dataFromPC[1]-'0')*100 + (dataFromPC[2]-'0')*10 + (dataFromPC[3]-'0');
        EEPROM.put(addrSpeed+4, valBrightness);
        display.setBrightness(valBrightness);
        display.print(valBrightness);
        Serial.print("valBrightness ");Serial.println(valBrightness); //****************************************
        delay(500);
      break;
      case 2: //времени покзаза после окончания измерения
        valTimeShow  = (dataFromPC[1]-'0')*10000 + (dataFromPC[2]-'0')*1000 + (dataFromPC[3]-'0')*100 + (dataFromPC[4]-'0')*10 + (dataFromPC[5]-'0');
        EEPROM.put(addrSpeed+5, valTimeShow);
        display.print(valTimeShow);
        Serial.print("valTimeShow ");Serial.println(valTimeShow); //****************************************
        delay(500);
        timeCount = millis();
      break;  
      case 3: //времени покзаза после окончания измерения
        valSpeedCom  = (dataFromPC[1]-'0')*100000 + (dataFromPC[2]-'0')*10000 + (dataFromPC[3]-'0')*1000 + (dataFromPC[4]-'0')*100 + (dataFromPC[5]-'0')*10 + (dataFromPC[6]-'0');
        EEPROM.put(addrSpeed, valSpeedCom);
        Serial.begin(valSpeedCom); // скорость 9600 38400
        display.print(valSpeedCom);
        Serial.print("valSpeedCom ");Serial.println(valSpeedCom); //****************************************
        delay(500);
      break;                
      default:
      break;             
    } 
    EEPROM.commit();
    delay(200);
    display.clearDisplay();
    stateCommun = 0;   
  }

  if ((millis() - timeCount) > valTimeShow) { //Выключаем отображение на экран, если прошло болше заданного времени
    display.clearDisplay();
    timeCount = millis();  
  }


  /*scroll_text(13,50,"Welcome to PxMatrix!",96,96,250);
  display.clearDisplay();
  display.clearDisplay();
  display.setTextColor(myRED);
  display.setCursor(2,13);
  display.print("Pixel");
  delay(3000);
  display.clearDisplay();
  display.setTextColor(myGREEN);
  display.setCursor(2,13);
  display.print("Time");
  //display_update_enable(true);
  Serial.println("Start");
  delay(3000);*/
}