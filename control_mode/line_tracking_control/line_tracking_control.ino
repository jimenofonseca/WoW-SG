

// declare libraries
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <CytronMotorDriver.h>
#include <DHT.h>
#include <DHT_U.h>

// declare constants
#define MAX_ADC_READING 1023
#define ADC_REF_VOLTAGE 5.0
#define REF_RESISTANCE 220
#define LUX_CALC_SCALAR 12518931
#define LUX_CALC_EXPONENT -1.405

// delclare sensor pins
#define TEMPERATURE_HUMIDITY_SENSOR_PIN 2
#define PHOTORESISTOR_PIN A2
#define BLUETOOTH_RECEIVER_PIN 3
#define BLUETOOTH_TRANSMITER_PIN 4
#define IR_RIGHT A1
#define IR_LEFT A0

// declare actuator pins
#define LEFT_WHEEL_M1A 5
#define LEFT_WHEEL_M1B 6
#define RIGHT_WHEEL_M2A 10
#define RIGHT_WHEEL_M2B 11

// declare classes
DHT temperature_humidity_sensor(TEMPERATURE_HUMIDITY_SENSOR_PIN, DHT22);
CytronMD left_wheel_actuator(PWM_PWM, LEFT_WHEEL_M1A, LEFT_WHEEL_M1B);
CytronMD right_wheel_actuator(PWM_PWM, RIGHT_WHEEL_M2A, RIGHT_WHEEL_M2B);
SoftwareSerial BTSerial(BLUETOOTH_RECEIVER_PIN, BLUETOOTH_TRANSMITER_PIN);

// declare global variables
float temperature_C;
float humidity_perc;
int illuminance_lux;
int state_blouetooth;
boolean BTConnect = false;
char inChar;
String order;

void setup() {
  // beign reading from serial
  Serial.begin(9600);

  // beign reading from bluetooth
  BTSerial.begin(9600);

  // begin temperature and humidity sensor
  temperature_humidity_sensor.begin();

  // begin reading from pins for motor
  pinMode(LEFT_WHEEL_M1A, OUTPUT);
  pinMode(LEFT_WHEEL_M1B, OUTPUT);
  pinMode(RIGHT_WHEEL_M2A, OUTPUT);
  pinMode(RIGHT_WHEEL_M2B, OUTPUT);

  // begin reading from pins from 
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  delay(300);
  
}

void loop() 
{
  // temperature readings
  temperature_C = temperature_humidity_sensor.readTemperature();

  // humidity readings
  humidity_perc = temperature_humidity_sensor.readHumidity();
  
  // illuminance readings
  illuminance_lux = illuminance_sensor();

  while (true) 
  {
    if (digitalRead(IR_LEFT) == LOW && digitalRead(IR_RIGHT) == LOW) 
    {
      robotForward(150);
      Serial.println("Forward");
    }
    else if (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == LOW) 
    {
      robotTurnLeft(150);
      Serial.println("Left");
    }
    else if (digitalRead(IR_LEFT) == LOW && digitalRead(IR_RIGHT) == HIGH) 
    {
      robotTurnRight(150);
      Serial.println("Right");
    }
    else if (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) 
    {
      robotStop();
      Serial.println("stop");
      break;
    }
  }

  // print data serial and send to bluetooth  
  DynamicJsonDocument doc(1024);
  doc["T_C"]    = temperature_C;
  doc["RH_%"]   = humidity_perc;
  doc["I_lux"] = illuminance_lux;
  doc["order"] = order;
  serializeJson(doc, Serial);
  Serial.println();
  serializeJson(doc, BTSerial);
  BTSerial.println();
  
  delay(100);
}

int illuminance_sensor()
{
  int   ldrRawData;
  float resistorVoltage; 
  float ldrVoltage;
  float ldrResistance;
  float ldrLux;
  ldrRawData = analogRead(PHOTORESISTOR_PIN);
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  illuminance_lux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);
  return illuminance_lux;
}


void robotStop()
{
  left_wheel_actuator.setSpeed(0);
  right_wheel_actuator.setSpeed(0);
}

void robotBreak()
{
  left_wheel_actuator.setSpeed(0);
  right_wheel_actuator.setSpeed(0);
}

void robotForward(int motorSpeed)
{
  left_wheel_actuator.setSpeed(motorSpeed);
  right_wheel_actuator.setSpeed(motorSpeed);
}

void robotReverse(int motorSpeed)
{
  left_wheel_actuator.setSpeed(-motorSpeed);
  right_wheel_actuator.setSpeed(-motorSpeed);
}

void robotTurnRight(int motorSpeed)
{
  left_wheel_actuator.setSpeed(motorSpeed);
  right_wheel_actuator.setSpeed(0);
}

void robotTurnLeft(int motorSpeed)
{
  left_wheel_actuator.setSpeed(0);
  right_wheel_actuator.setSpeed(motorSpeed);
}

String read_order()
{ 
  char inChar;
  char junk;
  String order;
  
  if(BTSerial.available())
  {
    order = "";
    while (BTSerial.available())
    {
      inChar = BTSerial.read();
      order = order + inChar;
    }
    while (BTSerial.available() > 0)  
    { 
      junk = BTSerial.read(); 
    }
  }
  return order;
}
 


  
