void InitBoard()
  {
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    SerialBT.begin("Poly_1"); //Bluetooth device name
    prefs.begin("delays"); // use "calibrations" namespace
    bool saved = prefs.getBool("saved");
    if(saved == true)
      prefs.getBytes("delays", delays, sizeof(delays));
    
    pinMode(adr_1, INPUT);
    pinMode(adr_2, INPUT);
    pinMode(adr_3, INPUT);
    pinMode(adr_4, INPUT);
    pinMode(fireInput, INPUT);
    pinMode(emergencyInput, INPUT);

    pinMode(beatLED, OUTPUT);
    pinMode(comLED, OUTPUT);
    pinMode(modbusLED, OUTPUT);
    pinMode(hooter, OUTPUT);
    pinMode(spareDO, OUTPUT);

    digitalWrite(beatLED, LOW);
    digitalWrite(comLED, LOW);
    digitalWrite(modbusLED, LOW);
    digitalWrite(hooter, LOW);
    digitalWrite(spareDO, LOW);

    GetBoards();
  }

void GetBoards()
  {
    if(digitalRead(adr_1) == HIGH)
      boards_Connected = boards_Connected | (0x01);
    else
      boards_Connected = boards_Connected & ~(0x01);

    if(digitalRead(adr_2) == HIGH)
      boards_Connected = boards_Connected | (0x02);
    else
      boards_Connected = boards_Connected & ~(0x02);

    if(digitalRead(adr_3) == HIGH)
      boards_Connected = boards_Connected | (0x04);
    else
      boards_Connected = boards_Connected & ~(0x04);

    if(digitalRead(adr_4) == HIGH)
      boards_Connected = boards_Connected | (0x08);
    else
      boards_Connected = boards_Connected & ~(0x08);
  }
