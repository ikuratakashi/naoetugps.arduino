#include <ArduinoJson.h>

#ifndef WIFIGPS_WIFI
#define WIFIGPS_WIFI

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "wifigps.Led.h"
#include <string.h>

//シリアルでの取得情報の最大サイズ
#define AS_WIFI_SERIAL_MAX 64

//Wifiクラス
class AsWifi{
  public:
  
    //初期化
    void Init(HardwareSerial *pSerialIn,HardwareSerial *pSerialOut);
    //Wifi処理を実行
    void SetStart();
    //処理を1ステップ進める
    bool DoRunStep();
    //wifi接続開始
    bool DoWifiCon();
    //httpへPOST
    String DoHttpPost(const char* pHost , int pPort , String pClass, String pData);
    //シリアルへの結果出力
    void WriteSerial(String pMessage);
    //ステータスのステップ実行
    void RunStatusLed();
    //バッファクリア
    void BufferClear(char *pBuff);
    //Wifi接続確認
    bool IsWifiStatus();
    //Wifi接続を確認してLEDの点灯を切り替える
    bool SetWifiStatusToLED();

    //wifi接続情報
    String mSSID = "";
    String mPASS = "";

    //シリアル通信
    HardwareSerial *mSerialIn;
    HardwareSerial *mSerialOut;
    
    //POST先情報
    ////ホスト
    char* mHost;
    
    ////接続ポート
    int mPort = 0;
    ////ポスト先クラス名
    String mPOST_CLASS;

    //デバッグ用
    const int DBG_LED_NG = 12;
    const int DBG_LED_OK = 14;

    //シリアル通信でパースが成功したかどうか
    const bool PARSE_NG = true;
    const bool PARSE_OK = false;

    //戻り値
    const bool RESULT_OK = false;
    const bool RESULT_NG = true;

    //バッファ終端文字列
    const String AS_WIFI_SERIAL_END = ";;";
    int AS_WIFI_SERIAL_END_LEN = 0;

    //コネクション状態
    bool IsConnection = false;
    
    //wifiクライアント
    WiFiClientSecure client;

    //シリアル情報取得関連
    ////情報取得カウント
    int mGetCnt = 0;
    ////取得バッファ
    char mGetBuf[AS_WIFI_SERIAL_MAX + 1];
    ////取得バッファ(チェック用)
    char mGetBufTmp[AS_WIFI_SERIAL_MAX + 1];
    ///シリアル情報の受信中かどうか
    bool IsSerialReading = false;
    ///シリアル情報に溜まったバッファ受信が完了したかどうか
    bool IsSerialCommit = false;
    ///シリアル情報に溜まった全バッファ受信が完了したかどうか
    bool IsSerialCommitAll = false;

    //LED
    AsLed mLedNG;
    AsLed mLedOK;

};

#endif
