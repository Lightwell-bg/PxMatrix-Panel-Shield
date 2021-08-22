#ifndef GLOBAL_H
#define GLOBAL_H

const String nName = "LWSpeed"; 
const String nVersion = "_v1.0";

#include <Wire.h>
#include <EEPROM.h>
// This is how many color levels the display shows - the more the slower the update
//#define PxMATRIX_COLOR_DEPTH 4
// Defines the speed of the SPI bus (reducing this may help if you experience noisy images)
//#define PxMATRIX_SPI_FREQUENCY 20000000
// Creates a second buffer for backround drawing (doubles the required RAM)
//#define PxMATRIX_double_buffer true

#include <PxMatrix.h>
#include "Fonts/FreeSansBold9pt7b.h"
//#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeMono9pt7b.h"
#include "myFonts/FreeMono7.h"
#include "myFonts/Arial-black10.h"
#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#define matrix_width 32
#define matrix_height 16

// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
uint8_t display_draw_time=70; //30-70 is usually fine

//PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B);
//PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
PxMATRIX display(64,16,P_LAT, P_OE,P_A,P_B,P_C,P_D);
//PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);
//uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};

//SPEED
#define MAX_SPEED 8
uint8_t addrSpeed = 100;

uint8_t dataFromPC[15];
String recievedMsg;
uint8_t countResSym = 0; //Число принятых символов для отображения
volatile uint8_t stateCommun; // состояние обмена (0 - ожидание; 1 - данные приняты, 2 - запись конфигурации в EPROM)
uint32_t timeCount = 0; // счетчик времени

uint32_t valSpeedCom = 9600; //Значение скорости работы COM-port comand - $3009600; valSpeedCom = 9600 bod MAX 
uint8_t valBrightness = 100; //Значение яркости (255 - самое яркое) comand - $1200; valBrightness = 200
uint16_t valTimeShow = 10000; //Значение времени покзаза после окончания измерения comand - $215000; valTimeShow = 15 s MAX 65536


#endif