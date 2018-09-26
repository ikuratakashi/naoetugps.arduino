#include <TinyGPS++.h>
TinyGPSPlus gps;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(22,OUTPUT);
  digitalWrite(22,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
   if(Serial1.available() > 0){
    gps.encode(Serial1.read());
    if (gps.location.isUpdated()){ // GPS情報が正常に更新されていたら    
      Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
      Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
      Serial.print("ALT=");  Serial.println(gps.altitude.meters());
    }
  }
  
}
