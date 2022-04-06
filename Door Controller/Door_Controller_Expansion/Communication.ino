void UpdateBoardStatus()
  {
    bard_Status = board_ID + d1_Status + d2_Status;
    Serial.print(bard_Status);
  }

void GetDoorArrayStatus()
  {
    uint8_t command = 120;
    Serial.print(command);
    while(Serial.available() == 0);
    door_Array_Status = (uint8_t)Serial.read();
  }

void GetDelays()
  {
    uint8_t command = 121;
    Serial.print(command);
    String inputString = "";
    while(Serial.available() == 0);
    while(Serial.available()) 
      {
        uint8_t data = (uint8_t)Serial.read();
        //Serial.print(data);
        inputString = inputString + data;
        if(data == '#')
          break;                
      }
    delays[0] = inputString.substring(inputString.lastIndexOf('@')+1, inputString.lastIndexOf('$')).toInt();
    delays[1] = inputString.substring(inputString.lastIndexOf('$')+1, inputString.lastIndexOf('%')).toInt();
    delays[2] = inputString.substring(inputString.lastIndexOf('%')+1, inputString.lastIndexOf('#')).toInt();
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

    Serial.print(boardStatus);
  }

void serialEvent() 
  {
    while (Serial.available()) 
      {
        uint8_t data = (uint8_t)Serial.read();
        if(data == 101)
          {
            emergency_Status = 1;
            EmergencyOpen();
          }
        else if(data == 102)
          {
            String inputString = "";
            while(Serial.available() == 0);
            while(Serial.available()) 
              {
                data = (uint8_t)Serial.read();
                //Serial.print(data);
                inputString = inputString + data;
                if(data == '#')
                  break;                
              }
            delays[0] = inputString.substring(inputString.lastIndexOf('@')+1, inputString.lastIndexOf('$')).toInt();
            delays[1] = inputString.substring(inputString.lastIndexOf('$')+1, inputString.lastIndexOf('%')).toInt();
            delays[2] = inputString.substring(inputString.lastIndexOf('%')+1, inputString.lastIndexOf('#')).toInt();
            switch_Delay = delays[0] * 1000;
            door_Opening_Delay = delays[1] * 1000;
            door_Open_Delay = delays[2] * 1000;
          }
        else if(data == board_ID)
          {
            SendBoardStatus();
          }
        else
          door_Array_Status = data;
      }
  }
