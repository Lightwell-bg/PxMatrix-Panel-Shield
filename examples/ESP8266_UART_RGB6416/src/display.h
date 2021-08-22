void display_update_enable(bool is_enable);

void initDisplay() {
    // Define your display layout here, e.g. 1/8 step, and optional SPI pins begin(row_pattern, CLK, MOSI, MISO, SS)
    display.begin(4);
    //display.begin(8, 14, 13, 12, 4);
    display.setFastUpdate(true);
    // Set the multiplex pattern {LINE, ZIGZAG,ZZAGG, ZAGGIZ, WZAGZIG, VZAG, ZAGZIG} (default is LINE)
    //display.setScanPattern(ZIGZAG);
    // Define multiplex implemention here {BINARY, STRAIGHT} (default is BINARY)
    display.setMuxPattern(BINARY);
    //display.setMuxPattern(STRAIGHT);
    display.setScanPattern(ZAGZIG);
     // Rotate display
    //display.setRotate(true);
    // Flip display
    //display.setFlip(true);
    // Control the minimum color values that result in an active pixel
    //display.setColorOffset(5, 5,5);
        // Set the color order {RRGGBB, RRBBGG, GGRRBB, GGBBRR, BBRRGG, BBGGRR} (default is RRGGBB)
    //display.setColorOrder(RRGGBB);
    // Set the time in microseconds that we pause after selecting each mux channel
    // (May help if some rows are missing / the mux chip is too slow)
    //display.setMuxDelay(0,1,0,0,0);
    // Set the number of panels that make up the display area width (default is 1)
    display.setPanelsWidth(2);
    // Set the brightness of the panels (default is 255)
    display.setBrightness(255);
    // Set driver chip type
    //display.setDriverChip(FM6124);
    display.clearDisplay();
    display.setFont(&FreeSansBold9pt7b);
    //display.setFont(&FreeSansBold12pt7b);
    display.setTextColor(myCYAN);
    display.setCursor(2,13);
    //display.print("Pixel");
    display.setTextColor(myMAGENTA);
    display_update_enable(true);
    Serial.println("Start");
    delay(3000);
}



// ISR for display refresh
void display_updater() {
  display.display(display_draw_time);
}

void display_update_enable(bool is_enable) {
  if (is_enable)
    display_ticker.attach(0.004, display_updater);
  else
    display_ticker.detach();
}

void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB) {
    uint16_t text_length = text.length();
    display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
    display.setTextSize(1);
    display.setRotation(0);
    display.setTextColor(display.color565(colorR,colorG,colorB));

    // Asuming 5 pixel average character width
    for (int xpos=matrix_width; xpos>-(matrix_width+text_length*5); xpos--)    {
      display.setTextColor(display.color565(colorR,colorG,colorB));
      display.clearDisplay();
      display.setCursor(xpos,ypos);
      display.println(text);
      delay(scroll_delay);
      yield();

      // This might smooth the transition a bit if we go slow
      // display.setTextColor(display.color565(colorR/4,colorG/4,colorB/4));
      // display.setCursor(xpos-1,ypos);
      // display.println(text);

      delay(scroll_delay/5);
      yield();

    }
}
