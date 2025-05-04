
void initLCD() {
  // LCD Begin
  lcd.init();
  lcd.backlight();

  // set special chars for scrollbar
  lcd.createChar(0, (uint8_t*)scroll_bar[0]);
  lcd.createChar(1, (uint8_t*)scroll_bar[1]);
  lcd.createChar(2, (uint8_t*)scroll_bar[2]);
  lcd.createChar(3, (uint8_t*)scroll_bar[3]);
  lcd.createChar(4, (uint8_t*)scroll_bar[4]);

  LCDML_setup(_LCDML_DISP_cnt);                   // LCDMenuLib Setup
  LCDML.MENU_enRollover();                        // Enable Menu Rollover
  LCDML.SCREEN_enable(mFunc_screensaver, 10000);  //   // Enable Screensaver (screensaver menu function, time to activate in ms)
}

void displayAbout() {
  // update LCD content
  char bufScreen[20];
  char bufBoard[20];
  sprintf(bufScreen, "%d.%d%d%", (screenSoftwareVersion / 100) % 10, (screenSoftwareVersion / 10) % 10, (screenSoftwareVersion / 1) % 10);
  sprintf(bufBoard, "%d.%d%d%", (boardSoftwareVersion / 100) % 10, (boardSoftwareVersion / 10) % 10, (boardSoftwareVersion / 1) % 10);

  // setup function
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(splash2);
  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(screenVer);
  lcd.setCursor(0, 1);
  lcd.print(bufScreen);

  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(boardVer);
  lcd.setCursor(0, 1);
  lcd.print(bufBoard);

  LCDML.MENU_goRoot();
}

void displaySplash() {
  // update LCD content
  char buf[20];
  sprintf(buf, "%d.%d%d%", (screenSoftwareVersion / 100) % 10, (screenSoftwareVersion / 10) % 10, (screenSoftwareVersion / 1) % 10);
  // setup function
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(splash1);
  lcd.setCursor(0, 1);
  lcd.print(splash2);

  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(screenVer);
  lcd.setCursor(0, 1);
  lcd.print(buf);
  delay(1500);
}