#ifndef WIFIGPS_GPS
#define WIFIGPS_GPS

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "wifigps.Timer.h"
#include "wifigps.Gps.h"
#include "wifigps.Led.h"
#include "wifigps.Debug.h"

//GPSクラス
class AsGps
{
  public :
    //初期化
    //引数1:入力用シリアル
    //引数2:出力用シリアル
    //引数3:GPS情報の出力間隔(ミリ秒)
    //引数4:デバッグモジュール
    void Init(HardwareSerial *pInSerial,HardwareSerial *pOutSerial,int pPutGpsInterval,AsDebug *pDebug);
    //GPSの処理を1ステップ進める
    void DoRunStep();
    //GPS取得を実行
    void SetStart();
    //GPS取得を停止
    void SetStop();
    //バッファへ座標を追加する
    //バッファは0番目から新しい情報
    void AddLocationBuf(TinyGPSPlus pGps);
    //バッファから座標を取得する
    //引数1(参照)：経度
    //引数2(参照)：緯度
    //引数2(参照)：標高
    void GetLocationBuf(String *pLAT,String *pLNG,String *pALT);
    
    //GPSの受信を行うシリアル
    HardwareSerial *mSerialGpsIn;
    //GPSの発信を行うシリアル
    HardwareSerial *mSerialGpsOut;
    
    //GPS情報を出力する間隔(ミリ秒)
    int mPutGpsInterval = 2000;
    //タイマー
    AsTimer mTimer;
    //開始フラグ
    bool IsStart = false;
    
    //GPSバッファ
    //構造体 - 座標
    struct LOCATION{
      String LAT;
      String LNG;
      String ALT;
    };
    LOCATION mLocationBuf[3];
    int mLocationCnt = 0;
    int mLocationMax = 0;

    //GPS
    TinyGPSPlus mGps;
    
    //LED
    AsLed mStatusLed;
    AsLed mSendLed;

    //Debugモジュール
    AsDebug *mDebug;
    
  private :
};

#endif
