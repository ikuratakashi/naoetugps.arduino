#include "wifigps.Timer.h"
#include "wifigps.Gps.h"
#include "wifigps.Debug.h"
#include "wifigps.Lcd.h"
#include <SoftwareSerial.h>

//rx,tx
//SoftwareSerial mySerial1 (10, 9);
//SoftwareSerial mySerial2 (11, 4);


AsGps mGPS;
AsLcd mLCD;
AsDebug mDebug;

TinyGPSPlus mGps_T;

void setup() {

  //初期設定
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  
  //Debugモジュール利用
  //引数1:デバックモードがシリアルの場合にデバック値を出力するシリアル
  //引数2:デバックモードがLCDの場合にデバック値を出力するLCD
  //引数3:デバックモード
  mDebug.Init(&Serial,&mLCD,mDebug.DEBUG_MODE_LCD);
  
  //GPSモジュール利用
  //引数1:入力用シリアル
  //引数2:出力用シリアル
  //引数3:GPS情報の出力間隔(ミリ秒)
  //引数4:デバッグモジュール
  mGPS.Init(&Serial1,&Serial2,5000,&mDebug);
  mGPS.SetStart();

  delay(1000);

}

void loop() {
  
/*
  //GPS位置情報をバッファ
  if(Serial1.available() > 0){
    mGps_T.encode(Serial1.read());
    if (mGps_T.location.isUpdated()){ // GPS情報が正常に更新されていたら
      Serial.print(mGps_T.location.lat());
      Serial.print("/");
      Serial.println(mGps_T.location.lng());
    }
  }
*/   

  //GPS制御
  mGPS.DoRunStep();
  
  //受信情報をシリアルに表示
  if(Serial2.available() > 0){
    while(Serial2.available()){
      Serial.write(Serial2.read());
    }
  }

  //受信情報をシリアルに表示
  if(Serial.available() > 0){
    while(Serial.available()){
      Serial.write(Serial.read());
    }
  }
   
}
