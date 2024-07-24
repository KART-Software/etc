# ETC

全日本学生フォーミュラ大会　電子制御スロットル(Electronic throttle control)

This project is created with [PlatformIO](https://platformio.org/)

## 使い方

### 基本的な使い方

1. 電源を入れると自動的に起動します。
1. コックピットスイッチのノブが左下になっていると `0% ~ 100%`、左上になっていると `アイドリング ~ 95%`、上になっていると `アイドリング ~ 60%` の範囲でスロットルが開閉します。

### シリアルモニタの見方

1. ボックスと PC を USB ケーブルでつないで、シリアルモニターを開いてください。（シリアルモニタは好きなものを使ってください。）

   シリアルモニタの例

   - https://learn.microsoft.com/ja-jp/cpp/embedded/serial-monitor?view=msvc-170&tabs=visual-studio
   - https://teratermproject.github.io/

   VSCode の PlatformIO 拡張機能にはシリアルモニターが含まれているのでそれを使っても構いません。

1. `Baud Rate` を `115200`、ポートを `Silicon Labs CP210x ...`に指定して、モニターを開始してください。
1. 左半分が生のセンサーの値、右半分が変換後の％表示です。（BPS だけは psi）（IST Controller からくるスロットル指令値は "ITTR" で表示されています）

### センサーのキャリブレーション、その他設定

簡単な操作で設定を変更することができます。

1. シリアルモニターを開始して`s`を入力すると `---- Calibration Start ----` と表示されて、キャリブレーションモードに入ります。

   注：　キャリブレーション中もモーターは動いています。（電スロの動作をしています。）

1. 各設定項目

   - 以下を入力して、センサー値を設定します。

     - `1` $\cdot\cdot\cdot$ アクセルペダル全閉時 (`APPS1_MIN`, `APPS2_MIN`, `ITTR_MIN`)
     - `2` $\cdot\cdot\cdot$ アクセルペダル全開時 (`APPS1_MAX`, `APPS2_MAX`, `ITTR_MAX`)
     - `3` $\cdot\cdot\cdot$ スロットル全閉時 (`TPS1_MIN`, `TPS2_MIN`)
     - `4` $\cdot\cdot\cdot$ スロットル全開時 (`TPS1_MAX`, `TPS2_MAX`)
     - `5` $\cdot\cdot\cdot$ アイドリング時 (`APPS_IDLING`)

     例えば、アクセルペダル全閉時のセンサー値を設定したいときは、アクセルペダル全閉の状態で `1` を入力します。

   - 以下を入力して、Plausibility Check の有無を設定します。（押すたびに`true`と`false`が入れ替わります）

     - `q` $\cdot\cdot\cdot$ APPS1 と APPS2 の値の整合性 (`APPS_CHECK_FLAG`)
     - `w` $\cdot\cdot\cdot$ TPS1 と TPS2 の値の整合性 (`BPS_CHECK_FLAG`)
     - `e` $\cdot\cdot\cdot$ APPS1 の値が範囲内にあるか (`APPS1_CHECK_FLAG`)
     - `r` $\cdot\cdot\cdot$ APPS2 の値が範囲内にあるか (`APPS2_CHECK_FLAG`)
     - `t` $\cdot\cdot\cdot$ TPS1 の値が範囲内にあるか (`TPS1_CHECK_FLAG`)
     - `y` $\cdot\cdot\cdot$ TPS2 の値が範囲内にあるか (`TPS2_CHECK_FLAG`)
     - `u` $\cdot\cdot\cdot$ APPS と TPS の値に矛盾がないか（スロットルがターゲットポジションに正確に動いているか） (`TARGET_CHECK_FLAG`)
     - `i` $\cdot\cdot\cdot$ BPS の値が範囲内に存在するか (`BPS_CHECK_FLAG`)
     - `o` $\cdot\cdot\cdot$ BPS と TPS の値が同時に大きくなっていないか（ブレーキを踏んでいてかつスロットルが開いている状況は ☓） (`BPSTPS_CHECK_FLAG`)

   - 以下を入力して、IST コントローラーからのスロットルポジション指令値を使用するかどうかを設定します。（FLAG が`false`のときは APPS、`true`のときは IST からの指令値を使用します）

     - `x` $\cdot\cdot\cdot$ IST コントローラーを用いるかどうか (`IST_CONTROLLER_CHECK_FLAG`)

   - その使用可能なキー

     - `v` $\cdot\cdot\cdot$ ターゲット信号として，キーボード入力を使用するようにする（マニュアルモード）（この設定は保存されず，再起動するともとに戻る）
     - `b` $\cdot\cdot\cdot$ マニュアルモードのターゲット値を`0.1`下げる
     - `n` $\cdot\cdot\cdot$ マニュアルモードのターゲット値を`0.1`上げる

   - 必要であれば以下のキーも使用します。
     - `m` $\cdot\cdot\cdot$ モーターの電源 OFF
     - `z` $\cdot\cdot\cdot$ 再起動

1. `f` を入力すると `---- Calibration Finish ----`、`---- Saved ----` と表示されてキャリブレーションモードが終了します。
   ここで保存されたセンサー値は再起動しても保存されているので、一度キャリブレーションをすれば次回からする必要はありません。

#### キャリブレーションの方法

注：　キャリブレーション中もモーターは動いているので、モーターの電源を切らずにスロットルに指をいれる際は細心の注意を払ってください。最悪指がなくなります。

##### APPS の設定

1. シリアルモニタを起動して、`s`を入力してキャリブレーションモードに入ります。
1. アクセルペダル全閉の状態で`1`、全開の状態で`2`を入力します。
1. `f`を入力して、保存してキャリブレーションモードを抜けます。

##### TPS の設定

TPS の MIN, MAX がだいたいあっている場合は、4 番からスタートしても OK。

1. シリアルモニタを起動して、`s`を入力してキャリブレーションモードに入ります。
1. `m`を入力してモーターの電源を切ります。この状態で手動でスロットルを全開、全閉の位置にセットして、`3`、`4`キーを入力して`TPS_MIN`、`TPS_MAX`をセットします。

   注：　ここで手動でスロットルをセットした場合、スロットルのギアにはバックラッシがあるので、手動でスロットルを動かしてちょうど全閉（or 全開）にしたときのセンサー値とモーターが動いてちょうど全閉（or 全開）になるときのセンサー値はバックラッシ分異なります。なので、手動セットだけでは不十分です。

1. `f`, `s`, `z`と順に入力します。（一度保存してキャリブレーションモードを抜けた後、もう一度キャリブレーションモードに入って再起動）
1. 再起動後、もう一度`s`を押してキャリブレーションモードに入り、`v`を押してマニュアルモードに入ります。この状態で`b`,`n`を押すことでスロットル開度を 0.1%ずつ上げ下げすることができます。`b`,`n`キーでスロットルをちょうど全閉、ちょうど全開に合わせて、それぞれ`3`,`4`キーを入力して`TPS_MIN`、`TPS_MAX`をセットします。
1. もう一度`v`を押してマニュアルモードを抜けたあと、`f`を押して保存，キャリブレーションモードを抜けます。

   注：　キャリブレーション後に設定が反映されているか、PC を繋がずに車両のキルスイッチを入り切りして確認してください。

## ハードウェア

- アクセルペダルポジションセンサー [AS-3](https://www.ipros.jp/product/detail/2000527534/)
- ブレーキ圧センサー [MLH01KPGB06A](https://sps.honeywell.com/jp/ja/products/advanced-sensing-technologies/industrial-sensing/industrial-sensors/industrial-pressure-sensors/mlh-series)
- DC モーター [モータ DCX26L GB KL 12V ギアヘッド GPX26 A 35:1 B7FEDFA2887B](https://www.maxongroup.co.jp/maxon/view/configurator?from=%2Fmaxon%2Fview%2Fcontent%2Fcart&configId=B7FEDFA2887B) or [モータ DCX26L GB KL 12V ギアヘッド GPX26 A 26:1 B8162356240B](https://www.maxongroup.com/camroot/pdf//b8162356240b/b8162356240b_3.pdf)
- DC モータードライバー [G2 ハイパワーモータードライバ 18v17](https://www.pololu.com/product/2991)
- スロットルポジションセンサー [CP-20H](https://www.midori.co.jp/products/potentiometer/angle_sensor/orange_pot/cp-20h) or [CP-3HABS](https://www.midori.co.jp/products/cp-3habs/)
