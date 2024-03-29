# ETC
全日本学生フォーミュラ大会　電子制御スロットル(Electronic throttle control)

This project is created with [PlatformIO](https://platformio.org/)


## 使い方
### 基本的な使い方
1. 電源を入れると自動的に起動します。
1. ボックス横のスイッチが上になっていると アイドリング ~ 100%、下になっていると 0% ~ 100% の範囲でスロットルが開閉します。

### シリアルモニタの見方
1. ボックスとPCをUSBケーブルでつないで、シリアルモニターを開いてください。（シリアルモニタは好きなものを使ってください。）

    シリアルモニタの例
    * https://learn.microsoft.com/ja-jp/cpp/embedded/serial-monitor?view=msvc-170&tabs=visual-studio
    * https://ehbtj.com/info/serial-monitor-pc-software/
    
    VSCodeのPlatformIO拡張機能にはシリアルモニターが含まれているのでそれを使っても構いません。
1. `Baud Rate` を `115200`、ポートを `Silicon Labs CP210x ...`に指定して、モニターを開始してください。 
1. 左半分が生のセンサーの値、右半分が変換後の％表示です。（BPSだけはpsi）（IST Controllerからくるスロットル指令値は "ITTR" で表示されています）

### センサーのキャリブレーション
簡単な操作でセンサー値をキャリブレーションできます。
1. シリアルモニターを開始して`s`を入力すると `---- Calibration Start ----` と表示されて、キャリブレーションモードに入ります。

    注：　キャリブレーション中もモーターは動いています。（電スロの動作をしています。）

1. 以下を入力して、センサー値を設定します。
    * `1` $\cdot\cdot\cdot$ アクセルペダル全閉時 (`APPS1_MIN`, `APPS2_MIN`, `ITTR_MIN`)
    * `2` $\cdot\cdot\cdot$ アクセルペダル全開時 (`APPS1_MIN`, `APPS2_MIN`, `ITTR_MIN`)
    * `3` $\cdot\cdot\cdot$ スロットル全閉時 (`TPS1_MIN`, `TPS2_MIN`)
    * `4` $\cdot\cdot\cdot$ スロットル全開時 (`TPS1_MAX`, `TPS2_MAX`)
    * `5` $\cdot\cdot\cdot$ アイドリング時 (`APPS_IDLING`)
    
    例えば、アクセルペダル全閉時のセンサー値を設定したいときは、アクセルペダル全閉の状態で `1` を入力します。
    ※必ずしも全パターンを設定する必要はありません。
    
    また、必要であれば以下のキーも使用します。
    * `m` $\cdot\cdot\cdot$ モーターの電源OFF
1. `f` を入力すると `---- Calibration Finish ----`、`---- Saved ----` と表示されてキャリブレーションモードが終了します。
    ここで保存されたセンサー値は再起動しても保存されているので、一度キャリブレーションをすれば次回からする必要はありません。

#### キャリブレーションのコツ
1. キャリブレーション中もモーターは動いているので、元から設定されているTPSの範囲が設定したい範囲より狭い場合TPSの全閉、全開を再現できません。このときは、キャリブレーションモードにおいて`m`キーを入力して、モーターの電源をOFFにして、手動でスロットルを全開、全閉の位置にセットして、`3`、`4`キーを入力して`TPS_MIN`、`TPS_MAX`をセットします。このとき、全閉全開それぞれ、ちょっと閉じすぎ、開きすぎ、ぐらいの開度でセットするのがコツです。セットしたら、`f`キーを入力して一旦値を保存し、スロットルを再起動します。
    
    注：　ここで手動でスロットルをセットした場合、スロットルのギアにはバックラッシがあるので、手動でスロットルを動かしてちょうど全閉（or全開）にしたときのセンサー値とモーターが動いてちょうど全閉（or全開）になるときのセンサー値はバックラッシ分異なります。なので、手動セットだけでは不十分です。

1. 再起動後、スロットルは 全閉よりちょい閉じすぎ～全開よりちょい開きすぎ ぐらいの範囲で動作します。この状態で、キャリブレーションモードに入り、アクセルペダルを動かしてスロットルがちょうど全閉、全開のときに`3`、`4`キーを入力して`TPS_MIN`、`TPS_MAX`をセットします。こうすることで、設定したセンサー値にモーターが動いたときにちょうどスロットルが全閉、全開になるようになります。セットしたら、`f`キーを入力してキャリブレーション完了となります。

さらにコツ

* アクセルペダルからスロットルは距離が離れているので、1人ではアクセルペダルを操作しながらスロットルを見ることができません。この場合は、キャリブレーション用のMOCKアクセルペダルセンサーを使います。電スロボックスから出ているAPPSのコネクタから一度車両のアクセルペダルを外し、MOCKセンサーをつないで、MOCKの全閉、全開を一旦キャリブレーションしてから上記の作業を行います。終わったらAPPSコネクタを車両のアクセルペダルにつないで、車両のアクセルペダルでもう一度キャリブレーションを行って完了です。


## ハードウェア
- アクセルペダルポジションセンサー [AS-3](https://www.ipros.jp/product/detail/2000527534/)
- ブレーキ圧センサー [MLH01KPGB06A](https://sps.honeywell.com/jp/ja/products/advanced-sensing-technologies/industrial-sensing/industrial-sensors/industrial-pressure-sensors/mlh-series)
- DCモーター [モータ DCX26L GB KL 12V ギアヘッド GPX26 A 35:1 B7FEDFA2887B](https://www.maxongroup.co.jp/maxon/view/configurator?from=%2Fmaxon%2Fview%2Fcontent%2Fcart&configId=B7FEDFA2887B)
- DMモータードライバー [G2ハイパワーモータードライバ 18v17](https://www.pololu.com/product/2991)
- スロットルポジションセンサー [CP-20H](https://www.midori.co.jp/products/potentiometer/angle_sensor/orange_pot/cp-20h)

