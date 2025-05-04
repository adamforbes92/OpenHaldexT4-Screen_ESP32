#pragma once

// library includes
#include <Arduino.h>            // for basic func.
#include <Wire.h>               // for lcd
#include <LiquidCrystal_I2C.h>  // for lcd
#include <LCDMenuLib2.h>        // for menus / lcd
#include <Preferences.h>        // for eeprom/remember settings
#include "openhaldex.h"         // general varables/etc
#include "TickTwo.h"            // for repeated tasks

// LCD & menu definitions
LiquidCrystal_I2C lcd(lcdAddress, _LCDML_DISP_cols, _LCDML_DISP_rows);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void lcdml_menu_display();
void lcdml_menu_clear();
void lcdml_menu_control();
LCDMenuLib2_menu LCDML_0(255, 0, 0, NULL, NULL);  // root menu element (do not change)
LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

// base menu items (first displayed)
LCDML_add(0, LCDML_0, 1, "Haldex Mode", NULL);  // this menu function can be found on "LCDML_display_menuFunction" tab
LCDML_add(1, LCDML_0, 2, "Live Data", NULL);    // this menu function can be found on "LCDML_display_menuFunction" tab
LCDML_add(2, LCDML_0, 3, "Settings", NULL);     // NULL = no menu function

// first layer (Haldex Modes)
LCDML_addAdvanced(3, LCDML_0_1, 1, NULL, "", mDyn_mode, 0, _LCDML_TYPE_dynParam);  // NULL = no menu function
LCDML_add(4, LCDML_0_1, 2, "Back...", mFunc_back);

// second layer (Live Data)
LCDML_addAdvanced(5, LCDML_0_2, 1, NULL, "Mode:", mDyn_modeStatus, 0, _LCDML_TYPE_dynParam_enableCustomRefresh);  // NULL = no menu function
LCDML_addAdvanced(6, LCDML_0_2, 2, NULL, "Ped%:", mDyn_ped, 0, _LCDML_TYPE_dynParam_enableCustomRefresh);         // NULL = no menu function
LCDML_addAdvanced(7, LCDML_0_2, 3, NULL, "Req:", mDyn_req, 0, _LCDML_TYPE_dynParam_enableCustomRefresh);          // NULL = no menu function
LCDML_addAdvanced(8, LCDML_0_2, 4, NULL, "Act:", mDyn_act, 0, _LCDML_TYPE_dynParam_enableCustomRefresh);          // NULL = no menu function
LCDML_addAdvanced(9, LCDML_0_2, 5, NULL, "Speed:", mDyn_speed, 0, _LCDML_TYPE_dynParam_enableCustomRefresh);      // NULL = no menu function

// third layer (Settings)
LCDML_addAdvanced(10, LCDML_0_3, 1, NULL, "", mDyn_isPaired, 0, _LCDML_TYPE_dynParam);         // NULL = no menu function
LCDML_addAdvanced(11, LCDML_0_3, 2, NULL, "", mDyn_isStandalone, 0, _LCDML_TYPE_dynParam);     // NULL = no menu functionLCDML_add(72, LCDML_0_4, 4, "About", mFunc_aboutMe);  // this menu function can be found on
LCDML_addAdvanced(12, LCDML_0_3, 3, NULL, "", mDyn_showSplash, 0, _LCDML_TYPE_dynParam);       // NULL = no menu function
LCDML_addAdvanced(13, LCDML_0_3, 4, NULL, "", mDyn_showScreensaver, 0, _LCDML_TYPE_dynParam);  // NULL = no menu function
LCDML_add(14, LCDML_0_3, 5, "About", mFunc_aboutMe);
LCDML_add(15, LCDML_0_3, 6, "Back...", mFunc_back);

// screensaver
LCDML_addAdvanced(16, LCDML_0, 8, COND_hide, "screensaver", mFunc_screensaver, 0, _LCDML_TYPE_default);  // this menu function can be found on "LCDML_display_menuFunction" tab

#define _LCDML_DISP_cnt 16
LCDML_createMenu(_LCDML_DISP_cnt);

// globals
openhaldexState state;
Preferences pref;

// for repeated tasks (write to EEP & send status)
TickTwo tickEEP(writeEEP, eepRefresh);
TickTwo tickbtSendStatus(btSendStatus, btRefresh);

void setup() {
  initLCD();   // begin LCD
  delay(500);  // let complete

#if stateDebug
  Serial.begin(serialBaud);  // begin Serial talkback
#endif
#if stateDebug
  Serial.println(F("OpenHaldex T4 Screen Initialising..."));  // Serial message for init
#endif

  readEEP();  // read the EEPROM for previous states

  if (showSplash) {
    displaySplash();  // allows time for auto-pair
  }

  // Set Pin Modes (only needed for BT reset - Serial/LCD/rotary are all handled in their respective modules)
  //reset bluetooth module to ensure basic mode
  digitalWrite(pinBT_Conf, LOW);
  digitalWrite(pinBT_Reset, LOW);
  pinMode(pinBT_Conf, OUTPUT);
  pinMode(pinBT_Reset, OUTPUT);
  delay(3000);
  pinMode(pinBT_Reset, INPUT);

  // begin BT module Serial comms and wait until available
  Serial1.begin(baudBT, SERIAL_8N1, pinBluetoothRX, pinBluetoothTX);
  delay(1000);

#if stateDebug
  Serial.println(F("OpenHaldex T4 Screen Initialised..."));
#endif

  tickEEP.start();           // begin ticker for the EEP
  tickbtSendStatus.start();  // begin ticker for BT Status
}

void loop() {
  tickEEP.update();           // refresh the EEP ticker
  tickbtSendStatus.update();  // refresh the BT Status ticker

  // split the menu into loop control for dynamic refresh (for live data)
  LCDML.loop_control();
  if (LCDML.TIMER_ms(g_timer_1000ms, 250)) {
    // only update the menu when a dynamic content function is called
    // This variable is set in the LCDML_display_menu Tab on line 59/60
    if (LCDML.MENU_checkDynRContent() == true) {
      LCDML.MENU_display();
    }
  }
  LCDML.loop_menu();

  // check to see if BT is connected / is getting data
  if (millis() - lastTransmission >= btTimeout) {
    btConnected = false;
  } else {
    btConnected = true;
  }

  if (requestPair) {
    btInit();  // can't do this in menus due to the interrupt, so set a flag and if it's true, run in loop
    requestPair = false;
  }

  while (Serial1.available() && !ignoreSerialBT) {  // if anything comes in Bluetooth Serial
#if stateDebug
    Serial.println(F("Got serial data..."));
#endif
    btIncoming[10] = { 0 };
    incomingLen = Serial1.readBytesUntil(serialPacketEnd, btIncoming, 10);
#if stateDebug
    Serial.println(incomingLen);
#endif
    if (incomingLen < 9) {
      runOnce = false;
    }
    btReceiveStatus();
  }
}
