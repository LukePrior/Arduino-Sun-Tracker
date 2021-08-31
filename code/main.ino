#include <Servo.h>
#include <Wire.h>
#include "DFRobot_INA219.h"

// setup the power meter
DFRobot_INA219_IIC ina219(&Wire, INA219_I2C_ADDRESS4);

// select the input pins for LDR
int lightSensor1 = A0;
int lightSensor2 = A1;
int lightSensor3 = A2;
int lightSensor4 = A3;

// variables to store the values coming from the sensors
int sensorValue1 = 0; 
int sensorValue2 = 0;
int sensorValue3 = 0;
int sensorValue4 = 0;

int horizontalDiff = 0;
int verticalDiff = 0;

// setup the servo motors
Servo servo1;
Servo servo2;
int horizontalAngle = 90;
int verticalAngle = 90;

void setup() {
  Serial.begin(9600); // sets serial port for communication
  servo1.attach(3);
  servo2.attach(5);
  servo1.write(horizontalAngle);
  servo2.write(verticalAngle);

  // setup power meter
  while(ina219.begin() != true) {
      Serial.println("INA219 begin faild");
      delay(2000);
  }
}

void loop() 
{ 
  // read & store the values from the sensors
  sensorValue1 = analogRead(lightSensor1);
  sensorValue2 = analogRead(lightSensor2);
  sensorValue3 = analogRead(lightSensor3);
  sensorValue4 = analogRead(lightSensor4);
  
  horizontalDiff = (sensorValue1 + sensorValue2) - (sensorValue3 + sensorValue4);
  verticalDiff = (sensorValue1 + sensorValue3) - (sensorValue2 + sensorValue4);

  if (horizontalDiff > 60 && horizontalAngle > 0) {
    horizontalAngle--;
    servo1.write(horizontalAngle);
  } else if (horizontalDiff < -60 && horizontalAngle < 180) {
    horizontalAngle++;
    servo1.write(horizontalAngle);
  }

  if (verticalDiff > 60 && verticalAngle < 180) {
    verticalAngle++;
    servo2.write(verticalAngle);
  } else if (verticalDiff < -60 && verticalAngle > 0) {
    verticalAngle--;
    servo2.write(verticalAngle);
  }

  Serial.println((String)"Horizontal: " + horizontalAngle);
  Serial.println((String)"Vertical: " + verticalAngle);
  
  // display power information
  Serial.print("Voltage: ");
  Serial.print(ina219.getBusVoltage_V(), 2);
  Serial.println("V");
  Serial.print("Current: ");
  Serial.print(ina219.getCurrent_mA(), 1);
  Serial.println("mA");
  Serial.print("Power: ");
  Serial.print(ina219.getPower_mW(), 1);
  Serial.println("mW");
  Serial.println();
  
  delay(10);
}
