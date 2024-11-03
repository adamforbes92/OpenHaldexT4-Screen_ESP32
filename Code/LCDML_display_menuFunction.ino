/* ===================================================================== *
 * Menu Callback Function                                                *
// *********************************************************************/

void mFunc_aboutMe(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    displayAbout();
  }

  if (LCDML.FUNC_loop())  // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny()) {  // check if any button is pressed (enter, up, down, left, right)
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if (LCDML.FUNC_close())  // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}

// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
  // remmove compiler warnings when the param variable is not used:
  LCDML_UNUSED(param);

  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // update LCD content
    char buf[20];
    char buf1[20];
    if (btConnected) {
      sprintf(buf1, "Paired!");
    } else {
      sprintf(buf1, "Not Paired!");
    }

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
    // setup function
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(buf1);
    lcd.setCursor(0, 1);
    lcd.print(buf);
    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if (LCDML.FUNC_loop()) {
    if (LCDML.BT_checkAny())  // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if (LCDML.FUNC_close()) {
    // The screensaver go to the root menu
    LCDML.MENU_goRoot();
  }
}

// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // end function and go an layer back
    LCDML.FUNC_goBackToMenu(1);  // leave this function and go a layer back
  }
}

// *********************************************************************
void mFunc_goToRootMenu(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // go to root and display menu
    LCDML.MENU_goRoot();
  }
}