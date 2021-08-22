bool saveDataEEPROM(uint8_t addr, int32_t valData);
uint32_t loadDataEEPROM(uint8_t addr = 100, int16_t sizeData = 512);

bool saveDataEEPROM(uint8_t addr, int32_t valData) {   // Save configuration from RAM into EEPROM
    EEPROM.put(addr, valData);
    delay(50);
    if (EEPROM.commit()) {                      // Only needed for ESP8266 to get data written
        //EEPROM.end();                           // Free RAM copy of structure
        return true; 
    }
    else {
        //EEPROM.end();                           // Free RAM copy of structure
        return false;         
    }                         
                        
}

uint32_t loadDataEEPROM(uint8_t addr, int16_t sizeData) {  // long	int32_t	4 байта
    Serial.println("Loading data from EEPROM");
    int32_t load;
    EEPROM.begin(sizeData);
    EEPROM.get(addr, load);
    //EEPROM.end();
    // Check if it is your real struct
    /*if (load.myVersion[0] != checkVer[0] ||
        load.myVersion[1] != checkVer[1] ||
        load.myVersion[2] != checkVer[2]) {
            _saveConfigEEPROM(_configWIFI);
            return;
        }*/
    Serial.print("Data from EPROM ");Serial.println(load);
    return load;
}