void CheckHandSwitches()
  {
    //For Door 1 Switch
    int16_t timer = switch_Delay;
    while(digitalRead(d1_Switch) == HIGH && timer > 0)
      {
        //Check switch is continuously pressed for switch delay ms
        delay(10);
        timer = timer - 10;
      }
    if(timer <= 0)
      {
        //Switch hold delay is complete. Set the switch Status.
        d1_Switch_Status = true;
      }
    else
      {
        d1_Switch_Status = false;
      }

    //For Door 2 Switch
    timer = switch_Delay;
    while(digitalRead(d2_Switch) == HIGH && timer > 0)
      {
        //Check switch is continuously pressed for switch delay ms
        delay(10);
        timer = timer - 10;
      }
    if(timer <= 0)
      {
        //Switch hold delay is complete. Set the switch flag.
        d2_Switch_Status = true;
      }
    else
      {
        d2_Switch_Status = false;
      }
  }

void CheckDoorsStatus()
  {
    if(digitalRead(d1_FB_1) == HIGH && digitalRead(d1_FB_2) == HIGH)
      {
        //Door 1 is opened
        d1_Status = 1;
      }
    else
      {
        //Door 1 is Closed
        d1_Status = 0;
      }

    if(digitalRead(d2_FB_1) == HIGH && digitalRead(d2_FB_2) == HIGH)
      {
        //Door 2 is opened
        d2_Status = 2;
      }
    else
      {
        //Door 2 is Closed
        d2_Status = 0;
      }
  }

void OpenTheDoor(uint8_t door)
  {
    if(door == door_1)
      {
        digitalWrite(d1_Lock, LOW);
        digitalWrite(d1_Red_LED, HIGH);
        digitalWrite(d1_Green_LED, LOW);
        digitalWrite(d1_Buzzer, HIGH);
        delay(1000);
        digitalWrite(d1_Buzzer, LOW);
        d1_Status = 1;        
      }
    if(door == door_2)
      {
        digitalWrite(d2_Lock, LOW);
        digitalWrite(d2_Red_LED, HIGH);
        digitalWrite(d2_Green_LED, LOW);
        digitalWrite(d2_Buzzer, HIGH);
        delay(1000);
        digitalWrite(d2_Buzzer, LOW);
        d2_Status = 2;
      }
    UpdateBoardStatus();
  }

void CloseTheDoor(uint8_t door)
  {
    if(door == door_1)
      {
        digitalWrite(d1_Lock, HIGH);
        digitalWrite(d1_Red_LED, LOW);
        digitalWrite(d1_Green_LED, HIGH);
        digitalWrite(d1_Buzzer, LOW);
        d1_Status = 0;
      }
    if(door == door_2)
      {
        digitalWrite(d2_Lock, HIGH);
        digitalWrite(d2_Red_LED, LOW);
        digitalWrite(d2_Green_LED, HIGH);
        digitalWrite(d2_Buzzer, LOW);
        d2_Status = 0;
      }
    UpdateBoardStatus();
  }

void DoorOpening(uint8_t door)
  {
    int16_t timer = door_Opening_Delay;
    if(door == door_1)
      {
        while(digitalRead(d1_FB_1) == LOW && digitalRead(d1_FB_2) == LOW && timer > 0)
          {
            //Check whether door is actually opened within specified time
            delay(10);
            timer = timer - 10;
          }
        if(timer <= 0)
          {
            //Door is not opened until set time. Close it again.
            CloseTheDoor(door);
          }
      }

    if(door == door_2)
      {
        while(digitalRead(d2_FB_1) == LOW && digitalRead(d2_FB_2) == LOW && timer > 0)
          {
            //Check whether door is actually opened within specified time
            delay(10);
            timer = timer - 10;
          }
        if(timer <= 0)
          {
            //Door is not opened until set time. Close it again.
            CloseTheDoor(door);
          }
      }
  }

void EmergencyOpen()
  {
    //Open both doors
    digitalWrite(d1_Lock, LOW);
    digitalWrite(d2_Lock, LOW);
  }

void BeepBuzzer(uint8_t door)
  {
    if(door == door_1)
      {
        digitalWrite(d1_Buzzer, HIGH);
        delay(250);
        digitalWrite(d1_Buzzer, LOW);
        delay(250);
      }
      
    if(door == door_2)
      {
        digitalWrite(d2_Buzzer, HIGH);
        delay(250);
        digitalWrite(d2_Buzzer, LOW);
        delay(250);
      }
  }

void DoorLogicControl(uint8_t door)
  {
    OpenTheDoor(door);
    DoorOpening(door);
    int16_t timer = door_Open_Delay;
    if(door == door_1)
      {
        if(d1_Status == 1)
          {
            while(timer > 0 && d1_Status == 1)
              {
                CheckDoorsStatus();
                delay(50);
                timer = timer - 50;
              }
            delay(100);
            CheckDoorsStatus();
            if(d1_Status == 1)
              {
                while(d1_Status == 1)
                  {
                    BeepBuzzer(door);
                    CheckDoorsStatus();
                  }
              }
          }
      }
    if(door == door_2)
      {
        if(d2_Status == 2)
          {
            while(timer > 0 && d2_Status == 2)
              {
                CheckDoorsStatus();
                delay(50);
                timer = timer - 50;
              }
            delay(100);
            CheckDoorsStatus();
            if(d2_Status == 2)
              {
                while(d2_Status == 2)
                  {
                    BeepBuzzer(door);
                    CheckDoorsStatus();
                  }
              }
          }
      }
    delay(5000);  //Delay for actual physical movement of the door to close position.
    CloseTheDoor(door);
  }
