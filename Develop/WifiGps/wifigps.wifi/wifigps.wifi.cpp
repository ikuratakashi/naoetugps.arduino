#include "wifigps.wifi.h"
#include "wifigps.wifi.ini.h"

//初期設定
AsWifiIni WIFI_INI;

//初期化
void AsWifi::Init(HardwareSerial *pSerialIn,HardwareSerial *pSerialOut){

  //シリアルモジュールを設定
  mSerialIn = pSerialIn;
  mSerialOut = pSerialOut;

  //設定
  mSSID = WIFI_INI.WIFI_SSID;
  mPASS = WIFI_INI.WIFI_PASS;
  mHost = WIFI_INI.WIFI_HOST;
  mPort = WIFI_INI.WIFI_PORT;
  mPOST_CLASS = WIFI_INI.WIFI_POST_CLASS;
  
  //LED
  mLedOK.Init(DBG_LED_OK);
  mLedNG.Init(DBG_LED_NG);

  //終端文字列の長さを取得
  AS_WIFI_SERIAL_END_LEN = AS_WIFI_SERIAL_END.length();

}

//wifi処理開始
void AsWifi::SetStart(){

  //シリアル通信開始
  mSerialIn->begin(9600);
  mSerialOut->begin(9600);

  int _cnt = 0;
  while(_cnt < 20){
    _cnt++;
    if(_cnt % 2 == 0){
      mLedOK.SetRun("FULL");
      mLedNG.SetStop();
    }else{
      mLedNG.SetRun("FULL");
      mLedOK.SetStop();
    }
    delay(500);
  }
  mLedOK.SetStop();
  mLedNG.SetStop();
  
  //wifi接続開始
  if (DoWifiCon() == RESULT_OK) {
    IsConnection = true;
    mLedOK.SetRun("F300");
  }else{
    IsConnection = false;
    mLedNG.SetRun("FD50");
  }
  
}

//wifi接続
bool AsWifi::DoWifiCon() {
  
  WriteSerial("SSID " + mSSID);
  WriteSerial("PASS " + mPASS);
  
  WiFi.begin(mSSID.c_str(), mPASS.c_str());
  
  int count = 0;
  WriteSerial("Con Start");
  mLedOK.SetRun("FD50");
  
  while ( count < 20 ) {

    int countled = 0;
    while (countled < 2000){
      RunStatusLed();
      countled++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      WriteSerial("Con Finish");
      mLedOK.SetStop();
      return RESULT_OK;
    }
    if(count <= 0){
      WriteSerial("Con Try");
    }else{
      WriteSerial(".");
    }
    delay(1000);
    count++;
  }
  
  mLedNG.SetRun("FULL");
  WriteSerial("Con Faild");
  RunStatusLed();
  return RESULT_NG;
  
}

