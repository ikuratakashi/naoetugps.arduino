#include "wifigps.Led.h"
#include "wifigps.Timer.h"

//--------------------
//パターン定義
//--------------------

//常に点灯
int PtnFull[] = {
  1,1
};

//フラッシュ20
int PtnFlash20[] = {
   1,20
  ,0,20
};

//フラッシュ50
int PtnFlash50[] = {
   1,50
  ,0,50
};

//フラッシュ100
int PtnFlash100[] = {
   1,100
  ,0,100
};

//フラッシュ200
int PtnFlash200[] = {
   1,200
  ,0,200
};

//フラッシュ300
int PtnFlash300[] = {
   1,300
  ,0,300
};

//フラッシュの繰り返し
int PtnFlashDelay[] = {
   1,100
  ,0,200
  ,1,100
  ,0,800
};

//初期化
//引数1：LED点灯タイプ
void AsLed::Init(int pPinNo){
  //LEDの点灯するピン番号
  mPinNo = pPinNo;
  //タイマー初期化
  mTimer.Init(1);
}

//現在のパターンを取得
void AsLed::GetPaternData(int *pLedOnOff, int *pDelayCnt){
  
  int _pt = mPaternCnt;
  if(_pt + 1 <= mPaternLen - 1){
    *pLedOnOff = mPatern[_pt];
    *pDelayCnt = mPatern[_pt+1];
  }else{
    *pLedOnOff = 0;
    *pDelayCnt = 0;
  }
  
}

//パターンのループ設定
void AsLed::SetPaternLoop(){
  if(mPaternCnt < mPaternLen){
    //パターンカウンタを加算
    mPaternCnt = mPaternCnt + 2;
  }else{
    //パターンのループ
    PternLoopCounter();
    mPaternCnt = 0;
  }
}

//LED点灯開始
//引数1:点灯パターン
//引数2:指定されたループ回数（未指定の場合は、SetStopが実行されるまでループ）
void AsLed::SetRun(String pType,int pLoopCntMax){

  mLoopCntMax = pLoopCntMax;

  if(mType == pType){
    return;
  }

  //LED点灯終了（初期化）
  SetStop();

  //パターン配列の取得 
  mType = pType;
  if(mType == "FULL"){
    mPatern = PtnFull;
    mPaternLen = sizeof(PtnFull) / sizeof(PtnFull[0]);
  }
  if(mType == "F20"){
    mPatern = PtnFlash20;
    mPaternLen = sizeof(PtnFlash20) / sizeof(PtnFlash20[0]);
  }
  if(mType == "F50"){
    mPatern = PtnFlash50;
    mPaternLen = sizeof(PtnFlash50) / sizeof(PtnFlash50[0]);
  }
  if(mType == "F100"){
    mPatern = PtnFlash100;
    mPaternLen = sizeof(PtnFlash100) / sizeof(PtnFlash100[0]);
  }
  if(mType == "F200"){
    mPatern = PtnFlash200;
    mPaternLen = sizeof(PtnFlash200) / sizeof(PtnFlash200[0]);
  }
  if(mType == "F300"){
    mPatern = PtnFlash300;
    mPaternLen = sizeof(PtnFlash300) / sizeof(PtnFlash300[0]);
  }
  if(mType == "FD50"){
    mPatern = PtnFlashDelay;
    mPaternLen = sizeof(PtnFlashDelay) / sizeof(PtnFlashDelay[0]);
  }
  
  IsStart = true;
  mTimerCnt = 0;
  mPaternCnt = 0;
  //PterLoopCnt = 0;
  pinMode(mPinNo,OUTPUT);
  
  //初回の動作を設定
  GetPaternData(&mLedOnOff,&mDelay);
  SetLed(mLedOnOff);  
  
  mTimer.SetRun();
}

//LEDピンへのデジタル出力(HIGH or LOW)
//引数1:LEDピンへの動作
void AsLed::SetLed(int pLedOnOff){
    if(pLedOnOff == 1){
      digitalWrite(mPinNo,HIGH);
    }else{
      digitalWrite(mPinNo,LOW);
    }
}

//LED点灯終了
void AsLed::SetStop(){
  IsStart = false;
  PterLoopCnt = -1;
  mType = "";
  mTimer.SetStop();
  digitalWrite(mPinNo,LOW);
}

//LED点灯処理を1ステップ進める
//メイン側の loop() へ設置する
void AsLed::DoRunStep(){
  if(IsStart == true){

    //タイマーの経過時間をチェック
    mTimer.WatchTimeCheck();

    //タイマーの経過時間が過ぎたかどうか
    if(mTimer.IsOver == true){
      mTimerCnt++;
      //Serial.println(mTimerCnt);
    }

    //LEDパターンの数に達したかどうか
    if(mTimerCnt >= mDelay){
      //パターンの内容を更新
      GetPaternData(&mLedOnOff,&mDelay);
      //LEDの設定
      SetLed(mLedOnOff);
      //タイマーカウント
      mTimerCnt = 0;
      //パターンのループ制御
      SetPaternLoop();
    }
    
  }
}

//パターン実行回数カウント
//100までカウントしたら0に戻る
void AsLed::PternLoopCounter(){
  PterLoopCnt++;
  if(PterLoopCnt > 100){
    PterLoopCnt = 0;
  }

  //ループ数が指定されていた場合
  if(mLoopCntMax > 0){
    //ループ数を超えたら停止
    if(PterLoopCnt >= mLoopCntMax){
      mType = "";
      SetStop();
    }
  }

}

