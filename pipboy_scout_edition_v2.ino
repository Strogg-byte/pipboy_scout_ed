#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;

#include <SPI.h>
#define CONV_FACTOR 0.00412
#define SAMPLE_TIME 3000

#define in A3
unsigned long start = 0;
int cont = 0;

int led_b = 12;
int led_r = 10;
int led_g = 11;
//int geiger = A3;
int rad = 0;
int muszer = 3;

int soundout = 2;
float voltage                 = 0.0;
int sensorValue               = 0;
int UVIndex                   = 0;
const long interval           = 1000;  
const int UV                  = A0; 
const int co                  = A2;
  
       int LPG_detected;  
String msg = "";

int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup() {
  Serial.begin(9600);
  pinMode(led_b,OUTPUT);
  pinMode(led_r,OUTPUT);
  pinMode(led_g,OUTPUT);
  pinMode(soundout,OUTPUT);
  pinMode(muszer,OUTPUT);

tone(soundout, 500,50);
delay(500);
noTone(soundout);
//######## end beep

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
 
start=millis();
}

void stalker_beep(){
  for(int i=0; i <= 1; i++){ 
tone(soundout, 500,50);
delay(200);
noTone(soundout); 
  }
}



void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(led_r, red_light_value);
  analogWrite(led_g, green_light_value);
  analogWrite(led_b, blue_light_value);
}

void loop() {
// ########### LPG ##############
  int gas_2 = analogRead(co);
// ##############################
  
//##################### UV
  sensorValue = analogRead(UV); 
 float voltage = (sensorValue * (4.4 / 1023.0))*100;
 //float Voltage = sensorValue/1024*5.0;
 float UV_index = voltage *.1;

//################# RAD ##########x
  int data = map(analogRead(A1), 0, 1023, 0, 1000);
  //Serial.println(data);
  if (data > 200){
    cont++;
  }
  
if (millis() - start > SAMPLE_TIME ){
    //cont=cont*5;
   // cont=250; //testing value
    float sievert = cont*CONV_FACTOR;
//Serial.println("cont:" +String(cont)+ "data: " +String(data)+ "Rad:" + String(cont) + " / " + String(sievert));
if(cont >=200){
  stalker_beep();
  RGB_color(250,0,0);
} else {
  RGB_color(0,0,0);
}
    
 analogWrite(muszer,cont/15);
 Serial.println("Rad: " +String(cont) + " cpm " + String(sievert) + " uSv/h");
   cont = 0;
   start = millis(); 

//######################## kiiratások 
Serial.println("Temp: " +String(bme.readTemperature()) + " *C");
 Serial.println("Humidity: " +String(bme.readHumidity()) + " %");
 Serial.println("Pressure: " +String(bme.readPressure() / 100.0F) + " hPa");
 Serial.println("Approx. Alt: " +String(bme.readAltitude(SEALEVELPRESSURE_HPA)) + " meter");

if(UV_index >= 8){
  stalker_beep(); RGB_color(255, 0, 0);  
  } else { 
          RGB_color(0, 0, 0); // Red   
          }
 Serial.println("UV index: " +String(UV_index));
 
 Serial.println("LPG/CO/CH4/H2/CH3OH/Sm: " + String(gas_2));
         
           if(gas_2 >= 300){
                stalker_beep();
                RGB_color(255, 0, 0); // Red
                } else { 
          RGB_color(0, 0, 0); // Red   

Serial.println("------------");
              } 

//#############################################   
  }
             

 
//delay(1000);
//##########################################x

}
