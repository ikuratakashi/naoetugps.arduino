#ifndef WIFIGPS_DEBUG
#define WIFIGPS_DEBUG

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "wifigps.Lcd.h"

//Debugモジュール
class AsDebug {
  public :
    //初期化
    //引数1:デバックモードがシリアルの場合にデバック値を出力するシリアル
    //引数2:デバックモードがLCDの場合にデバック値を出力するLCD
    //引数3:デバックモード
    void Init(HardwareSerial *pOutSerial,AsLcd *pLcd,int pDebugMode);
    //出力
    void DebugPrint(String pTxt);
    //デバッグ出力用 LCDオブジェクト
    AsLcd *mLcd;
    //デバッグ出力用 Sirialオブジェクト
    HardwareSerial *mSerial;
    //デバッグ出力モード
    const int DEBUG_MODE_LCD = 1;
    const int DEBUG_MODE_SER = 2;
    int mDebugMode;
    //行番号表示のカウント
    int mRowNo = 0;
    //行数表示のカウント最大値
    int mRowNoMax = 99;
};

#endif
