void serialEvent() 
  {
    while (Serial.available()) 
      {
        uint8_t data = (uint8_t)Serial.read();
        if(data == 120)
          {
            Serial.print(door_Array_Status);
          }
        else if(data == 121)
          {
            Serial.print('@');
            Serial.print(delays[0]);
            Serial.print('$');
            Serial.print(delays[1]);
            Serial.print('%');
            Serial.print(delays[2]);
            Serial.print('#');
          }
        else
          {
            uint8_t board = 0;
            uint8_t door = 0;
            board = data / 10;
            door = data % 10;
            doors_Status[board] = door;
            if(door == 0)
              door_Array_Status = 0;
             else
              door_Array_Status = data;
          }
      }
  }

void serialBTEvent() 
  {
    if(SerialBT.available())
      {
        uint8_t data = (uint8_t)SerialBT.read();
        if(data == 102)
          {
            Serial.print(data);
            uint8_t count = 0;
            String inputString = "";
            while(SerialBT.available() == 0);
            while(SerialBT.available()) 
              {
                data = (uint8_t)SerialBT.read();
                Serial.print(data);
                inputString = inputString + data;
                if(data == '#')
                  break;                
              }
            delays[0] = inputString.substring(inputString.lastIndexOf('@')+1, inputString.lastIndexOf('$')).toInt();
            delays[1] = inputString.substring(inputString.lastIndexOf('$')+1, inputString.lastIndexOf('%')).toInt();
            delays[2] = inputString.substring(inputString.lastIndexOf('%')+1, inputString.lastIndexOf('#')).toInt();
            prefs.putBool("saved", true);
            prefs.putBytes("delays", delays, sizeof(delays));
          }
      }
    while (SerialBT.available()) 
      {
        uint8_t data = (uint8_t)SerialBT.read();
      }
  }

void initNode()
  {
    node.begin(1, Serial2);
    node.preTransmission(preTrans);
    node.postTransmission(postTrans);
  }

//Read Modbus data and send to PLC
void node_Read()
  {
    uint8_t j;
    uint8_t emergency = 0;
    uint8_t fire = 0;
    
    uint8_t result = node.readHoldingRegisters(0, 2);
    
    if (result == node.ku8MBSuccess)
    {
      SerialBT.println("Node Data");
      fire = node.getResponseBuffer(0);
      emergency = node.getResponseBuffer(1);
      if(fire != 0 || emergency != 0)
        {
          uint8_t command = 101;
          Serial.print(command);
          door_Array_Status = 101;
          for(uint8_t i = 0; i < 8; i++)
            doors_Status[i] = 3;
          if(fire != 0)
            {
              //Fire Input Received. Turn on hooter
              digitalWrite(hooter, HIGH);
            }
        }
    }
    SerialBT.println(fire);
    SerialBT.println(emergency);
    SerialBT.println("-------------------------------------");
  }

//Get data from PLC and write to modbus
void node_Write()
  {
    GetDoorsData();
    
    for(int i = 0; i < 83; i++)
      node.setTransmitBuffer(i, doorData[i]);
    
    uint8_t result = node.writeMultipleRegisters(0, 83);
  
    SerialBT.println("Writting Complete");
    SerialBT.println("-------------------------------------");
    SerialBT.println();
    SerialBT.println();
  }

void GetDoorsData()
  {
    uint8_t index = 0;
    uint8_t board = 0;
    uint8_t boardStatus = 0;
    int d1_Status = 0;
    int d1_FB_1 = 0;
    int d1_FB_2 = 0;
    int d1_Switch = 0;
    int d2_Status = 0;
    int d2_FB_1 = 0;
    int d2_FB_2 = 0;
    int d2_Switch = 0;
    
    if(digitalRead(fireInput) == HIGH)
      {
        doorData[index] = 1;
        index++;
      }
    else
      {
        doorData[index] = 0;
        index++;
      }

    if(digitalRead(emergencyInput) == HIGH)
      {
        doorData[index] = 1;
        index++;
      }
    else
      {
        doorData[index] = 0;
        index++;
      }
      
    for(board = 0; board <= boards_Connected; board++)
      {
        Serial.print(board);
        while(Serial.available() == 0);        
        boardStatus = (uint8_t)Serial.read();
        uint8_t res = boardStatus & (0x01);
        if(res != 0)
          d1_Switch = 1;
        else
          d1_Switch = 0;

        res = boardStatus & (0x02);
        if(res != 0)
          d2_Switch = 1;
        else
          d2_Switch = 0;

        res = boardStatus & (0x04);
        if(res != 0)
          d1_Status = 1;
        else
          d1_Status = 0;

        res = boardStatus & (0x04);
        if(res != 0)
          d2_Status = 1;
        else
          d2_Status = 0;

        if(d1_Status != 0)
          {
            d1_FB_1 = 1;
            d1_FB_2 = 1;
          }
        else
          {
            d1_FB_1 = 0;
            d1_FB_2 = 0;
          }

        if(d2_Status != 0)
          {
            d2_FB_1 = 1;
            d2_FB_2 = 1;
          }
        else
          {
            d2_FB_1 = 0;
            d2_FB_2 = 0;
          }

        doorData[index] = d1_Status;
        index++;
        doorData[index] = d1_FB_1;
        index++;
        doorData[index] = d1_FB_2;
        index++;
        doorData[index] = d1_Switch;
        index++;
        doorData[index] = 0;
        index++;

        doorData[index] = d2_Status;
        index++;
        doorData[index] = d2_FB_1;
        index++;
        doorData[index] = d2_FB_2;
        index++;
        doorData[index] = d2_Switch;
        index++;
        doorData[index] = 0;
        index++;
      }
  }
