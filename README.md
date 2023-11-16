# ETC
全日本学生フォーミュラ大会　電子制御スロットル(Electronic throttle control)

This project is created with [PlatformIO](https://platformio.org/)

## 使い方
### 基本的な使い方
1. 電源を入れると自動的に起動します。
1. 横のスイッチが上になっていると アイドリング ~ 100%、下になっていると 0% ~ 100% の範囲でスロットルが開閉します。
### シリアルモニタの見方
1. VisualStudioCodeに拡張機能のPlatformioをインストールしてください。
1. VSCodeを開いて、ボックスとPCをUSBケーブルで繋げてください。
1. VSCodeの左下にあるコンセントのマークを押すとシリアルモニタが表示されます。
1. 左半分が生のセンサーの値、右半分が変換後の％表示です。（BPSだけはpsi）（IST Controllerからくるスロットル指令値は "ITTR" で表示されています）
### センサーのキャリブレーション
1. 自動でセンサーをキャリブレーションする機能はついていません。
1. 主に調整するところは、TPS1、TPS2、APPS1、APPS2、ITTR の範囲であろう。シリアルモニタを見ながら、生の値を "constants.hpp" の相当する箇所に書く（書き換える）。
    * APPS1,2 の 設定は APPS_1_RAW_MIN, APPS_2_RAW_MIN, APPS_1_RAW_MAX, APPS_2_RAW_MAX を書き換える。ほかも同様
    * ここで、"MIN", "MAX" はそれぞれ、全閉時の値、全開時の値であり、必ずしも MIN < MAX とならないことに注意。
3. 書き換えたらVSCodeの左下の右矢印のマークを押して、プログラムをマイコンに書き込む。

## ハードウェア
- アクセルペダルポジションセンサー [AS-3](https://www.ipros.jp/product/detail/2000527534/)
- ブレーキ圧センサー [MLH01KPGB06A](https://sps.honeywell.com/jp/ja/products/advanced-sensing-technologies/industrial-sensing/industrial-sensors/industrial-pressure-sensors/mlh-series)
- DCモーター [モータ DCX26L GB KL 12V ギアヘッド GPX26 A 35:1 B7FEDFA2887B](https://www.maxongroup.co.jp/maxon/view/configurator?from=%2Fmaxon%2Fview%2Fcontent%2Fcart&configId=B7FEDFA2887B)
- DMモータードライバー [G2ハイパワーモータードライバ 18v17](https://www.pololu.com/product/2991)
- スロットルポジションセンサー [CP-20H](https://www.midori.co.jp/products/potentiometer/angle_sensor/orange_pot/cp-20h)

