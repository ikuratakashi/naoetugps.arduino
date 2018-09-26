#include "wifigps.Lcd.h"

//アドレス,col数,row数
//LiquidCrystal_I2C GLCD(0x27, 20, 4);
KanaLiquidCrystal_I2C GLCD(0x27, 20, 4);

//初期化
void AsLcd::Init(){
}
//LCD制御を実行
void AsLcd::SetStart(){

  //初期化
  GLCD.begin();
  GLCD.backlight();
  GLCD.kanaOn();
  GLCD.clear();
  IsBuffFull = false;

  //スクロール表示用のバッファ最大Indexを取得
  mRowScrollIdxMax = sizeof(mRowScrollBuf) / sizeof(mRowScrollBuf[0])  - 1;
  mRowScrollIdx = 0;

  //オープニング表示
  DispOpening();
  
}

//オープニング表示
void AsLcd::DispOpening(){
  mDispMode = DISPMODE_OPENING;
  DispAutoRowScroll(">");
  DispAutoRowScroll(">    JOETSU+GPS");
  DispAutoRowScroll(">");
  DispAutoRowScroll(">  arukisoft.com");
  delay(1000);
  DoBuffClear();
  GLCD.clear();
  
}

//ディスプレイへ表示(はみ出した文字はカット)
void AsLcd::DSPrint(int pRow,int pCol,String pTxt){

  String _Buf = pTxt.substring(0,DISP_SIZE_COL);
  GLCD.setCursor(pCol, pRow);
  GLCD.print(_Buf);
  
}

//ディスプレイへ行スクロールするように出力
//引数1:出力文字列
//引数2:番号を表示するかどうか
void AsLcd::DispAutoRowScroll(String pTxt,bool pIsNo = false){

  //行番号のカウント
  mRowNo++;
  if(mRowNo > mRowNoMax){
    mRowNo = 1;
  }
  
  //バッファがフルの場合、一つずつ上にずらし、最後のバッファを空ける
  if(IsBuffFull == true){
    for(int i = 0; i <= mRowScrollIdx; i++){
      mRowScrollBuf[i] = mRowScrollBuf[i + 1];
    }
  }
  
  //バッファの先頭に値を設定する
  mRowScrollBuf[mRowScrollIdx] = pTxt;
  if(pIsNo == true){
    char _buf[64];
    String _bufs = "";
    sprintf(_buf,"%d:",mRowNo);
    _bufs.concat(_buf);
    _bufs.concat(mRowScrollBuf[mRowScrollIdx]);
    mRowScrollBuf[mRowScrollIdx] = _bufs;
  }

  //出力
  GLCD.clear();
  for(int i = 0 ; i <= mRowScrollIdx; i++){
    DSPrint(i,0,mRowScrollBuf[i]);
  }

  //最大Indexを超えないように制御
  mRowScrollIdx++;
  if(mRowScrollIdx > mRowScrollIdxMax){
    mRowScrollIdx = mRowScrollIdxMax;
    IsBuffFull = true;
  }
  
}

//LCD制御を停止
void AsLcd::SetStop(){
}
//処理を1ステップ進める
void AsLcd::DoRunStep(){
}
//処理を1ステップ進める（開始用）
void AsLcd::DoRunStepStart(){  
}
//処理を1ステップ進める（停止用）
void AsLcd::DoRunStepStop(){  
}

//出力バッファをクリアする
void AsLcd::DoBuffClear(){
  mRowNo = 0;
  mRowScrollIdx = 0;
  IsBuffFull = false;
}


