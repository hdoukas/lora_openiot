/*  
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.2
 *  Design:            David Gascón
 *  Implementation:    Covadonga Albiñana, Victor Boria, Ruben Martin
 *  
 *  Version 1.3
 *  Implementation: Charalampos Doukas, OpenIoT, CREATE-NET
 *  Description: Sample Arduino code for sampling from a Grove sensor
 *  and performing a broadcast transmition 
 *  This sample code reads from the analog port (A0) and transmits both on demand
 *  based on sensor changes and on every defined interval
 */

#include <Wire.h>

// Cooking API libraries
#include <arduinoUtils.h>

// Include the SX1272 and SPI library:
#include "arduinoLoRa.h"
#include <SPI.h>

//modify this to transmit based on an interval
#define INTERVAL 300000   //trasnmit every X minutes (5 here as en example)

//modify this accordingly
String sensor_ID = "Ax";

//modify to set the sensor value change threshold
//if the new sensor value differs for more then the
//defined threshold, a transmition will occur
int threshold = 600;


int e;

char message1[10];

//modife for analog pin accordingly
const int analogInPin = A0;
int sensorValue = 0;
String stringvalue = "";

unsigned long time;
unsigned long temp;

int lightStateA, lightStateB;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // Print a start message
  Serial.println(F("SX1272 module and Arduino: send packets without ACK"));
  
  powerLoRaOn();

  temp = 0;

  lightStateA = 0;
  lightStateB = 0;
  
}

void loop(void)
{

  //transmit based on sensor changes:
  lightStateA = analogRead(analogInPin);
  if(abs(lightStateA - lightStateB) > threshold) {
    lightStateB = lightStateA;
    //transmit new light status:
    //Serial.println(lightStateA);

    stringvalue = sensor_ID;
    stringvalue += lightStateA;
    Serial.println(stringvalue);
    stringvalue.toCharArray(message1, 10);
    
    // Send message1 and print the result
    e = sx1272.sendPacketTimeout(8, message1);
    Serial.print(F("Packet sent, state "));
    Serial.println(e, DEC);

    //power LoRa OFF:
    //powerLoRaOff();
    
    
  }

  time = millis();
  //check if it's time to transmit:
  if(time - temp > INTERVAL) {
    temp = time;

    //power LoRa ON:
    //powerLoRaOn();


    sensorValue = analogRead(analogInPin);
    Serial.println(sensorValue);
    stringvalue = "E:";
    stringvalue += lightStateA;
    stringvalue.toCharArray(message1, 10);
    
    // Send message1 and print the result
    e = sx1272.sendPacketTimeout(8, message1);
    Serial.print(F("Packet sent, state "));
    Serial.println(e, DEC);

    //power LoRa OFF:
    //powerLoRaOff();
  
  }

  delay(2000);

}

void powerLoRaOn() {

  // Power ON the module
  e = sx1272.ON();
  Serial.print(F("Setting power ON: state "));
  Serial.println(e, DEC);
  
  // Set transmission mode and print the result
  e |= sx1272.setMode(4);
  Serial.print(F("Setting Mode: state "));
  Serial.println(e, DEC);
  
  // Set header
  e |= sx1272.setHeaderON();
  Serial.print(F("Setting Header ON: state "));
  Serial.println(e, DEC);
  
  // Select frequency channel
  e |= sx1272.setChannel(CH_10_868);
  Serial.print(F("Setting Channel: state "));
  Serial.println(e, DEC);
  
  // Set CRC
  e |= sx1272.setCRC_ON();
  Serial.print(F("Setting CRC ON: state "));
  Serial.println(e, DEC);
  
  // Select output power (Max, High or Low)
  e |= sx1272.setPower('H');
  Serial.print(F("Setting Power: state "));
  Serial.println(e, DEC);
  
  // Set the node address and print the result
  e |= sx1272.setNodeAddress(2);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  if (e == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));

}

void powerLoRaOff() {
 sx1272.OFF();
}


