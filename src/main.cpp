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
void Predict();

sensors_event_t a, g, temp;

NeuralNetwork* NN;

float gX, gY, gZ;
float aX, aY, aZ;

bool openDataFlow = false;
bool openPredict = false;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

  NN = new NeuralNetwork();

  Bt.begin("Esp32");// D21:SDA   D22:SDL
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
  if(openDataFlow){
    Bt.print((getAcc() + getGyro()).c_str());
  }

  if(openPredict){
    Predict();
  }
  
  checkMsg(c);
  delay(100);


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
      openDataFlow = !openDataFlow;
      break;

    case '4':
      openPredict = !openPredict;
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
  return String(aX) + "," + String(aY) + "," + String(aZ) + ",";
}

String getGyro(){
  mpu.getEvent(&a, &g, &temp);
  gX = a.gyro.x;
  gY = a.gyro.y;
  gZ = a.gyro.z;
  return String(gX) + "," + String(gY) + "," + String(gZ) + ",";
}

void Predict(){
  float number1 = random(100) / 100.0;
  float number2 = random(100) / 100.0;

  NN->getInputBuffer()[0] = number1;
  NN->getInputBuffer()[1] = number2;

  float* result = NN->predict();


  const char *expected = number2 > number1 ? "True" : "False";

  const char *predicted = result[1] > result[0] ? "True" : "False";
 
  Serial.printf("%.2f %.2f - result1 %.2f - result2 %.2f Expected %s, Predicted %s\n", number1, number2, result[0], result[1], expected, predicted);
}







