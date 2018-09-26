#include "wifigps.Debug.h"
#include "wifigps.Lcd.h"

//初期化
//引数1:デバックモードがシリアルの場合にデバック値を出力するシリアル
//引数2:デバックモードがLCDの場合にデバック値を出力するLCD
//引数3:デバックモード
void AsDebug::Init(HardwareSerial *pOutSerial,AsLcd *pLcd,int pDebugMode){

  mDebugMode = pDebugMode;

  //シリアル利用
  if(mDebugMode == DEBUG_MODE_SER){
    mSerial = pOutSerial;
  }
  
  //LCDモジュール利用
  if(mDebugMode == DEBUG_MODE_LCD){
    mLcd = pLcd;
    mLcd->Init();
    mLcd->SetStart();
  }
  
}

//出力
void AsDebug::DebugPrint(String pTxt){

  mRowNo++;
  if(mRowNo > mRowNoMax){
    mRowNo = 1;
  }
  
  //シリアル利用
  if(mDebugMode == DEBUG_MODE_SER){
    char _buf[64];
    String _bufs = "";
    sprintf(_buf,"%02d:",mRowNo);
    _bufs.concat(_buf);
    _bufs.concat(pTxt);
    mSerial->println(_bufs);
  }
  
  //LCDモジュール利用
  if(mDebugMode == DEBUG_MODE_LCD){
    mLcd->DispAutoRowScroll(pTxt,true);
  }
  
}

