void serialEvent() { //Чтение данных от PC
  static uint8_t index;
  static bool flagSysSet;
  //static boolean getStarted; //Старт парсинга от PC
  while (Serial.available() > 0) {
    if (index > 14) index = 0; //!!!!!!!!!!!!!! ДБ! Радар что-то еще передает и массив преполняется!!!!!
    wdt_reset();
    byte incomingByte = Serial.read();        //  ЧИТАЕМ входящий символ
    //Serial.print("Recieved serialEvent ");Serial.println((char)incomingByte); //****************************************
    //if (getStarted) {                         // если приняли начальный символ (парсинг разрешён)
    if (incomingByte != ';' && incomingByte != 13 && incomingByte != 10 && incomingByte != '$') {   // если это не пробел И не конец  
          dataFromPC[index] = incomingByte;
          index++;
          countResSym++;
    }
    if (incomingByte == '$') {
      flagSysSet = true;  
    }
    if (incomingByte == ';') {                // если приняли ; - конец парсинга
       //getStarted = false;
       index = 0;
       timeCount = millis(); //Сбрасываем счетчик для начала отсчета времени отображения последней цифры
       if (flagSysSet) {
        flagSysSet = false;
        stateCommun = 2; //System setup 
        break; //ЧТОБЫ ПЕРЕДАТЬ УПРАВЛЕНИЕ LOOP или так и будем крутиться в WHILE
       }
       else {
        flagSysSet = false;
        stateCommun = 1; //Start transmit to LC
        break; //ЧТОБЫ ПЕРЕДАТЬ УПРАВЛЕНИЕ LOOP или так и будем крутиться в WHILE
       }       
    } 
    if (incomingByte == 13 || incomingByte == 10) {                // если приняли ; - конец парсинга
       //Serial.read(); //Читаем второй символ LF (10)
       index = 0;
       timeCount = millis(); //Сбрасываем счетчик для начала отсчета времени отображения последней цифры
       flagSysSet = false;
       stateCommun = 1; //Start transmit to LC  
       break; //ЧТОБЫ ПЕРЕДАТЬ УПРАВЛЕНИЕ LOOP или так и будем крутиться в WHILE    
    }     
  }

}//END Чтение данных от PC и парсинг