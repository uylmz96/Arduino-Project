#include <Servo.h>
String readString = "";
String command="-1";


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
  //gelen mesajı sıfırlıyoruz
 }
 if(readString.length()>0)
 {
  readString+=" ";
  command =getValue(readString);  
  delay(15);
  } 
 if (command!="-1") { // komut var ise 
     if (command=="1") { // gelen komut kontrolüne göre işlem yaptırıyoruz
      Serial.println("İleri gidiliyor");
      gearChange();
      Sg90.write(90);    
      Lights(1); //Ön ışıkları aç
     } 
     else if (command=="2") {
      Serial.println("Geri gidiliyor");
      Engine(150,0);
      Sg90.write(90);
      Lights(0);
     } 
     else if (command=="3") {
      Serial.println("Sola dönüş");
       Sg90.write(120);
     } 
     else if (command=="4") {
      Serial.println("Sağa dönüş");
       Sg90.write(60);
     } 
     else if (command=="6") {
       if(gear<5)
       {
         gear=gear+1;
       }
       gearChange();
       Serial.println("Vites "+gear);
     } 
     else if (command=="7") {
     if(gear>1)
      {
        gear=gear-1;
      } 
      gearChange();
       Serial.println("Vites "+gear);
     } 
     else if (command=="5") {
      if(light==1)
      {
        Serial.println("Işıklar kapalı");
        light=0;
        digitalWrite(light_f,LOW);
        digitalWrite(light_b,LOW);
      }
      else
      {
        Serial.println("Işıklar açık");
        light=1;
        digitalWrite(light_f,HIGH);
        digitalWrite(light_b,HIGH);
      }
     } 
     else if (command=="8") {
      Serial.println("Araç durduruldu");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(light_f,LOW);
      digitalWrite(light_b,LOW);
      Sg90.write(90);
     }
     command="-1";
     
  }
  readString = "";
 delay(100);
}

void Lights(int val)
{
  /*  val=1 ön ışıkları için
   *  val=0 arka ışıkları için
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
  /* x=1 ileri    x=0 geri
   * val motorun hızı için
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
void gearChange()
{
  if(gear==1) { Engine(50,1); }
  if(gear==2) { Engine(80,1); }
  if(gear==3) { Engine(125,1); }
  if(gear==4) { Engine(190,1); }
  if(gear==5) { Engine(255,1); }
}
String getValue(String Data)
{
  /*  Gelen serialData dan anlamlı bir dizi yakalamaya çalışıyor.
   *  Eğer yakalanırsa geriye ilgili kod döndürülüyor. 
   */
  String val="";
  for(int  i=0;i<Data.length();i++)
  {
    if(Data[i]!=' ')
    {
      val+=Data[i];      
    }
    else if(Data[i]==' ')
    {
      if(val=="İleri" or val=="ileri"){return "1";}
      else if(val=="Geri" or val=="geri"){return "2";}
      else if(val=="Sol" or val=="sol" or val=="Sola" or val=="sola"){return "3";}
      else if(val=="Sağ" or val=="sağ" or val=="Sağa" or val=="sağa"){return "4";}
      else if(val=="Işıklar" or val=="ışıklar" or val=="Işıkları" or val=="ışıkları"){return "5";}
      else if(val=="Yükselt" or val=="yükselt" or val=="Hızlan" or val=="hızlan"){return "6";}
      else if(val=="Düşür" or val=="düşür" or val=="Yavaşla" or val=="yavaşla"){return "7";}
      else if(val=="Dur" or val=="dur"){return "8";}
      else if(val=="Selam" or val=="selam" or val=="Merhaba" or val=="merhaba" or val=="Selamla" or val=="selamla" or val=="Selamlar" or val=="selamlar") 
      {
        digitalWrite(light_b,HIGH); 
        digitalWrite(light_f,HIGH);
        Serial.println("Merhaba ben Nova");
        delay(1000);
        digitalWrite(light_b,LOW); 
        digitalWrite(light_f,LOW);
        val="";
        return "-1";
        
      }
      else {val="";}
      
    }   
  }
  Serial.println("Yanlış Komut");
  return "-1";
}
