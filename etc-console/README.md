# ETC Console

Web Serial API を使ったブラウザベースの ETC (Electronic Throttle Control) モニタリング・設定ツール。

## 技術スタック

- **Preact** + **TypeScript**
- **Vite** (ビルド / dev server)
- **uPlot** (時系列チャート)

## セットアップ

```bash
pnpm install
```

## 開発

```bash
pnpm dev
```

## ビルド

```bash
pnpm build
```

`dist/` に静的ファイルが出力される。

## 機能

- **センサーモニター** — APPS, TPS, ITTR, BPS, Target のリアルタイム表示 (50Hz)
- **時系列チャート** — uPlot による 5 チャンネルのライブグラフ
- **エラーステータス** — 9 種類のプラウシビリティチェックの LED 表示
- **キャリブレーション** — センサー Min/Max 設定、アイドリング設定、手動制御
- **プラウシビリティフラグ** — 個別チェックの有効/無効切り替え
- **設定管理** — デバイスからの取得、JSON エクスポート/インポート
- **デバッグログ** — デバイスからのメッセージ表示

## 対応ブラウザ

Web Serial API が必要。**Chrome** または **Edge** で動作。

## プロトコル

ファームウェアとの通信は NDJSON (改行区切り JSON) で行う。

| 方向 | type | 説明 |
|------|------|------|
| FW → Console | `s` | センサーデータ (50Hz) |
| FW → Console | `d` | デバッグログ |
| Console → FW | `c` | コマンド送信 |
| FW → Console | `r` | コマンドレスポンス |
