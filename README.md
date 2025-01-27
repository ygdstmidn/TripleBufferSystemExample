# TripleBufferSystemExample
## 概要
TripleBufferSystemのサンプルプログラム\
mbedでいうBufferedSerialのようなもの

| 項目         | 仕様             |
| ------------ | ---------------- |
| 使用マイコン | NUCLEO_F446RE    |
| 使用IDE      | CubeIDE,VSCode   |
| 使用言語等   | C++,STM32CubeHAL |

## 今後の目標

## 何を実現できたのか
TripleBufferSystemのサンプルプログラムを作成，動作確認

## ピン設定
| ピン | 設定        | ラベル      | 備考           |
| ---- | ----------- | ----------- | -------------- |
| PA5  | GPIO_Output | DebugLED    | ラベルのみ変更 |
| PC13 | GPIO_EXTI13 | DebugButton | ラベルのみ変更 |

## クロック設定
| 項目 | 内容 | 備考 |
| ---- | ---- | ---- |
|      |      |      |

## 変更した設定
### usart2
#### NVIC Settings
USART2 global interrupt ✅

## プログラム概要
### 定義
TripleBufferSystemClass tbsPcUart
### setup()
Hello WorldとPCに送信\
tbsPcUartの初期化，関数設定\
PCとのUSARTのread割り込みの設定
### loop()
now = HAL_GetTick()
#### 0.1秒ごとに
現在の経過時間をPCに送信\
ボタンが押されていた場合，LEDの点灯/消灯を切り替える
#### 1秒ごとに
tbsPcUart.read()\
readした結果をPCに送信\
バッファが溢れていたらバッファを作り直し\
1文字以上取得していればfflush(NULL)\
### tbsPcUartAfterSetup()
PCとのUSARTのread割り込み中止\
PCとのUSARTのread割り込み設定\
割り込み有効化
