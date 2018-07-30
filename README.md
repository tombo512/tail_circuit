# tail_circuit
Arduino Nanoで尾翼（rudder, elevator）サーボモータを動かすプログラムです．
rudderは右左のスイッチ，elevatorはスライド式可変抵抗を用いています．

## 1. ライブラリインポート
~~このコードはVerSpeedServoというライブラリをインポートしています．~~
~~Arduinoフォルダ下のLibraryフォルダに以下で配布されているヘッダーファイルをインストールしてください．~~

[VerSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo)

(2018/05/13追記)
VerSpeedServoの使用は取りやめました．
現在のバージョンでは通常のServo.hをインクルードしています．

(2018/07/08追記)
iterWriteメソッドをコード内で定義しました．

## 2. 拡張子
Arduino IDEのスケッチは拡張子が.inoになります．エディタでコーディングする際は.cppを使用した方がシンタックスハイライトが入ってやりやすいですが，スケッチで読み込む際は.inoに変更してください.
