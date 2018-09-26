#ifndef WIFIGPS_LED
#define WIFIGPS_LED

#include <Arduino.h>
#include "wifigps.Timer.h"

//LEDクラス
class AsLed
{
  public : 
  
      //初期化
      //引数1：LED点灯のタイプ
      void Init(int pPinNo);
      
      //引数1:点灯パターン
      //引数2:指定されたループ回数（未指定の場合は、SetStopが実行されるまでループ）
      void SetRun(String pType,int pLoopCntMax = -1);
      
      //Led点灯処理終了
      void SetStop();
      //Led点灯処理を1ステップ進める
      void DoRunStep();
      //現在のパターンを取得
      //引数1:LEDのOn/Off
      //引数2:待機カウント数
      void GetPaternData(int *pLedOnOff, int *DelayCnt);
      //LEDピンへのデジタル出力(HIGH or LOW)
      //引数1:LEDピンへの動作
      void SetLed(int pLedOnOff);
      //パターンのループ設定
      void SetPaternLoop();
      //パターン実行回数カウント
      //100までカウントしたら0に戻る
      void PternLoopCounter();

      //パターンのループ回数(最大100回までカウントその後は0でリセット)
      int PterLoopCnt = 0;

      //点灯タイプ
      String mType = "";
      //タイマーカウンター
      long mTimerCnt = 0;
      //パターンカウンター
      long mPaternCnt = 0;
      //開始フラグ
      bool IsStart = false;
      //パターン配列
      int *mPatern;
      //パターン配列数
      int mPaternLen = 0;
      //Ledのピン番号
      int mPinNo = -1;
      //現在の状態 LED On/Off
      int mLedOnOff = 0;
      //現在の状態 Delayカウント
      int mDelay = 0;
      //タイマー
      AsTimer mTimer;
      //ループカウント最大
      int mLoopCntMax = -1;

};

#endif
