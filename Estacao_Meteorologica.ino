#include <SFE_BMP180.h>
#include <Wire.h>
#include <dht11.h>

dht11 sensor;
SFE_BMP180 pressure;

#define ALTITUDE 760.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters

int lmPin = A0;

void setup()
{
  Serial.begin(115200);
  Serial.println("REBOOT");
  pinMode(lmPin, INPUT);
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    while(1);
  }
}

void loop()
{
  char status;
  double T,P,p0,a;

  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("Pressão absoluta: ");
          Serial.print(P,2);
          Serial.print(" mbars, ");
          Serial.print(P*0.7500617,2);
          Serial.print(" mmHg, ");
          Serial.print(P*0.1-1,2);
          Serial.println(" +-0,1 kPa");
          
          p0 = pressure.sealevel(P,ALTITUDE);
          Serial.print("Pressão relativa (sea-leavel): ");
          Serial.print(p0,2);
          Serial.print(" mbars, ");
          Serial.print(p0*0.7500617,2);
          Serial.print(" mmHg, ");
          Serial.print(p0*0.1-1,2);
          Serial.println("+-0,1 kPa");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  
  int chk = sensor.read(2);
  switch(chk) {
    case DHTLIB_OK:
      break;
    case DHTLIB_ERROR_CHECKSUM:
      break;
    case DHTLIB_ERROR_TIMEOUT:
      break;
    default:
      break;
  }
  Serial.print("Umidade (%): ");
  Serial.println((float)sensor.humidity, 2);
  Serial.print("Temperatura (oC): ");
  Serial.println(tempC());
  delay(5000);
}

float tempC() {
  float raw = analogRead(lmPin);
  float percent = raw/1023.0; // <2>
  float volts = percent*5; // <3>
  return 100.0*volts; // <4>
}
