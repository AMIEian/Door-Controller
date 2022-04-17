void serialEvent() 
  {
    while(Serial2.available()) 
      {
        uint8_t data = (uint8_t)Serial2.read();
        //SerialBT.print("Data From Arduino = ");
        //SerialBT.println(data);
        if(data == 'x')
          {
            Serial2.write(door_Array_Status);
            SerialBT.print("Sending Door Array Status = ");
            SerialBT.println(door_Array_Status);
          }
        else if(data == 'y')
          {
            SendDelays();          
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
            Serial2.write(door_Array_Status);
            SerialBT.print("Current Door Array Status = ");
            SerialBT.println(door_Array_Status);
          }
      }
  }

void serialBTEvent() 
  {
    if(SerialBT.available())
      {
        char data = (char)SerialBT.read();
        if(data == 'z')
          {
            //uint8_t count = 0;
            String inputString = "";
            while(SerialBT.available() == 0);
            while(SerialBT.available()) 
              {
                data = (char)SerialBT.read();
                inputString = inputString + data;
                if(data == '#')
                  break;                
              }
            //inputString = inputString + '\n';
            SerialBT.print("Input String = ");
            SerialBT.println(inputString);            
            delays[0] = inputString.substring(inputString.lastIndexOf('@')+1, inputString.lastIndexOf('$')).toInt();
            delays[1] = inputString.substring(inputString.lastIndexOf('$')+1, inputString.lastIndexOf('%')).toInt();
            delays[2] = inputString.substring(inputString.lastIndexOf('%')+1, inputString.lastIndexOf('#')).toInt();
            prefs.putBool("saved", true);
            prefs.putBytes("delays", delays, sizeof(delays));
            SerialBT.print("Delays has been updated...");
            SendDelays();
          }
      }
    while (SerialBT.available()) 
      {
        uint8_t data = (uint8_t)SerialBT.read();
      }
  }
  
/*
void serialBTEvent() 
  {
    if(SerialBT.available())
      {
        uint8_t data = (uint8_t)SerialBT.read();
        if(data == 'z')
          {
            while(SerialBT.available() == 0);
            delays[0] = (uint8_t)SerialBT.read() - 48;
            while(SerialBT.available() == 0);
            delays[1] = (uint8_t)SerialBT.read() - 48;
            while(SerialBT.available() == 0);
            delays[2] = (uint8_t)SerialBT.read() - 48;
            prefs.putBool("saved", true);
            prefs.putBytes("delays", delays, sizeof(delays));
            SerialBT.println("Delays Changed...!");
            SendDelays();    
          }
      }
    while (SerialBT.available()) 
      {
        uint8_t data = (uint8_t)SerialBT.read();
      }
  }
*/

void SendDelays()
  {
    Serial2.print('z');
    Serial2.write(delays[0]);
    Serial2.write(delays[1]);
    Serial2.write(delays[2]);

    SerialBT.println("Sending Delays...");
    SerialBT.print('z');
    SerialBT.print(delays[0]);
    SerialBT.print(delays[1]);
    SerialBT.print(delays[2]);
    SerialBT.println();
  }

void initNode()
  {
    node.begin(1, Serial);
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
          Serial2.write(command);
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
        uint8_t req = board * 10 + 5;
        Serial2.write(req);
        while(Serial2.available() == 0);        
        boardStatus = (uint8_t)Serial2.read();
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
