#include "wifigps.Timer.h"

//タイマー実行
void AsTimer::Init(unsigned long Interval){
  mInterval = Interval;
}

//タイマー実行
void AsTimer::SetRun(){
  IsOver = false;
  IsRun = true;
  mMetro.interval(mInterval);
}

//タイマーの停止
void AsTimer::SetStop(){
  mMetro.reset();
  IsRun = false;
  IsOver = false;
}

//経過時間のチェック
void AsTimer::WatchTimeCheck(){
  if(IsRun == true){
    if(mMetro.check() == 1){
      IsOver = true;
    }else{
      IsOver = false;
    }
  }else{
    IsOver = false;
  }
}

