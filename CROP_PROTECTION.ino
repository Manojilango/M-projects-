  
#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<dht.h>  

#define dht_dpin 6 


LiquidCrystal lcd(8, 9, 13, 12, 11, 10);
void light_MONITOR();
void SENSOR_MONITOR();



int Mot_L_Plus  = 16;
int Mot_L_Minus = 17;
int Mot_R_Plus  = 18;
int Mot_R_Minus = 19;

int RAIN_SENSOR = 15;

dht DHT;

int IR = 2;
int PUMP1 = 7;

int Fencing=5;
int Buzzer=4;

int val1;
int temp = 0;
int val = 0;
int T,H,M,A,R;
char buff[50];
char ch;



void IR_check()
{
  Serial.println("$Animal Detected..#");
  lcd.clear();
  lcd.print("Animal Detected");
  digitalWrite(Buzzer,HIGH);
  delay(2000);

}


void setup() 
{
    Serial.begin(9600); 
    pinMode(Mot_L_Plus, OUTPUT);   
    pinMode(Mot_L_Minus, OUTPUT); 
    pinMode(Mot_R_Plus, OUTPUT);   
    pinMode(Mot_R_Minus, OUTPUT);

    pinMode(RAIN_SENSOR, INPUT);
    pinMode(Fencing,INPUT);
    pinMode(IR, INPUT_PULLUP);

    digitalWrite(Fencing,LOW);
     
    pinMode(PUMP1, OUTPUT);
    pinMode(Buzzer, OUTPUT);
     Serial.begin(9600);
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("PROTECTION OF");
    lcd.setCursor(0,1);
    lcd.print("CROPS...");
    Serial.println("PROTECTION OF CROPS..");
    delay(2000);
    attachInterrupt(digitalPinToInterrupt(IR), IR_check, FALLING);

}

void loop()                    
{

     HARDWARE();
      
}
char Serial_read(void)
{
      char ch;
      while(Serial.available() == 0);
      ch = Serial.read(); 
      return ch;
}

void HARDWARE(void)
{

while(1)
{
  MOISTURE_MONITOR();
  delay(1000);
  SENSOR_MONITOR();
   delay(1000);
   HUMIDITY_MEASUREMENT();
   delay(1000);
   Fencing_check();
   delay(1000);
//   t.update();
}
}
void Fencing_check()
{
    delay(1000);
  digitalWrite(Buzzer,LOW);

  if(digitalRead(Fencing)==HIGH)
  {
    lcd.clear();
    lcd.print("  Fencing ");
    lcd.setCursor(0,1);
    lcd.print("Disconnected");
    Serial.println("$Fencing Disconnected#");
    digitalWrite(Buzzer,HIGH);
    delay(1000);
    digitalWrite(Buzzer,LOW);
  } 

} 
void HUMIDITY_MEASUREMENT(void)
{
     delay(1000);
  digitalWrite(Buzzer,LOW);

    DHT.read11(dht_dpin);
    Serial.print("Humidity: ");
    Serial.print(DHT.humidity);   // printing Humidity on LCD
    Serial.print(" %");
    Serial.print("  ");
    
    delay(500);

    Serial.print("Temperature:");
    Serial.print(DHT.temperature);   // Printing temperature on LCD
    Serial.print(" C");
    Serial.println("  ");
    

  lcd.clear();
  lcd.print("RH:");
  lcd.print(DHT.humidity);
  lcd.setCursor(0, 1);
  lcd.print("TEMP:");
  lcd.print(DHT.temperature);
  delay(500);
   
 }
void MOISTURE_MONITOR()
{
     delay(1000);
  digitalWrite(Buzzer,LOW);

    lcd.setCursor(0,0);
    val = analogRead(0);    // read the input pin
    M=1023-val;  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Moisture:");
    lcd.print(M); 
    delay(2000);
    if(M<100)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Low Moisture Level");
      lcd.setCursor(0,1);
      lcd.print("Pump On..."); 
      Serial.println("$Low Moisture Level Pump On..#");
      digitalWrite(PUMP1,HIGH); 
      delay(1000);
      
    }
    if(M>300)
    {
      digitalWrite(PUMP1,LOW); 
      
      
    }
    
}

void SENSOR_MONITOR()
{
     delay(1000);
  digitalWrite(Buzzer,LOW);

   if(digitalRead(RAIN_SENSOR)==LOW)
   {      
          R=0;
           lcd.clear();
           lcd.print("RAIN DETECTED");
           Serial.println("$Rain Detected...#");
           delay(1000);
           ARM_CLOSE();
          
           while(digitalRead(RAIN_SENSOR)==LOW);
            ARM_OPEN();
         
   }
   else
   {
    R=1;
    }

}


 void ARM_OPEN()
 {
     delay(1000);
  digitalWrite(Buzzer,LOW);

    lcd.clear();
    lcd.print("OPENING PANELS..");
    
    digitalWrite(Mot_R_Plus, HIGH);   
    digitalWrite(Mot_R_Minus, LOW); 
    delay(1200);
    ARM_STOP(); 
    delay(1000);
    digitalWrite(Mot_L_Plus, HIGH);   
    digitalWrite(Mot_L_Minus, LOW); 
    delay(1200);
    ARM_STOP();
    delay(1000);
 }
 
 void ARM_STOP()
 {
     delay(1000);
  digitalWrite(Buzzer,LOW);

   digitalWrite(Mot_L_Plus, LOW);   
   digitalWrite(Mot_L_Minus, LOW); 
   digitalWrite(Mot_R_Plus, LOW);   
   digitalWrite(Mot_R_Minus, LOW); 
 }
  void ARM_CLOSE()
 {
     delay(1000);
  digitalWrite(Buzzer,LOW);

  lcd.clear();
    lcd.print("CLOSING PANELS..");
   digitalWrite(Mot_L_Plus, LOW);   
   digitalWrite(Mot_L_Minus, HIGH); 
   delay(1200);
   ARM_STOP();
     delay(1000);
   digitalWrite(Mot_R_Plus, LOW);   
   digitalWrite(Mot_R_Minus, HIGH); 
   delay(1200);
   ARM_STOP();
     delay(1000);
 }
