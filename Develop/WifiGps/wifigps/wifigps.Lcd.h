#ifndef WIFIGPS_LCD
#define WIFIGPS_LCD

#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <KanaLiquidCrystal.h>

//LCDクラス
class AsLcd
{
  public :
    //初期化
    //引数1:アドレス
    //引数2:SDAピン
    //引数2:SCLピン
    void Init();
    //LCD制御を実行
    void SetStart();
    //LCD制御を停止
    void SetStop();
    //オープニング表示
    void DispOpening();
    
    //ディスプレイへ表示(はみ出した文字はカット)
    void DSPrint(int pRow,int pCol,String pTxt);
    //ディスプレイへ行スクロールするように出力
    //引数1:出力文字列
    //引数2:番号を表示するかどうか
    void DispAutoRowScroll(String pTxt,bool pIsNo = false);
    
    //処理を1ステップ進める
    void DoRunStep();
    //処理を1ステップ進める（開始用）
    void DoRunStepStart();
    //処理を1ステップ進める（停止用）
    void DoRunStepStop();
    //出力バッファをクリアする
    void DoBuffClear();

    //ディスプレイモード
    int mDispMode = "";
    //ディスプレイモード
    const int DISPMODE_OPENING = 1;
    //ディスプレイサイズ
    const int DISP_SIZE_ROW = 4;
    const int DISP_SIZE_COL = 20;
    //行スクロール出力用バッファ(ディスプレイの行数)
    String mRowScrollBuf[4];
    //行スクロールカウント最大数
    int mRowScrollIdxMax = 0;
    //行スクロールカウンタ
    int mRowScrollIdx = 0;
    //バッファフルフラグ
    bool IsBuffFull = false;
    //行番号表示のカウント
    int mRowNo = 0;
    //行数表示のカウント最大値
    int mRowNoMax = 9;
    
};

#endif
