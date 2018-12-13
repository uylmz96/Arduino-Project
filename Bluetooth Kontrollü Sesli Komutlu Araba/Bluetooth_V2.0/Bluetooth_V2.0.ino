#include <Servo.h>
String readString = "";


//Bluetooth.
const int Tx=6;
const int Rx=5;
//Motor Sürücü.
const int in1=7;
const int in2=8;
const int enA=3;
//Işıklar.
const int light_f=11;//Ön 
const int light_b=12;//Arka
//Servo motor.
const int in_servo=10;

int gear=1;   //Vites kontrolü
byte light=0; //Işıkların açık kapalı durumu

Servo Sg90;
void setup() {
  Serial.begin(9600); //Bluetooth modülümüz ile 9600 bandında haberleşeceğiz
  Sg90.attach(10);
  pinMode(light_f, OUTPUT); 
  pinMode(light_b, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT); 

}
 
void loop() {
 while (Serial.available()) { // gelen komutu alıyoruz
 char c = (char)Serial.read();
 readString += c;
 }
 
 if (readString.length() > 0) { // komut var ise 
     if (readString == "İleri" or readString == "ileri") { // gelen komut kontrolüne göre işlem yaptırıyoruz
      if(gear==1) { Engine(50,1); }
      if(gear==2) { Engine(80,1); }
      if(gear==3) { Engine(125,1); }
      if(gear==4) { Engine(190,1); }
      if(gear==5) { Engine(255,1); }
      Sg90.write(90);    
      Lights(1); //Ön ışıkları aç
     } 
     else if (readString == "Geri" or readString == "geri") {
      Engine(150,0);
      Sg90.write(90);
      Lights(0);
     } 
     else if (readString == "Sol" or readString == "sol") {
       Sg90.write(120);
     } 
     else if (readString == "Sağ" or readString == "Sağ") {
       Sg90.write(60);
     } 
     else if (readString == "Yükselt" or readString == "yükselt") {
       if(gear<5)
       {
         gear=gear+1;
       }
     } 
     else if (readString == "Düşür" or readString == "düşür") {
     if(gear>1)
      {
        gear=gear-1;
      } 
     } 
     else if (readString == "Işık" or readString == "ışık") {
      if(light==1)
      {
        light=0;
        digitalWrite(light_f,LOW);
        digitalWrite(light_b,LOW);
      }
      else
      {
        light=1;
        digitalWrite(light_f,HIGH);
        digitalWrite(light_b,HIGH);
      }
     } 
     else if (readString == "Dur" or readString == "dur") {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(light_f,LOW);
      digitalWrite(light_b,LOW);
      Sg90.write(90);
     }
     readString = ""; //gelen mesajı sıfırlıyoruz
  }
 
 delay(100);
}

void Lights(int val)
{
  /*
    val=1 ön ışıkları için
    val=0 arka ışıkları için
  */
  if(light==1 and val==1) 
  {
    digitalWrite(light_b,LOW); 
    digitalWrite(light_f,HIGH);
  }
  else if (light==1 and val==0)
  {
    digitalWrite(light_b,HIGH); 
    digitalWrite(light_f,LOW);    
  }
  else
  {
    digitalWrite(light_b,LOW); 
    digitalWrite(light_f,LOW);    
  }
}
void Engine(int val,int x)
{
  /*
    x=1 ileri
    x=0 geri

    val motorun hızı için
  */
  if(x==1)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA,val);     
  }else{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA,val);
  }     
}
