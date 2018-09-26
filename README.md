# Naoetu GPS.arduino

Naoetu GPSの座標をarduinoから発信する

### 機器構成
・マイコン ...[Arduino Mega(ハードウェアシリアルを使用)](https://amzn.to/2NE04Hv)
・Wifi ...[ESP-WROOM-02](https://amzn.to/2R0oOHR)
・GPS ...[GY-GPS6MV2](https://amzn.to/2R1m3X0)
・LCD ...[IIC/I2C LCD2004](https://amzn.to/2zwlkGA)

### プログラムインストール(Arduino)
1. USBでArduinoとPCで接続
1. Arduino IDEのボードセレクトで"Arduino Mega"を選択
1. Arduino IDEのシリアルポートをArduinoが接続されているポートを選択
1. コンパイル＆ボードへ書き込み

### プログラムインストール(ESP-WROOM-02)
1. シリアルコンバータ経由でESP-WROOM-02とPCを接続
1. Arsuino IDEのボードに対する設定（※ボード設定 参照）
1. Arduino IDEのボードセレクトで"ESP-12 Module"を選択
1. Arduino IDEのシリアルポートをシリアルコンバータが接続されているポートを選択
1. コンパイル＆ボードへ書き込み

#### ボード設定

|プロパティ名|値|
|:--|:--:|
|align left|align right|
|Flash Size|4M(3MSPIFFS)|
|ResetMethod|ck|
|CPU Frequency|80MHz|
|Upload Speed|115200|
|Erase Flash|Only Sketch|
