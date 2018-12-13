#include <SoftwareSerial.h>
#include <Servo.h>

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


SoftwareSerial mySerial(6,5); // TX,RX
Servo Sg90;
void setup()
{ 
  mySerial.begin(9600);  
  Sg90.attach(10);
  pinMode(light_f, OUTPUT); 
  pinMode(light_b, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT); 
}

void loop()
{
 
  char ch = mySerial.read();
  if(ch=='1') //ileri gitmek için
  {   
    if(gear==1) { Engine(50,1); }
    if(gear==2) { Engine(80,1); }
    if(gear==3) { Engine(125,1); }
    if(gear==4) { Engine(190,1); }
    if(gear==5) { Engine(255,1); }
    Sg90.write(90);    
    Lights(1); //Ön ışıkları aç
  }
  if(ch=='2') //Geri gitmek için
  {
    Engine(150,0);
    Sg90.write(90);
    Lights(0);
  }
  if(ch=='3')//Sola dön
  {
    Sg90.write(120);
  }
  if(ch=='4')//Sağa dön
  {
    Sg90.write(60);
  }
  if(ch=='5')
  {
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
  if(ch=='6')
  {
    if(gear<5)
    {
      gear=gear+1;
    }
  }
  if(ch=='7')
  {
    if(gear>1)
    {
      gear=gear-1;
    }
  }
  if(ch=='8')
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(light_f,LOW);
    digitalWrite(light_b,LOW);
    Sg90.write(90);
  }
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
