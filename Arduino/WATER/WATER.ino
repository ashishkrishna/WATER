#include <DHT.h>
#include <Adafruit_MPL115A2.h>
#include <Wire.h>
#include <avr/power.h>
#include <avr/sleep.h>
#define DHTPIN 4
#define DHTTYPE DHT11
Adafruit_MPL115A2 pressure;
double baropressure, lightread, alt;
float temp, humidity;
const double baseline = 1016.5;
double P;
DHT dht(DHTPIN, DHTTYPE);
int Prpin = A0;
int valvePin = 3;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600); 
  Serial.print("DHT11 Status - \t");
  dht.begin();
  pressure.begin();
  pinMode(valvePin, OUTPUT);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  humidity = dht.readHumidity();
  delay(400);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  lightread = analogRead(Prpin);
  delay(400);
  Serial.print(" Light resistor value: ");
  Serial.println(lightread);
  baropressure = pressure.getPressure();
  delay(400);
  temp = pressure.getTemperature();
  delay(400);
  Serial.print(" Temperature: ");
  Serial.println(temp);
  baropressure = baropressure * 10;
  delay(100);
  Serial.print(" Pressure: ");
  Serial.println(baropressure);
  valveControl(baropressure, lightread, temp, humidity);
  delay(400);
  if(isnan(humidity) || isnan(temp) || isnan(baropressure) || isnan(lightread)) {
    return;
  }
  
}

void valveControl(double baropressure, double lightread, float temp, float humidity) {
    float satisfactoryval = 450.565;
    float weightvalue = 0.25 * (float) baropressure + 0.25 * (float) lightread + 0.25 * temp + 0.25 * (float) humidity;
    if(weightvalue-satisfactoryval < -25) {
      digitalWrite(valvePin, HIGH);
      sleep();
      delay(5000);
      wakeup();
      digitalWrite(valvePin, LOW);
      Serial.println("Valve opened/closed for 5000 milliseconds");
      return;
    }

    else if (weightvalue-satisfactoryval > 25) {
      digitalWrite(valvePin, HIGH);
      sleep();
      delay(10000);
      wakeup();
      digitalWrite(valvePin, LOW);
      Serial.println("Valve opened/closed for 10000 milliseconds");
      return;
    }
    else {
      digitalWrite(valvePin, HIGH);
      sleep();
      delay(2500);
      wakeup();
      digitalWrite(valvePin, LOW);
      Serial.println("Valve opened/closed for 2500 milliseconds");
      return; 
    }
}

void sleep() {
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  power_adc_disable();
  power_spi_disable();
}

void wakeup() {
  sleep_disable();
  power_all_enable();
}
    

    
    
    









