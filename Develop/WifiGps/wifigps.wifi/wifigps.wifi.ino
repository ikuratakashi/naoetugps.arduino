#include "wifigps.wifi.h"
#include <Metro.h>

AsWifi mWifi;

void setup() {
  
  //初期化
  mWifi.Init(&Serial,&Serial);

  //Wifi開始
  mWifi.SetStart();

}

void loop() {

  //wifi処理実行
  mWifi.DoRunStep();

}
