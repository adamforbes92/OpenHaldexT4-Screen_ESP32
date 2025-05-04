/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */

void mDyn_mode(uint8_t line)
// *********************************************************************
{
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos()) {
    // make only an action when the cursor stands on this menu item
    //check Button
    if (LCDML.BT_checkAny()) {
      if (LCDML.BT_checkEnter()) {
        ignoreSerialBT = false;
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if (LCDML.MENU_getScrollDisableStatus() == 0) {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        } else {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }

        // do something
        // ...
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkUp()) {
        ignoreSerialBT = true;
        lastMode += 1;
        if (lastMode > 2) {
          if (isStandalone) {
            lastMode = 1;
          } else {
            lastMode = 0;
          }
        }
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkDown()) {
        ignoreSerialBT = true;
        lastMode -= 1;
        if (isStandalone) {
          if (lastMode < 1) {
            lastMode = 2;
          }
        } else {
          if (lastMode < 0) {
            lastMode = 2;
          }
        }
      }

      if (LCDML.BT_checkLeft()) {
      }
      if (LCDML.BT_checkRight()) {
      }
    }
  }

  char buf[20];
  switch (lastMode) {
    case 0:
      state.mode = MODE_STOCK;
      sprintf(buf, "Mode: Stock");
      break;
    case 1:
      state.mode = MODE_FWD;
      sprintf(buf, "Mode: FWD");
      break;
    case 2:
      state.mode = MODE_5050;
      sprintf(buf, "Mode: 5050");
      break;
    default:
      sprintf(buf, "Error!");
      break;
  }

  //LCDML.loop_menu();

  // use the line from function parameters
  lcd.setCursor(1, (line + _LCDML_DSIP_use_header));
  lcd.print(buf);
}

void mDyn_isPaired(uint8_t line)
// *********************************************************************
{
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos()) {
    // make only an action when the cursor stands on this menu item
    //check Button
    if (LCDML.BT_checkAny()) {
      if (LCDML.BT_checkEnter()) {
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if (LCDML.MENU_getScrollDisableStatus() == 0) {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        } else {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }

        // do something
        // ...
        if (!btConnected) {
          //LCDML.MENU_enScroll();
          //btInit();
        }
        btInit();
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkUp()) {
        //btConnected = !btConnected;
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkDown()) {
        //btConnected = !btConnected;
      }
    }
  }

  char buf[20];
  if (btConnected) {
    sprintf(buf, "Is Paired: Y");
  } else {
    sprintf(buf, "Is Paired: N");
  }
  //LCDML.loop_menu();

  // use the line from function parameters
  lcd.setCursor(1, (line + _LCDML_DSIP_use_header));
  lcd.print(buf);
}

void mDyn_isStandalone(uint8_t line)
// *********************************************************************
{
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos()) {
    // make only an action when the cursor stands on this menu item
    //check Button
    if (LCDML.BT_checkAny()) {
      if (LCDML.BT_checkEnter()) {
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if (LCDML.MENU_getScrollDisableStatus() == 0) {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        } else {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }

        // do something
        // ...
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkUp()) {
        isStandalone = !isStandalone;
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkDown()) {
        isStandalone = !isStandalone;
      }
    }
  }

  char buf[20];
  if (isStandalone) {
    sprintf(buf, "No CAN: Y");
    state.mode = MODE_FWD;
  } else {
    sprintf(buf, "No CAN: N");
  }

  // use the line from function parameters
  lcd.setCursor(1, (line + _LCDML_DSIP_use_header));
  lcd.print(buf);
}

void mDyn_showSplash(uint8_t line)
// *********************************************************************
{
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos()) {
    // make only an action when the cursor stands on this menu item
    //check Button
    if (LCDML.BT_checkAny()) {
      if (LCDML.BT_checkEnter()) {
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if (LCDML.MENU_getScrollDisableStatus() == 0) {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        } else {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }

        // do something
        // ...
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkUp()) {
        showSplash = !showSplash;
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkDown()) {
        showSplash = !showSplash;
      }
    }
  }

  char buf[20];
  if (showSplash) {
    sprintf(buf, "Show Splash: Y");
  } else {
    sprintf(buf, "Show Splash: N");
  }

  // use the line from function parameters
  lcd.setCursor(1, (line + _LCDML_DSIP_use_header));
  lcd.print(buf);
}

void mDyn_showScreensaver(uint8_t line)
// *********************************************************************
{
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos()) {
    // make only an action when the cursor stands on this menu item
    //check Button
    if (LCDML.BT_checkAny()) {
      if (LCDML.BT_checkEnter()) {
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if (LCDML.MENU_getScrollDisableStatus() == 0) {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        } else {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }

        // do something
        // ...
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkUp()) {
        showScreensaver = !showScreensaver;
      }

      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkDown()) {
        showScreensaver = !showScreensaver;
      }
    }
  }

  char buf[20];
  if (showScreensaver) {
    sprintf(buf, "Screensaver: Y");
    LCDML.SCREEN_enable(mFunc_screensaver, 10000);  // set to 10 seconds

  } else {
    sprintf(buf, "Screensaver: N");
    LCDML.SCREEN_disable();
  }

  // use the line from function parameters
  lcd.setCursor(1, (line + _LCDML_DSIP_use_header));
  lcd.print(buf);
}

