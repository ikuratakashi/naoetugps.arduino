# Naoetu GPS.arduino

Naoetu GPSの座標をarduinoから発信する  

ソースは、Arduino Mega専用  
ハードウェアシリアルの２以降を使用とデジタルピンの20番台を使用  

### ◆機器構成
・マイコン ...[Arduino Mega(ハードウェアシリアルを贅沢に複数使用)](https://amzn.to/2NE04Hv)  
・Wifi ...[ESP-WROOM-02](https://amzn.to/2R0oOHR)  
・GPS ...[GY-GPS6MV2](https://amzn.to/2R1m3X0)  
・LCD ...[IIC/I2C LCD2004](https://amzn.to/2zwlkGA)  

### ◆どんな動きか
1.マイコンでGPSからの座標を取得
    
2.受信したGPSの座標はLCDへ表示  

3.jsonに整形  
  整形json：{lng:9999.999,lat:9999.999};;  
  ※座標は10進緯度経度（世界測地系...だと思う）  
  ※最後のセミコロン(;)は、終端文字列  
  
4.一定期間毎にwifiへjson形式の座標を送信  

5.json形式の座標をwifiで取得し、サーバへPOST  
ポストする値は終端文字(;;)がある最後の情報１件のみ。  
送信の際にwifiで受信したバッファをクリアしている。  

※wifiの[ESP-WROOM-02](https://amzn.to/2R0oOHR)は、Arduinoとして動作する。Arduinoにwifi機能が付いたスグレモノ！！  

### ◆プログラムインストール(Arduino)
1. USBでArduinoとPCで接続
1. Arduino IDEのボードセレクトで"Arduino Mega"を選択
1. Arduino IDEのシリアルポートをArduinoが接続されているポートを選択
1. コンパイル＆ボードへ書き込み

### ◆プログラムインストール(ESP-WROOM-02)
1. [シリアルコンバータ](https://amzn.to/2NFXvod)経由でESP-WROOM-02とPCを接続
1. Arsuino IDEのボードに対する設定（※ボード設定 参照）
1. Arduino IDEのボードセレクトで"ESP-12 Module"を選択
1. Arduino IDEのシリアルポートをシリアルコンバータが接続されているポートを選択
1. コンパイル＆ボードへ書き込み

シリアルコンバータは安いのを使うとすぐ壊れる確率が高いです。
最初購入したのは(398円)、TXCとRXCが逆に書いてありました...そしてすぐ壊れた...

#### ◆ボード設定
|プロパティ名|値|
|:--|:--:|
|Flash Size|4M(3MSPIFFS)|
|ResetMethod|ck|
|CPU Frequency|80MHz|
|Upload Speed|115200|
|Erase Flash|Only Sketch|

## GITにコミットされていないファイルについて

#### wifigps.wifi.ini.h
このファイルには、wifiのパスワード等を記載する為、gitにはコミットしていません。
各自用意して下さいね。　　

/wifigps.wifi  
 └ wifigps.wifi.ini.h  
```
#ifndef WIFIGPS_WIFI_INI
#define WIFIGPS_WIFI_INI

#include <Arduino.h>

//設定ファイル読み込みクラス
class AsWifiIni{
  public:
    void Init();
  
    //wifi接続情報
    const String WIFI_SSID = "[SSID]";
    const String WIFI_PASS = "[パスワード]";
    
    //POST先情報
    ////ホスト
    char* WIFI_HOST = "[GPSを送信するホスト名（例:hogehoge.com）]";
    ////接続ポート
    const int WIFI_PORT = [GPSを送信するホストのsocket.io待ち受けポート番号];
    ////ポスト先クラス名
    const String WIFI_POST_CLASS = "[ポストするクラス名]";
};
#endif
```
