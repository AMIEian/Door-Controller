#include "BluetoothSerial.h"
#include <Preferences.h>
#include <ModbusMaster.h>

#define beatLED 15
#define comLED 2
#define modbusLED 4

#define fireInput 33
#define hooter 32
#define spareDO 22
#define emergencyInput 35

#define adr_4 12
#define adr_3 13
#define adr_2 14
#define adr_1 34

#define RXD2 16
#define TXD2 17
#define MAX485_DE 2

long currentmillis = 0;
int last_blink_time = 0;
int blink_interval = 500;
int ledState = LOW;     // the current state of LED

uint8_t boards_Connected = 0;
uint8_t doors_Status[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t door_Array_Status = 0;
int doorData[83];

uint8_t delays[3] = {2, 5, 20};

BluetoothSerial SerialBT;
//Preferances to store data permanently
Preferences prefs;

// instantiate ModbusMaster object
ModbusMaster node;

void preTrans()
{
  digitalWrite(MAX485_DE, 1);
}

void postTrans()
{
  digitalWrite(MAX485_DE, 0);
}

void setup()
  {
    InitBoard();
    initNode();
  }

void loop() 
  {
    digitalWrite(comLED, HIGH);
    serialEvent();
    serialBTEvent();
    digitalWrite(comLED, LOW);
    CheckEmergency();
    digitalWrite(modbusLED, HIGH);
    node_Read();
    node_Write();
    digitalWrite(modbusLED, LOW);
    delay(10);
    currentmillis = millis();   //Get current millis
    if(currentmillis - last_blink_time >= blink_interval)
        {
            last_blink_time = currentmillis;
            // toggle state of LED
            ledState = !ledState;
        
            // control LED arccoding to the toggled state
            digitalWrite(beatLED, ledState);
        }
  }
