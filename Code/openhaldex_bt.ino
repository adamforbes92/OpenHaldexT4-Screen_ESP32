#pragma once
#include "openhaldex.h"

String atCommands[12] = { "AT",
                          "AT+UART=19200,0,0",
                          "AT+NAME=OpenHaldexScreen",
                          "AT+ROLE=1",
                          "AT+CMODE=0",
                          "AT+CLASS=0",
                          "AT+RMAAD",
                          "AT+INQM=0,4,4",
                          "AT+INQ",
                          "AT+BIND=",
                          "AT+LINK=",
                          "AT+RESET" };
char assembledAddr[50];

bool btInit() {
  char *p = NULL;
  char *header = NULL;
  char *addr1 = NULL;
  char *addr2 = NULL;
  char *addr3 = NULL;

#if stateDebug
  Serial.println(F("Beginning pairing..."));
#endif

  // init pins/LCD etc
  if (1) {
    LCDML.SCREEN_disable();  // disable screensaver while pairing...
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(pairingBT);

    // display actioned command
    if (showScreensaver) {
      LCDML.SCREEN_disable();
    }

    digitalWrite(pinBT_Reset, LOW);
    digitalWrite(pinBT_Conf, HIGH);

    pinMode(pinBT_Reset, OUTPUT);
    pinMode(pinBT_Conf, OUTPUT);

    delay(btTimeout);
    pinMode(pinBT_Reset, INPUT);

    Serial.begin(serialBaud);  // begin Serial talkback
    Serial1.begin(baudAT, SERIAL_8N1, pinBluetoothRX, pinBluetoothTX);

    delay(btTimeout);
  }

  // loop through each AT command (most will return OK).  Special cases are AT+LINK & AT+BIND (9 & 10 in array)
  for (int i = 0; i < 12; i++) {
    delay(btDelay);
    at_buf[0] = '\0';

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(atCommands[i]);

    // write command to Serial1 (BT Module)
    if (i == 8 || i == 9 || i == 10) {
      // special case (once INQ), capture all addresses & parse until OpenHaldex found
      if (i == 8) {
#if stateDebug
        Serial.println(atCommands[i]);
#endif
        Serial1.setTimeout(btTimeout * 2.6);
        Serial1.println(atCommands[i]);
        int readByteCount = 0;
        while (!p) {
          readByteCount = Serial1.readBytesUntil('OK\n', at_buf, arraySize(at_buf));
#if stateDebug
          Serial.println(at_buf);
          Serial.println(readByteCount);
#endif
          if (readByteCount > 1) {
            p = strstr(at_buf, OpenHaldexT4);
          }

          at_buf[readByteCount] = '\0';
        }

        // if found 'OpenHaldex T4' in returned address, break down for address numbers (AT returns :, wants it fed back as ",")
        if (p) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Found: ");
          lcd.setCursor(0, 1);
          lcd.print(OpenHaldexT4);
          while (Serial1.available()) Serial1.read();  // Clear out what remains.

#if stateDebug
          Serial.print("Found: ");
          Serial.print(OpenHaldexT4);
#endif
          addr1 = strtok(at_buf, ":,\r");
          addr1 = strtok(NULL, ":,\r");  // Parses second CSV from SIM808 response (GPS fix value)
          addr2 = strtok(NULL, ":,\r");
          addr3 = strtok(NULL, ":,\r");

#if stateDebug
          Serial.print(addr1);
          Serial.print(addr2);
          Serial.print(addr3);
#endif

          btConnected = true;
        }

        if (!btConnected) {
#if stateDebug
          Serial.print("Could not find ");
          Serial.print(OpenHaldexT4);
          Serial.print("");
#endif
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Could not find");
          lcd.setCursor(0, 1);
          lcd.print(OpenHaldexT4);
          break;
        }
      }

      if (i == 9 && btConnected) {
        sprintf(assembledAddr, "AT+BIND=%s,%s,%s", addr1, addr2, addr3);
#if stateDebug
        Serial.println(assembledAddr);
#endif
        Serial1.println(assembledAddr);
      }

      if (i == 10 && btConnected) {
        sprintf(assembledAddr, "AT+LINK=%s,%s,%s", addr1, addr2, addr3);
#if stateDebug
        Serial.println(assembledAddr);
#endif
        Serial1.println(assembledAddr);
      }
    } else {
#if stateDebug
      Serial.println(atCommands[i]);
#endif
      Serial1.println(atCommands[i]);
      while (!Serial1.available()) {}
      Serial1.readBytesUntil('OK\n', at_buf, arraySize(at_buf));
      // print response to BT Module to screen
      lcd.setCursor(0, 1);
      lcd.print(at_buf);
#if stateDebug
      Serial.println(at_buf);
#endif
    }
  }

  if (!btConnected) {
    // display actioned command
    at_buf[0] = '\0';
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(atCommands[11]);

    // write command to Serial1 (BT Module)
    Serial1.println(atCommands[11]);
    while (!Serial1.available()) {}
    Serial1.readBytesUntil('OK\n', at_buf, arraySize(at_buf));

    // print response to BT Module to screen
    lcd.setCursor(0, 1);
    lcd.print(at_buf);
  }

  pinMode(pinBT_Reset, INPUT);
  digitalWrite(pinBT_Conf, LOW);
  pinMode(pinBT_Conf, OUTPUT);
  delay(btTimeout);

