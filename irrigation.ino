#include <dht.h>

#define a_dht A0
#define a_soil A1
#define relay 2
#define d_r 13
#define d_o 12
#define d_g 11

dht DHT;
int humidity;
int temp;
int moisture;
int motor = 0;
int ssinit = 2;
int soil = 0;

void setup() {
  //setup board
  Serial.begin(9600);
  delay(1000);

  //test boot
  Serial.print("Irrigation System\n");
  delay(2000);

  //led setup
  pinMode(d_r,OUTPUT);
  pinMode(d_o,OUTPUT);
  pinMode(d_g,OUTPUT);

  //relay setup
  pinMode(relay, OUTPUT);
}

//default loop
void loop() {
  if(moisture < 5) {
    soil = 0;
    loop2();
    pump0();
    if(ssinit == 2) {
      ssinit = 1;
      Serial.println("Initializing soil moisture sensor");
    } else if(ssinit == 1){
      ssinit = 0;
      Serial.println("Soil not found or super dry soil found");
    }
    r1();o1();g1();
    delay(1000);
    r0();o0();g0();
    delay(1000);
  } else {
    soil = 1;
    ssinit = 1;
    loop1();
    loop2();
    if(motor == 0) {
      if(moisture >= 5 && moisture <= 60) {
        motor = 1;
        g1();
        pump1();
      }
    } else {
      if(moisture >= 80) {
        motor = 0;
        pump0();
        g0();
      }
    }
    delay(1000);
  }
}

//dht loop
void loop1() {
  r1();
  DHT.read11(a_dht);
  humidity = DHT.humidity;
  temp = DHT.temperature;
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%\n");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("C\n");
  r0();
}

//soil loop
void loop2() {
  o1();
  moisture = analogRead(a_soil);
  moisture = map(moisture,1023,375,0,100);
  if(soil != 0) {
    Serial.print("Moisture: ");
    Serial.print(moisture);
    Serial.print("%\n"); 
  }
  o0();
}

//water pump
void pump0() {
  if(soil != 0) {
    Serial.println("Pump: off");  
  }
  digitalWrite(relay, HIGH);
}
void pump1() {
  if(soil == 0) {
    pump0();
  }
  Serial.println("Pump: on");
  digitalWrite(relay, LOW);
}

//red led
void r1() {
  digitalWrite(d_r,HIGH);
}
void r0() {
  digitalWrite(d_r,LOW);
}

//orange led
void o1() {
  digitalWrite(d_o,HIGH);
}
void o0() {
  digitalWrite(d_o,LOW);
}

//green led
void g1() {
  digitalWrite(d_g,HIGH);
}
void g0() {
  digitalWrite(d_g,LOW);
}
