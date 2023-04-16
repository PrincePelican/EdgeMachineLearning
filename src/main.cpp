#include <Arduino.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "NeuralNetwork.cpp"

#define LED 2
Adafruit_MPU6050 mpu;
BluetoothSerial Bt;
float* accSet;
int licznikDanych = 0;

void checkMsg(char znak);
String getAccGyro();
void predictOutput();
String getActivityType(float* result);

sensors_event_t a, g, temp;

NeuralNetwork* NN;

float gX, gY, gZ;
float aX, aY, aZ;

bool openDataFlow = false;
bool openPredict = false;

void setup() {
  accSet = new float[9];
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

  String dataAccGyro = getAccGyro();
  
  if(openDataFlow){
    Bt.print(dataAccGyro.c_str());
  }

  if(openPredict && licznikDanych == 3){
      predictOutput();
  }


  checkMsg(c);
  delay(400);
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
  if(licznikDanych==3){
    licznikDanych=0;
  }
  mpu.getEvent(&a, &g, &temp);
  aX = a.acceleration.x;
  aY = a.acceleration.y;
  aZ = a.acceleration.z;
  gX = a.gyro.x;
  gY = a.gyro.y;
  gZ = a.gyro.z;
  accSet[licznikDanych*3+0] = a.acceleration.x;
  accSet[licznikDanych*3+1] = a.acceleration.y;
  accSet[licznikDanych*3+2] = a.acceleration.z;
  licznikDanych++;
  return String(aX) + "," + String(aY) + "," + String(aZ) + "," + String(gX) + "," + String(gY) + "," + String(gZ);
}

void predictOutput(){
  for(int i=0; i<9; i++){
    printf("A%d: %.2f", i, accSet[i]);
  }
  printf("\n");
  NN->getInputBuffer()[0] = accSet[0];
  NN->getInputBuffer()[1] = accSet[1];
  NN->getInputBuffer()[2] = accSet[2];
  NN->getInputBuffer()[3] = accSet[3];
  NN->getInputBuffer()[4] = accSet[4];
  NN->getInputBuffer()[5] = accSet[5];
  NN->getInputBuffer()[6] = accSet[6];
  NN->getInputBuffer()[7] = accSet[7];
  NN->getInputBuffer()[8] = accSet[8];;

  float* result = NN->predict();
  
  Serial.printf("result1 %.2f - result2 %.2f - result3 %.2f, result4 %.2f \n",result[0], result[1], result[2], result[3]);

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
    return "Chód";
    break;

  case 1:
     return "Bieg";
    break;
  
  case 2:
     return "Rower";
    break;
  
  case 3:
     return "Spoczynek";
    break;

  }
}