// *********************************************************************
void mDyn_modeStatus(uint8_t line)
// *********************************************************************
{

  // clear the line manuel because clear the complete content is disabled when a external refreshed function is active
  lcd.setCursor(1, line);
  for (uint8_t i = 0; i < _LCDML_DISP_cols - 3; i++)  // -3 because:
                                                      // -1 for counter from 0 to x
                                                      // -1 for cursor position
                                                      // -1 for scrollbar on the end
  {
    lcd.print(F(" "));
  }

  char buf[20];
  switch (lastMode) {
    case 0:
      sprintf(buf, "Mode: Stock");
      break;
    case 1:
      sprintf(buf, "Mode: FWD");
      break;
    case 2:
      sprintf(buf, "Mode: 5050");
      break;
    default:
      sprintf(buf, "Error!");
      break;
  }

  // use the line from function parameters
  lcd.setCursor(1, line);
  lcd.print(buf);

  // reset initscreen timer when this function is displayed
  //LCDML.SCREEN_resetTimer();

  // check if this function is active (cursor stands on this line)
  if (LCDML.BT_checkAny()) {  // check if any button is pressed (enter, up, down, left, right)
    // LCDML_goToMenu stops a running menu function and goes to the menu
    LCDML.MENU_goRoot();
  }
}

// *********************************************************************
void mDyn_ped(uint8_t line)
// *********************************************************************
{
  // clear the line manuel because clear the complete content is disabled when a external refreshed function is active
  lcd.setCursor(1, line);
  for (uint8_t i = 0; i < _LCDML_DISP_cols - 3; i++)  // -3 because:
                                                      // -1 for counter from 0 to x
                                                      // -1 for cursor position
                                                      // -1 for scrollbar on the end
  {
    lcd.print(F(" "));
  }

  char buf[20];
  sprintf(buf, "Ped %: %d", pedValue);

  // use the line from function parameters
  lcd.setCursor(1, line);
  lcd.print(buf);

  // reset initscreen timer when this function is displayed
  //LCDML.SCREEN_resetTimer();

  // check if this function is active (cursor stands on this line)
  if (LCDML.BT_checkAny()) {  // check if any button is pressed (enter, up, down, left, right)
    // LCDML_goToMenu stops a running menu function and goes to the menu
    LCDML.MENU_goRoot();
  }
}

void mDyn_req(uint8_t line)
// *********************************************************************
{
  // clear the line manuel because clear the complete content is disabled when a external refreshed function is active
  lcd.setCursor(1, line);
  for (uint8_t i = 0; i < _LCDML_DISP_cols - 3; i++)  // -3 because:
                                                      // -1 for counter from 0 to x
                                                      // -1 for cursor position
                                                      // -1 for scrollbar on the end
  {
    lcd.print(F(" "));
  }


  char buf[20];
  sprintf(buf, "Req %: %d", (lockTarget * 100 / 72));

  // use the line from function parameters
  lcd.setCursor(1, line);
  lcd.print(buf);

  // reset initscreen timer when this function is displayed
  //LCDML.SCREEN_resetTimer();

  // check if this function is active (cursor stands on this line)
  if (LCDML.BT_checkAny()) {  // check if any button is pressed (enter, up, down, left, right)
    // LCDML_goToMenu stops a running menu function and goes to the menu
    LCDML.MENU_goRoot();
  }
}

void mDyn_act(uint8_t line)
// *********************************************************************
{
  // clear the line manuel because clear the complete content is disabled when a external refreshed function is active
  lcd.setCursor(1, line);
  for (uint8_t i = 0; i < _LCDML_DISP_cols - 3; i++)  // -3 because:
                                                      // -1 for counter from 0 to x
                                                      // -1 for cursor position
                                                      // -1 for scrollbar on the end
  {
    lcd.print(F(" "));
  }


  char buf[20];
  sprintf(buf, "Act %: %d", haldexEngagement);

  // use the line from function parameters
  lcd.setCursor(1, line);
  lcd.print(buf);

  // reset initscreen timer when this function is displayed
  //LCDML.SCREEN_resetTimer();

  // check if this function is active (cursor stands on this line)
  if (LCDML.BT_checkAny()) {  // check if any button is pressed (enter, up, down, left, right)
    // LCDML_goToMenu stops a running menu function and goes to the menu
    LCDML.MENU_goRoot();
  }
}

void mDyn_speed(uint8_t line)
// *********************************************************************
{
  // clear the line manuel because clear the complete content is disabled when a external refreshed function is active
  lcd.setCursor(1, line);
  for (uint8_t i = 0; i < _LCDML_DISP_cols - 3; i++)  // -3 because:
                                                      // -1 for counter from 0 to x
                                                      // -1 for cursor position
                                                      // -1 for scrollbar on the end
  {
    lcd.print(F(" "));
  }


  char buf[20];
  sprintf(buf, "km/h: %d", vehicleSpeed);

  // use the line from function parameters
  lcd.setCursor(1, line);
  lcd.print(buf);

  // reset initscreen timer when this function is displayed
  //LCDML.SCREEN_resetTimer();

  // check if this function is active (cursor stands on this line)
  if (LCDML.BT_checkAny()) {  // check if any button is pressed (enter, up, down, left, right)
    // LCDML_goToMenu stops a running menu function and goes to the menu
    LCDML.MENU_goRoot();
  }
}