//処理を進める
bool AsWifi::DoRunStep(){

  bool IsBufFull = false;

  //Wifi接続チェック
  SetWifiStatusToLED();

  //シリアル情報の受信
  if (mSerialIn->available() > 0) {

    if(mGetCnt <= 0){
      WriteSerial("Read Begin");
    }

    char _buf[1];
    _buf[0] = mSerialIn->read();
    if((_buf[0] & 0xF0) != 0xF0){
      mGetBuf[mGetCnt] = _buf[0];
      mGetCnt++;
    }
    IsSerialReading = true;

    //バッファが全て使用された場合は、コミットと判断
    //文字配列の最後には\0が入るため、1つ少ない大きさで判断
    if(mGetCnt >= AS_WIFI_SERIAL_MAX - 1){
      IsSerialCommit = true;
      IsBufFull = true;
    }
    
  }else{
    if(IsSerialReading == true){
      IsSerialCommit = true;
    }
    IsSerialReading = false;
  }

  //バッファの情報が全て受信したか判定
  if(IsSerialCommit == true){
    
    WriteSerial("Read Commit-----------");
    mGetBuf[mGetCnt] = '\0';
    
    //Serial.println("mGetBuf=[" + String(mGetBuf) + "] bef");
    
    BufferClear(mGetBufTmp);
    strcpy(mGetBufTmp,mGetBuf);
    mGetBufTmp[mGetCnt] = '\0';
    String _BufChk = String(mGetBufTmp);
    //Serial.println("_BufChk=[" + _BufChk + "]");
    
    //終端の文字列があるかどうか
    if(_BufChk.endsWith(AS_WIFI_SERIAL_END)){
      mGetBuf[mGetCnt - AS_WIFI_SERIAL_END_LEN] = '\0';
      IsSerialCommitAll = true;
      //Serial.println("mGetBuf1=[" + String(mGetBuf) + "]");
    }else if(IsBufFull == true){
    //バッファサイズが超えているか
      mGetBuf[AS_WIFI_SERIAL_MAX - 1] = '\0';
      IsSerialCommitAll = true;
      //Serial.println("mGetBuf2=[" + String(mGetBuf) + "]");
      //溜まったSerialのバッファを全て空読み
      WriteSerial("Buffer clear start ");
      while(mSerialIn->available()){
        char _delbuf[1];
        _delbuf[0] = mSerialIn->read();
        //Serial.println(_delbuf[0]);
      }
      WriteSerial("Buffer clear finish ");
      
    }

    IsSerialCommit = false;
    
  }
  
  //取得した情報を送信する
  if(IsSerialCommitAll == true){
    
    WriteSerial("Read CommitAll---------");

    //複数の情報を受信した場合、最後の１件を取得する
    String _Buf = String(mGetBuf);
    
    //制御用の情報を初期化
    BufferClear(mGetBuf);
    IsSerialCommit = false;
    IsSerialCommitAll = false;
    mGetCnt = 0;

    //最後の{****}を取得
    int _GetStart = 0;
    _GetStart = _Buf.lastIndexOf("}");
    //Serial.print("[" + _Buf + "]" + " } _GetStart=");
    //Serial.println(_GetStart);
    if(_GetStart > 0){
      _Buf = _Buf.substring(0,_GetStart + 1);
    }
    _GetStart = _Buf.lastIndexOf("{");
    //Serial.print("[" + _Buf + "]" + " { _GetStart=");
    //Serial.println(_GetStart);
    if(_GetStart > 0){
      _Buf = _Buf.substring(_GetStart);
    }
    
    //受信データとして正しいかを判断
    //先頭に"{"がついていること
    bool IsJson = false;
    if((_Buf.substring(0,1) == "{") && (_Buf.endsWith("}") == true)){
      IsJson = true;
    }else{
      IsJson = false;
    }
    if(IsJson == true){
      WriteSerial("Json...[" + _Buf + "]");
    }else{
      WriteSerial("No Json...[" + _Buf + "]");
    }
    
    if(IsConnection == true && IsJson == true){
    //wifiでの接続が完了している場合 かつ Jsonデータ
  
      //Jsonのパース
      WriteSerial("Parse Start");
      
      StaticJsonBuffer<200> _jBuf;
      JsonObject& _pBuf = _jBuf.parseObject(_Buf.c_str());
      
      // パースが成功したか確認
      if (_pBuf.success() == false) {
        //失敗したら終了
        WriteSerial("Parse Error!");
        return RESULT_NG;
      }

      WriteSerial("Parse Finish");
  
      //-------------------------------
      //URL引数の作成
      //-------------------------------
      String _Url  = "/" + mPOST_CLASS;
      String _Result = "";
  
      // データの取り出しは暗黙の型変換
      const char *_Lat = _pBuf["LAT"];
      const char *_Lng = _pBuf["LNG"];

      //値確認
      if(String(_Lat).equals("") == false && String(_Lng).equals("") == false){
 
        WriteSerial("Value OK");
      
        //POSTデータ作成
        String _Data = "";
        _Data += "lat=" + String(_Lat);
        _Data += "&";
        _Data += "lng=" + String(_Lng);
        _Data += "&";
        _Data += "type=1";
    
        mLedOK.SetRun("FULL");
        
        _Result = DoHttpPost(mHost,mPort,_Url, _Data);
        WriteSerial(_Result);
  
        //ライト点滅
        int _cnt = 0;
        mLedOK.SetRun("F50");
        while(_cnt < 100000){
          _cnt++;
          RunStatusLed();
        }
        mLedOK.SetRun("F300");
        
      }else{

        WriteSerial("Value NG");
        mLedOK.SetStop();
        mLedNG.SetStop();
        
        //ライト点滅
        int _cnt = 0;
        mLedNG.SetRun("F50");
        while(_cnt < 100000){
          _cnt++;
          RunStatusLed();
        }
        
        mLedOK.SetRun("F300");
        
      }
      
    }//end - if(IsConnection == true && IsJson == true){

  }//end - if(IsSerialCommitAll == true){
  
  //ステータス用のLED制御
  RunStatusLed();
  
  return RESULT_OK;
  
}

//情報をPOSTする
String AsWifi::DoHttpPost(const char* pHost , int pPort , String pClass, String pData){

  bool _IsConnected = false;

  if(client.connected() == false){
    WriteSerial("Conneced False");
    WriteSerial("Connecting Start");
    _IsConnected = client.connect(pHost, pPort);
    if(_IsConnected == true){
      WriteSerial("Connecting OK");
    }
  }
  
  if (_IsConnected == true) {
    
    WriteSerial("HTTP GET Start");
    
    WriteSerial("GET " + pClass + "?" + pData + " HTTP/1.1");
    WriteSerial("Host: " + (String)pHost);
    
    client.println("GET " + pClass + "?" + pData + " HTTP/1.1");
    client.println("Host: " + (String)pHost);
    client.println("User-Agent: ESP8266/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(pData.length());
    client.println();
    client.println(pData);
    
    delay(10);
    
    String response = client.readString();
    int bodypos =  response.indexOf("\r\n\r\n") + 4;
    
    return response.substring(bodypos);
    
  }else{
    return "Connecting NG";
  }
  
}

//シリアルへの結果出力
void AsWifi::WriteSerial(String pMessage){
  mSerialOut->println("WIFI:" + pMessage);
}

//ステータスのステップ実行
void AsWifi::RunStatusLed(){
  //ステータス用のLED制御
  mLedOK.DoRunStep();
  mLedNG.DoRunStep();
}

//バッファクリア
void AsWifi::BufferClear(char *pBuff){
  int _cnt = sizeof(pBuff) / sizeof(char);
  for(int i=0;i < _cnt;i++){
    pBuff[i] = '\0';
  }
}

//Wifi接続確認
bool AsWifi::IsWifiStatus(){
  if(WiFi.status() == WL_CONNECTED){
    //接続中
    return RESULT_OK;
  }else{
    //切断中
    return RESULT_NG;
  }
}

//Wifi接続を確認してLEDの点灯を切り替える
bool AsWifi::SetWifiStatusToLED(){
  if(IsWifiStatus() == RESULT_OK){
    //接続中のため未処理  
    mLedOK.SetRun("F300");
    mLedNG.SetStop();
  }else{
    //切断中のためNGのLED発光
    mLedOK.SetStop();
    mLedNG.SetRun("F300");
  }
}

