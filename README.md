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

    <span style="color: red; ">注：　TPSのキャリブレーションを行うときは、スロットルに指を入れて手動でスロットルを動かす必要があります。
    キャリブレーション中はモーターの電源は切れていますが、万が一のことがあるので必ずモーターの電源線を抜いてから行ってください。指がなくなります。
    </span>
1. 以下を入力して、センサー値を設定します。
    * `1` $\cdot\cdot\cdot$ アクセルペダル全閉時 (`APPS1_MIN`, `APPS2_MIN`, `ITTR_MIN`)
    * `2` $\cdot\cdot\cdot$ アクセルペダル全開時 (`APPS1_MIN`, `APPS2_MIN`, `ITTR_MIN`)
    * `3` $\cdot\cdot\cdot$ スロットル全閉時 (`TPS1_MIN`, `TPS2_MIN`)
    * `4` $\cdot\cdot\cdot$ スロットル全開時 (`TPS1_MAX`, `TPS2_MAX`)
    
    例えば、アクセルペダル全閉時のセンサー値を設定したいときは、アクセルペダル全閉の状態で `1` を入力します。
    ※必ずしも全パターンを設定する必要はありません。
1. `f` を入力すると `---- Calibration Finish ----`、`---- Saved ----` と表示されてキャリブレーションモードが終了し、通常の制御が始まります。
    ここで保存されたセンサー値は再起動しても保存されているので、一度キャリブレーションをすれば次回からする必要はありません。


## ハードウェア
- アクセルペダルポジションセンサー [AS-3](https://www.ipros.jp/product/detail/2000527534/)
- ブレーキ圧センサー [MLH01KPGB06A](https://sps.honeywell.com/jp/ja/products/advanced-sensing-technologies/industrial-sensing/industrial-sensors/industrial-pressure-sensors/mlh-series)
- DCモーター [モータ DCX26L GB KL 12V ギアヘッド GPX26 A 35:1 B7FEDFA2887B](https://www.maxongroup.co.jp/maxon/view/configurator?from=%2Fmaxon%2Fview%2Fcontent%2Fcart&configId=B7FEDFA2887B)
- DMモータードライバー [G2ハイパワーモータードライバ 18v17](https://www.pololu.com/product/2991)
- スロットルポジションセンサー [CP-20H](https://www.midori.co.jp/products/potentiometer/angle_sensor/orange_pot/cp-20h)

