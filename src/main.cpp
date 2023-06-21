#include <Arduino.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "NeuralNetwork.cpp"

#define LED 2
#define SampleSize 3
Adafruit_MPU6050 mpu;
BluetoothSerial Bt;
float* accSet;
float* gyroSet;
int licznikDanych = 0;
unsigned int delayTime = 100;

void checkMsg(char znak);
String getAccGyro();
void predictOutput();
String getActivityType(float* result);
void loadBufor(int sizeSample, boolean gyroscope);

sensors_event_t a, g, temp;

NeuralNetwork* NN;

float gX, gY, gZ;
float aX, aY, aZ;

bool openDataFlow = false;
bool openPredict = false;

void setup() {
  accSet = new float[9];
  gyroSet = new float[9];
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
  }

  checkMsg(c);

  String dataAccGyro = getAccGyro();
  if(openDataFlow){
    Bt.print(dataAccGyro.c_str());
    digitalWrite(LED, !digitalRead(LED));
  }

  unsigned long timeOfPrediction = 0;
  if(openPredict && licznikDanych == SampleSize){
    unsigned long start = millis();
      predictOutput();
      digitalWrite(LED, !digitalRead(LED));
    unsigned long end = millis();
    timeOfPrediction = end-start;
  }

  delay(delayTime-timeOfPrediction);
}

void checkMsg(char znak){

  switch (znak)
  {
    case '3':
      openDataFlow = !openDataFlow;
      delayTime = 100;
      break;

    case '4':
      openPredict = !openPredict;
      delayTime = 300;
      break;
    
    case '5':
      openDataFlow = false;
      break;

    case '6':
      openPredict = false;
      break;

    default:
      break;
  }
}

String getAccGyro(){
  if(licznikDanych==SampleSize){
    licznikDanych=0;
  }
  mpu.getEvent(&a, &g, &temp);
  aX = a.acceleration.x;
  aY = a.acceleration.y;
  aZ = a.acceleration.z;
  gX = g.gyro.x;
  gY = g.gyro.y;
  gZ = g.gyro.z;
  accSet[licznikDanych*3] = aX;
  accSet[licznikDanych*3+1] = aY;
  accSet[licznikDanych*3+2] = aZ;
  gyroSet[licznikDanych*3] = gX;
  gyroSet[licznikDanych*3+1] = gY;
  gyroSet[licznikDanych*3+2] = gZ;
  licznikDanych++;
  return String(aX) + "," + String(aY) + "," + String(aZ) + "," + String(gX) + "," + String(gY) + "," + String(gZ);
}

void predictOutput(){
  for(int i=0; i<SampleSize*3; i++){
    printf("A%d: %.2f", i, accSet[i]);
  }
  printf("\n");
    for(int i=0; i<SampleSize*3; i++){
    printf("A%d: %.2f", i, gyroSet[i]);
  }
  printf("\n");
  loadBufor(SampleSize,true);

  float* result = NN->predict();
  
  Serial.printf("Chód %.2f - Bieg %.2f - Rower %.2f, Spoczynek %.2f \n",result[0], result[1], result[2], result[3]);

  Bt.print(getActivityType(result).c_str());
}


String getActivityType(float* result)
{
  int maks = 0;
  for(int i = 1; i < 4; i++){
    if(result[i] > result[maks]){
      maks = i;
    }
  }

  switch (maks)
  {
  case 0:
    return "Chodzenie";
    break;

  case 1:
     return "Bieganie";
    break;
  
  case 2:
     return "Rower";
    break;
  
  case 3:
     return "Spoczynek";
    break;
  }
  return "";
}

void loadBufor(int sizeSample, boolean gyroscope){
  int licznik = 0;
  for(int i=0;i<sizeSample;++i){
    NN->getInputBuffer()[licznik*3] = accSet[i*3];
    NN->getInputBuffer()[licznik*3+1] = accSet[i*3+1];
    NN->getInputBuffer()[licznik*3+2] = accSet[i*3+2];
    if(gyroscope){
      licznik++;
      NN->getInputBuffer()[licznik*3] = gyroSet[i*3];
      NN->getInputBuffer()[licznik*3+1] = gyroSet[i*3+1];
      NN->getInputBuffer()[licznik*3+2] = gyroSet[i*3+2];
    }
    licznik++;
  }
}






