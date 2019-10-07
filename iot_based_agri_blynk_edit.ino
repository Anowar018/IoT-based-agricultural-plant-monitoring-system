#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleDHT.h>
#include <DHT.h>

char auth[]= "iDv3o2o9yB-X0TkL-czT2N4-8P_EZh97"; // Your Auth token
char ssid[]= "Jontrotantrik";
char pass[]= "jtl.robotics";

const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

const int fan= 14; //D5

const int DHTPIN = 2; // D4 on nodemcu
//const int ph_sensor=A0;
const int pump1=4;//D2
const int pump2=5;//D1
const int moisture=12;//D6 on nodemcu

SimpleDHT11 dht11;
BlynkTimer timer;
int moisture_value;
#define DHTTYPE DHT11   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

void sendSensor()
{
   
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  //byte phValue=0;
  byte moisture_value=0;
  //byte phValue;
//  ph_level=analogRead(ph_sensor);
  int err = SimpleDHTErrSuccess;
 
  // for debug purposes.
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");
  // DHT11 sampling rate is 1HZ.
//Serial.print((int)phValue); Serial.println(" PH");
  Serial.print((int)moisture_value); Serial.println(" Moisture");
  delay(1000);
  Blynk.virtualWrite(V5, temperature);
  
  Blynk.virtualWrite(V7, humidity);
 // Blynk.virtualWrite(V6, phValue);
   Blynk.virtualWrite(V6, moisture_value);
   

}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(fan, OUTPUT);
  pinMode (pump1,OUTPUT);
  pinMode(pump2,OUTPUT);
  //digitalWrite(pump1,HIGH);
    //digitalWrite(pump2,HIGH);

  // Sending authentication token to blynk. Getting access to your WiFi with SSID and password
  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  
  Blynk.run();
  timer.run();
    ph();
  moisture_control();
}
void moisture_control()
{
  moisture_value=analogRead(moisture);
 
  Serial.print("value= ");
  Serial.print(moisture_value);
  if (moisture_value<=600)
  {
    digitalWrite(pump1,LOW);
   // digitalWrite(pump2,LOW);
   
 //   delay(5000);
  }
  else
  {
    digitalWrite(pump1,HIGH);
//    digitalWrite(pump2,HIGH); 
  }
}
void ph(){
   for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + 21.34;
 Serial.print("sensor = ");
 Serial.println(phValue);
 
 delay(20);
 Blynk.virtualWrite(V0, phValue);
}

  
 
void temp_hum()
{
//  float h = dht.readHumidity();
    float t = dht.readTemperature();

    delay(3000);
  //      Serial.print("Humidity: "); 
      //  Serial.print(h);
        //Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");

         if (t>32)
  {
   digitalWrite(fan,HIGH);
  }
  else{
    digitalWrite(fan,LOW);
  }
}
