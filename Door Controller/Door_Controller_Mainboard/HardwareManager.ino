void CheckEmergency()
  {
    if(digitalRead(fireInput) == LOW || digitalRead(emergencyInput) == LOW)
      {
        //Fire or Emergency Input Pressed. Send Emergency Signal.
        uint8_t command = 101;
        Serial2.write(command);
        door_Array_Status = 101;
        for(uint8_t i = 0; i < 8; i++)
          doors_Status[i] = 3;
        if(digitalRead(fireInput) == HIGH)
          {
            //Fire Input Received. Turn on hooter
            digitalWrite(hooter, HIGH);
          }
      }
  }
