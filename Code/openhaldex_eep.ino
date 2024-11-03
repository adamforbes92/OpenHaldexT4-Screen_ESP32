#pragma once
#include "openhaldex.h"

void readEEP() {
#if stateDebug
  Serial.println(F("EEPROM initialising!"));
#endif /* stateDebug */

  // use ESP32's 'Preferences' to remember settings.  Begin by opening the various types.  Use 'false' for read/write.  True just gives read access
  pref.begin("screenSwVer", false);
  pref.begin("lastMode", false);
  pref.begin("pedValue", false);
  pref.begin("isCustom", false);
  pref.begin("isStandalone", false);
  pref.begin("showScreensaver", false);
  pref.begin("showSplash", false);

  // first run comes with EEP valve of 255, so write actual values.  If found/match SW version, read all the values
  if (pref.getUChar("screenSwVer") != screenSoftwareVersion) {
#if stateDebug
    Serial.println(F("First run, set Bluetooth module, write Software Version etc"));
#endif /* stateDebug */
    pref.putUChar("screenSwVer", screenSoftwareVersion);
    pref.putUChar("lastMode", lastMode);
    pref.putFloat("pedValue", pedValue);
    pref.putBool("isCustom", isCustom);
    pref.putBool("isStandalone", isStandalone);
    pref.putBool("showScreensaver", showScreensaver);
    pref.putBool("showSplash", showSplash);
  } else {
    screenSoftwareVersion = pref.getUChar("screenSwVer");
    lastMode = pref.putUChar("lastMode", false);
    state.ped_threshold = pref.putFloat("pedValue", false);
    isCustom = pref.getBool("isCustom", false);
    isStandalone = pref.getBool("isStandalone", false);
    showScreensaver = pref.getBool("showScreensaver", false);
    showSplash = pref.getBool("showSplash", false);
  }
#if stateDebug
  Serial.println(F("EEPROM initialised!"));
#endif /* stateDebug */
}

void writeEEP() {
#if stateDebug
  Serial.println(F("Writing EEPROM..."));
#endif /* stateDebug */

  // only update if the value has changed
  switch (state.mode) {
    case MODE_STOCK:
      lastMode = 0;
      break;
    case MODE_FWD:
      lastMode = 1;
      break;
    case MODE_5050:
      lastMode = 2;
      break;
    case MODE_CUSTOM:
      lastMode = 3;
      break;
  }

  // update EEP only if changes have been made
  pref.getUChar("screenSwVer", screenSoftwareVersion);
  pref.getUChar("lastMode", lastMode);
  pref.putFloat("pedValue", pedValue);
  pref.putBool("isCustom", isCustom);
  pref.putBool("isStandalone", isStandalone);
  pref.putBool("showScreensaver", showScreensaver);
  pref.putBool("showSplash", showSplash);

#if stateDebug
  Serial.println(F("Written EEPROM with data:..."));
  Serial.printf("Software Version: %d.%d%d%\n", (screenSoftwareVersion / 100) % 10, (screenSoftwareVersion / 10) % 10, (screenSoftwareVersion / 1) % 10);
  Serial.printf("Last Mode: %d\n", lastMode);
  Serial.printf("Is Standalone: %d\n", isStandalone);
  Serial.printf("Minimum Pedal: %d%\n", state.ped_threshold);
  Serial.printf("Show Screensaver: %d%\n", showScreensaver);
  Serial.printf("Show Splash: %d%\n", showSplash);
#endif /* stateDebug */
}
