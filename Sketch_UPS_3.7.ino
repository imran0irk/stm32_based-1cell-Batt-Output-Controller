#include <SPI.h>
#include <Wire.h>

//Define analog input
#define ANALOG_IN_PIN PA0
#define DC_IN_PIN PA3
#define CTRL_PIN PA11 //Battery output control
//LOW & HIGH
#define LED1 PB8  //LOW BATTERY
#define LED2 PB9  //UPS ON
//%%%%
#define LED3 PB12  //25%
#define LED4 PB13  //50%
#define LED5 PB14  //75%
#define LED6 PB15  //90-100%

//Floats Input and output
float adc_voltage = 0.0;
float in_voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int adc_value = 0;
float ref_voltage = 3.3;
bool hold = false;

//#################################### LOW Voltage cut off
float low_voltage = 2.9;
//####################################

void setup() {
  pinMode(CTRL_PIN, OUTPUT);
  pinMode(ANALOG_IN_PIN, INPUT);
  pinMode(DC_IN_PIN, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
}

void loop() {
  adc_value = analogRead(ANALOG_IN_PIN);
  adc_voltage  = ((adc_value*ref_voltage)/4096); 
  in_voltage = (adc_voltage/(R2/(R1+R2)));

  if (digitalRead(DC_IN_PIN) == HIGH){
    hold = false;
    digitalWrite(CTRL_PIN, LOW);
    digitalWrite(LED2, LOW);  //ups_on
    digitalWrite(LED1, LOW);  //LOW
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
  }
  else if (digitalRead(DC_IN_PIN) == LOW){
    if ((in_voltage <= low_voltage)&&(hold == false)){
      hold = true;
      digitalWrite(CTRL_PIN, LOW);
      digitalWrite(LED2, LOW);  //ups_on
      digitalWrite(LED1, HIGH); //Battery LOW
    }  
    else if ((in_voltage > low_voltage)&&(hold == false)){
      digitalWrite(CTRL_PIN, HIGH);
      digitalWrite(LED2, HIGH); //ups_on /load on
      digitalWrite(LED1, LOW);  //LOW . NORMAL
    }
    else {
      digitalWrite(CTRL_PIN, LOW);
      digitalWrite(LED2, LOW); //ups_on /load on
      digitalWrite(LED1, HIGH);  //LOW
    }
    if (in_voltage > 4.1 ){
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, HIGH);
    }
    else if (in_voltage <= 4.1 && in_voltage > 3.7){
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, LOW);
    }
    else if (in_voltage <= 3.7 && in_voltage > 3.5){
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
    }
    else if (in_voltage <= 3.5 && in_voltage >= 3.2){
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
    }
    else if (in_voltage < 3.2){
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
    }
  }
}