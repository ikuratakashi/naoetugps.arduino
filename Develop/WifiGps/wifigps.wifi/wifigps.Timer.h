#ifndef WIFIGPS_TIMER
#define WIFIGPS_TIMER
#include <Metro.h>

//タイマークラス
class AsTimer
{
  public : 
  
      //初期化
      //引数1：タイマーのインターバル(単位：ミリ秒)
      void Init(unsigned long pInterval);
      
      //タイマーの実行インターバル
      unsigned long mInterval;
      //タイマー開始
      void SetRun();
      //タイマー停止
      void SetStop();
      //タイマーの時間経過確認
      void WatchTimeCheck();
      //指定時間が経過したかどうか
      bool IsOver = false;
      //タイマーが実行中かどうか
      bool IsRun = false;
      //タイマー
      Metro mMetro;
};

#endif
