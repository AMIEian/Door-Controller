void InitBoard()
  {
    Serial.begin(9600);
    
    pinMode(adr_1, INPUT);
    pinMode(adr_2, INPUT);
    pinMode(adr_3, INPUT);
    pinMode(adr_4, INPUT);
    
    pinMode(d1_FB_1, INPUT);
    pinMode(d1_FB_2, INPUT);
    pinMode(d1_Switch, INPUT);
    pinMode(d1_Lock, OUTPUT);
    pinMode(d1_Red_LED, OUTPUT);
    pinMode(d1_Green_LED, OUTPUT);
    pinMode(d1_Buzzer, OUTPUT);

    pinMode(d2_FB_1, INPUT);
    pinMode(d2_FB_2, INPUT);
    pinMode(d2_Switch, INPUT);
    pinMode(d2_Lock, OUTPUT);
    pinMode(d2_Red_LED, OUTPUT);
    pinMode(d2_Green_LED, OUTPUT);
    pinMode(d2_Buzzer, OUTPUT);

    digitalWrite(d1_Lock, HIGH);
    digitalWrite(d1_Red_LED, LOW);
    digitalWrite(d1_Green_LED, LOW);
    digitalWrite(d1_Buzzer, LOW);

    digitalWrite(d2_Lock, HIGH);
    digitalWrite(d2_Red_LED, LOW);
    digitalWrite(d2_Green_LED, LOW);
    digitalWrite(d2_Buzzer, LOW);

    GetBoardID();
    GetDelays();
  }

void GetBoardID()
  {
    if(digitalRead(adr_1) == HIGH)
      board_ID = board_ID | (0x01);
    else
      board_ID = board_ID & ~(0x01);

    if(digitalRead(adr_2) == HIGH)
      board_ID = board_ID | (0x02);
    else
      board_ID = board_ID & ~(0x02);

    if(digitalRead(adr_3) == HIGH)
      board_ID = board_ID | (0x04);
    else
      board_ID = board_ID & ~(0x04);

    if(digitalRead(adr_4) == HIGH)
      board_ID = board_ID | (0x08);
    else
      board_ID = board_ID & ~(0x08);

    board_ID = board_ID * 10;
  }
