

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
#define TRIGPIN 13
#define ECHOPIN 12

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
float distance_cm;
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

  // begin reading from pins from distance sensor
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  delay(300);
  
}

void loop() 
{
  // distance reading
  distance_cm = distance_sensor();
  
  // temperature readings
  temperature_C = temperature_humidity_sensor.readTemperature();

  // humidity readings
  humidity_perc = temperature_humidity_sensor.readHumidity();
  
  // illuminance readings
  illuminance_lux = illuminance_sensor();

  // move the robot safely
  move_robot(distance_cm);

  // print data serial and send to bluetooth  
  DynamicJsonDocument doc(1024);
  doc["T_C"]    = temperature_C;
  doc["RH_%"]   = humidity_perc;
  doc["I_lux"] = illuminance_lux;
  doc["d_cm"] = distance_cm;
  doc["order"] = order;
  serializeJson(doc, Serial);
  Serial.println();
  serializeJson(doc, BTSerial);
  BTSerial.println();
  
  delay(100);
}

// AUXILIARY FUNCTIONS
float distance_sensor()
{
  long duration;
  // distance reading
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance_cm = duration * 0.018;
  return distance_cm;
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

void move_robot(float distance_cm)
{
  String order;
  // read the order  
  order = read_order();
  
  if (distance_cm > 25)
  {
    if (order == "s") 
    {
      robotStop();
    }
    else if (order == "r") 
    {
      robotForward(200);
    }
    else if (order == "b") 
    {
      robotReverse(200);
    }
    else if (order == "l") 
    {
      robotTurnLeft(150);
    }
    else if (order == "r") 
    {
      robotTurnRight(150);
    }
    else
    {
      robotStop();
    }
  }
  else if (order == "r") 
  {
    robotReverse(200);
  }
  else 
  {
    robotStop();
  }
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
 


  
