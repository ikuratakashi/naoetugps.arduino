#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");

  Serial.begin(115200);
  
}

void loop()
{

  if(Serial.available()){                   //シリアルポートにデータがたまっているか確認
    String bufShow;                             //受信した文字列を格納する変数
    String bufLcd;                             //受信した文字列を格納する変数
    while (1) {                             //無限ループの開始
      if(Serial.available()){               //シリアルポートにデータがたまっているか確認

        //入力値の取得
        char temp = char(Serial.read());    //int型からchar型へ変換して一時変数に受信データを保持
        
        //シリアルモニタ用
        bufShow += temp;                        //受信した文字列を後方に結合させる
        if(temp == '\n'){
          break;
        }            //改行コード（\n）=（￥n）が来たらループを終了

        //LCDディスプレイ用
        if(temp == '\r'){
          break;
        }            //改行コード（\n）=（￥n）が来たらループを終了
        bufLcd += temp;                        //受信した文字列を後方に結合させる
        
      }
    }
    Serial.print(bufShow);                      //受信したデータを送り返す
    lcd.print(bufLcd);
  }
        
}
