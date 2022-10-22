#include <Arduino.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "NeuralNetwork.cpp"

#define LED 2
Adafruit_MPU6050 mpu;
BluetoothSerial Bt;

void checkMsg(char znak);
String getAcc();
String getGyro();

sensors_event_t a, g, temp;

NeuralNetwork* NN;

float gX, gY, gZ;
float aX, aY, aZ;

bool openAcc = false;
bool openGyro = false;



void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

  NN = new NeuralNetwork();

  Bt.begin("Esp32");
  if(!mpu.begin()){
    Serial.println("Nie udalo sie zainicjalizowac mpu");
    digitalWrite(LED, HIGH);
  }
  Wire.begin();

}



void loop() {
  char c = 0;
  if(Serial.available()){
    Bt.write(Serial.read());
  }
  if(Bt.available()){
    c = Bt.read();
    Serial.write(c);
    Serial.print("\n");

  }
  if(openAcc){
    Bt.print(getAcc().c_str());

  }
  if(openGyro){
    Bt.print(getGyro().c_str());
  }
  
  checkMsg(c);
  delay(300);

  NN->getInputBuffer()[0] = 1;
  float result = NN->predict();

  Serial.printf("%f", result);
}


void checkMsg(char znak){

  switch (znak)
  {
    case '1':
      digitalWrite(LED, HIGH);
      break;

    case '2':
      digitalWrite(LED, LOW);
      break;

    case '3':
      openAcc = !openAcc;
      break;

    case '4':
      openGyro = !openGyro;
      break;

    case '5':
      /* code */
      break;

    default:
      break;
  }
}

String getAcc(){
  mpu.getEvent(&a, &g, &temp);
  aX = a.acceleration.x;
  aY = a.acceleration.y;
  aZ = a.acceleration.z;
  return "Acc:" + String(aX) + "," + String(aY) + "," + String(aZ) + "\n";
}

String getGyro(){
  mpu.getEvent(&a, &g, &temp);
  gX = a.gyro.x;
  gY = a.gyro.y;
  gZ = a.gyro.z;
  return "Gyro:" + String(gX) + "," + String(gY) + "," + String(gZ) + "\n";
}





