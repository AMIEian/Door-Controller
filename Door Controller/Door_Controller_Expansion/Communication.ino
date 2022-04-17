void UpdateBoardStatus()
  {
    bard_Status = 0;
    bard_Status = board_ID + d1_Status + d2_Status;
    //Serial.print("Board Status = ");
    Serial.write(bard_Status);
  }

void GetDoorArrayStatus()
  {
    //uint8_t command = 120;
    Serial.print('x');
    while(Serial.available() == 0);
    door_Array_Status = (uint8_t)Serial.read();
    //Serial.print("Door status received = ");
    //Serial.println(door_Array_Status);
  }

void GetDelays()
  {
    Serial.print('y');
    while(Serial.available() == 0);
    char startFlag = (uint8_t)Serial.read();
    while(Serial.available() == 0);
    delays[0] = (uint8_t)Serial.read();
    while(Serial.available() == 0);
    delays[1] = (uint8_t)Serial.read();
    while(Serial.available() == 0);
    delays[2] = (uint8_t)Serial.read();
    switch_Delay = delays[0] * 1000;
    door_Opening_Delay = delays[1] * 1000;
    door_Open_Delay = delays[2] * 1000;
  }

void SendBoardStatus()
  {
    uint8_t boardStatus = 0;
    if(d1_Switch_Status == true)
      boardStatus = boardStatus | (0x01);
    else
      boardStatus = boardStatus & ~(0x01);
            
    if(d2_Switch_Status == true)
      boardStatus = boardStatus | (0x02);
    else
      boardStatus = boardStatus & ~(0x02);

    if(d1_Status == 1)
      boardStatus = boardStatus | (0x04);
    else
      boardStatus = boardStatus & ~(0x04);      
      
    if(d2_Status == 2)
      boardStatus = boardStatus | (0x08);
    else
      boardStatus = boardStatus & ~(0x08);

    Serial.write(boardStatus);
  }

void serialEvent() 
  {
    while(Serial.available()) 
      {
        uint8_t data = (uint8_t)Serial.read();
        if(data == 'e')
          {
            emergency_Status = 1;
            EmergencyOpen();
          }
        else if(data == 'z')
          {
            while(Serial.available() == 0);
            delays[0] = (uint8_t)Serial.read();
            while(Serial.available() == 0);
            delays[1] = (uint8_t)Serial.read();
            while(Serial.available() == 0);
            delays[2] = (uint8_t)Serial.read();
            switch_Delay = delays[0] * 1000;
            door_Opening_Delay = delays[1] * 1000;
            door_Open_Delay = delays[2] * 1000;
          }
        else if(data == (board_ID + 5))
          {
            SendBoardStatus();
          }
        else
          door_Array_Status = data;
      }
  }
