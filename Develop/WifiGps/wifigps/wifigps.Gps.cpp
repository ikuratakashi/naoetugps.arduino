#include "wifigps.Gps.h"
#include "wifigps.Debug.h"

//初期化
//引数1:入力用シリアル
//引数2:出力用シリアル
//引数3:GPS情報の出力間隔(ミリ秒)
//引数4:デバッグモジュール
void AsGps::Init(HardwareSerial *pInSerial,HardwareSerial *pOutSerial,int pPutGpsInterval,AsDebug *pDebug){

  //退避
  mSerialGpsIn  = pInSerial;
  mSerialGpsOut = pOutSerial;
  mPutGpsInterval = pPutGpsInterval;
  mTimer.Init(mPutGpsInterval);
  mDebug = pDebug;

  //座標のバッファ格納初期化
  mLocationMax = sizeof(mLocationBuf) / sizeof(mLocationBuf[0]);
  mLocationCnt = 0;

  //LED
  mStatusLed.Init(22);
  mSendLed.Init(24);
  
}

//GPS取得開始
void AsGps::SetStart(){
  IsStart = true;
  mTimer.SetRun();
}
//GPS取得終了
void AsGps::SetStop(){
  IsStart = false;
  mTimer.SetStop();
  mSendLed.SetStop();
  mStatusLed.SetStop();
}

//GPSの処理を１ステップ進める
void AsGps::DoRunStep(){
  
  if(IsStart == true){
    
    //タイマーの経過時間をチェック
    mTimer.WatchTimeCheck();
    
    //タイマーの経過時間が過ぎたかどうか
    if(mTimer.IsOver == true){
      
      //座標送信...LED
      mSendLed.SetRun("F50",3);

      //座標の送信
      String _BufLNG;
      String _BufLAT;
      String _BufALT;
      GetLocationBuf(&_BufLAT,&_BufLNG,&_BufALT);
      
      mDebug->DebugPrint("LAT=:" + _BufLAT);
      mDebug->DebugPrint("LNG=:" + _BufLNG);

      //Json作成
      String _BufJson = "";
      _BufJson += "{";
      _BufJson += "LAT:" + _BufLAT + "";
      _BufJson += ",LNG:" + _BufLNG + "";
      _BufJson += ",ALT:" + _BufALT + "";
      _BufJson += "};;";
      mSerialGpsOut->println(_BufJson);
      
    }else{

      //GPS位置情報をバッファ
      if(mSerialGpsIn->available() > 0){
        mGps.encode(mSerialGpsIn->read());
        if (mGps.location.isUpdated()){ // GPS情報が正常に更新されていたら
          AddLocationBuf(mGps);
          //受信OK...LED
          mStatusLed.SetRun("FD50",1);
        }
      }
      
    }
  }
  
  //ステータス用のLED制御
  mStatusLed.DoRunStep();
  mSendLed.DoRunStep();
  
}

//バッファへ座標を追加する
//バッファは0番目から新しい情報
//引数1：GPSロケーション情報
void AsGps::AddLocationBuf(TinyGPSPlus pGps){

  //ロケーション情報の移動（最初の情報を空ける）
  for(int i=0;i < mLocationMax - 1 ;i++){
    mLocationBuf[i+1].LAT = mLocationBuf[i].LAT;
    mLocationBuf[i+1].LNG = mLocationBuf[i].LNG;
    mLocationBuf[i+1].ALT = mLocationBuf[i].ALT;
  }
  
  mLocationBuf[0].LAT = String(pGps.location.lat(),6);
  mLocationBuf[0].LNG = String(pGps.location.lng(),6);
  mLocationBuf[0].ALT = pGps.altitude.meters();

  mLocationCnt++;
  if(mLocationCnt >= mLocationMax){
    mLocationCnt = mLocationMax;
  }
  
}


//バッファから座標を取得する
//引数1(参照)：経度
//引数2(参照)：緯度
//引数3(参照)：標高
void AsGps::GetLocationBuf(String *pLAT,String *pLNG,String *pALT){
  *pLAT = mLocationBuf[0].LAT;
  *pLNG = mLocationBuf[0].LNG;
  *pALT = mLocationBuf[0].ALT;
}