#if stateDebug
  Serial.println(F("Ending pairing..."));
#endif

  Serial1.end();
  Serial1.begin(baudBT, SERIAL_8N1, pinBluetoothRX, pinBluetoothTX);

  if (showScreensaver) {
    LCDML.SCREEN_enable(mFunc_screensaver, 10000);  // set to 10 seconds
  }

  if (btConnected) {
    runOnce = false;
    return true;
  } else {
    return false;
  }
}

void btSendStatus() {
#if stateDebug
  Serial.println(F("Sending status"));
#endif
  if (btConnected) {
    if (incomingLen > 0 && !runOnce) {
      btOutgoing[0] = 4;  // APP_MSG_IS_SCREEN override control
      btOutgoing[1] = serialPacketEnd;
      Serial1.write(btOutgoing, 2);
      runOnce = true;
    }

    btOutgoing[0] = 0;  // APP_MSG_MODE
    btOutgoing[1] = state.mode;
    btOutgoing[2] = pedValue;
    btOutgoing[3] = serialPacketEnd;
    Serial1.write(btOutgoing, 4);
  }
}

void btReceiveStatus() {
  /*
    incoming packet details (for knowing what each byte is...)
    packet.data[0] = APP_MSG_STATUS;
    packet.data[1] = 0;  // was haldexStatus
    packet.data[2] = haldexEngagement;
    packet.data[3] = lockTarget;
    packet.data[4] = vehicleSpeed;
    packet.data[5] = state.mode_override;
    packet.data[6] = state.mode;
    packet.data[7] = pedValue;
    packet.data[8] = screenSoftwareVersion;
    packet.data[9] = SERIAL_PACKET_END;
    packet.len = 10;
  */

  lastTransmission = millis();

  if (btConnected) {
#if stateDebug
    Serial.print("APP_MSG_STATUS: ");
    Serial.println(btIncoming[0]);
    Serial.print("State.mode_override: ");
    Serial.println(btIncoming[5]);
    Serial.print("State.mode: ");
    Serial.println(btIncoming[6]);
    Serial.print("SERIAL_PACKET_END: ");
    Serial.println(btIncoming[9]);
#endif

    haldexState = btIncoming[1];
    haldexEngagement = btIncoming[2];
    lockTarget = btIncoming[3];
    vehicleSpeed = btIncoming[4];
    //state.mode_override = btIncoming[5];
    //state.mode = btIncoming[6];
    pedValue = btIncoming[7];
    boardSoftwareVersion = btIncoming[8];
  }
}