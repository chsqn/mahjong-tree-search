# mahjong-tree-search

一人麻雀における和了確率とテンパイ確率を計算する.

[Read this in English.](README.md)

## 使用方法

### 準備

- cmakeをインストールします.
- C++17以上に対応したコンパイラを用意します.

### ビルド
- デバッグモード
```shell
$ mkdir build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

- リリースモード
```shell
$ mkdir build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

### 計算
1. 手牌枚数と計算方法に応じたプログラムを実行します.
2. MPSZ形式で手牌を入力します.
3. 最大ツモ回数を入力します.
4. 結果が表で出力される. Prb列は和了確率, Rdy列はテンパイ確率です. 手牌枚数が3*N*+2(*N*は自然数)枚の場合のみ, Tile列とDisc列(どの牌が不要牌かを表す)が出力されます.

> **NOTE**: シャンテン数を増加させる牌を捨てた場合の和了確率とテンパイ確率は計算されません.

- 手牌枚数: 13枚, 計算方法: 厳密

```shell
$ ./calc1-recu
Enter 13 tiles.
2478m111999p11s1z
Enter Range
18

The shanten number is 1

Prb     Rdy
0.338418        0.864314

Time (msec.)    0
```

- 手牌枚数: 13枚, 計算方法: モンテカルロ

```shell
$ ./calc1-prob
Enter 13 tiles.
2478m111999p11s1z
Enter Range
18

The shanten number is 1

Prb     Rdy
0.325134        0.864314

Time (msec.)    0
```

- 手牌枚数: 14枚, 計算対象: 厳密

```shell
Enter 14 tiles.
12478m111999p11s1z
Enter Range
18

The shanten number is 1

Tile    Disc    Prb     Rdy
1m      1       0.342166        0.866829
2m      0       0       0
4m      1       0.342166        0.866829
7m      0       0       0
8m      0       0       0
1p      0       0       0
9p      0       0       0
1s      0       0       0
1z      1       0.342166        0.866829

Time (msec.)    0
```

- 手牌枚数: 14枚, 計算対象: モンテカルロ

```shell
Enter 14 tiles.
12478m111999p11s1z
Enter Range
18

The shanten number is 1

Tile    Disc    Prb     Rdy
1m      1       0.336082        0.866829
2m      0       0       0
4m      1       0.336082        0.866829
7m      0       0       0
8m      0       0       0
1p      0       0       0
9p      0       0       0
1s      0       0       0
1z      1       0.332432        0.866829

Time (msec.)    0
```

### 三人麻雀モード
`THREE_PLAYER`を有効にします.

例:
```
$ cmake .. -DCMAKE_BUILD_TYPE=Release -DTHREE_PLAYER=on
```

## 備考
- Bash on Ubuntu on Windows (Ubuntu 18.04)で動作を確認しています. 使用したCMakeのバージョンは3.10.2, コンパイラのバージョンはGCC 7.4.0です.
