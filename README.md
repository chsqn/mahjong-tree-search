# mahjong-tree-search

一人麻雀における和了確率とテンパイ確率を計算する.

English version of the README is in preparation.

## 使用方法

### 準備

- cmakeをインストールする.
- C++17以上に対応したコンパイラを用意する.

### ビルド

```shell
$ mkdir build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

### 計算実行
1. 手牌枚数と計算対象に応じたプログラムを選択して実行する(下記例を参照).
2. MPSZ形式で手牌を入力する.
3. 最大ツモ回数を入力する.
4. 結果が表で出力される. Prb列は和了確率, Rdy列はテンパイ確率である. 手牌枚数が14枚の場合のみ, Tile列とDisc列が出力される.それぞれ打牌候補と不要牌(または余剰牌)であるかを表している.


- 手牌枚数: 13枚, 計算対象: 厳密値

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

- 手牌枚数: 13枚, 計算対象: 近似値

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

- 手牌枚数: 14枚, 計算対象: 厳密値

```shell
$ ./calc2-recu
Enter 14 tiles.
12478m111999p11s1z
Enter Range
18

The shanten number is 1

Tile    Disc    Prb     Rdy
1m      1       0.342166        0.866829
2m      0       0.311581        0.837875
4m      1       0.342166        0.866829
7m      0       0.201056        0.701223
8m      0       0.207982        0.701223
1p      0       0.123966        0.545718
9p      0       0.123966        0.545718
1s      0       0.245413        0.766679
1z      1       0.342166        0.866829

Time (msec.)    1
```

- 手牌枚数: 14枚, 計算対象: 近似値

```shell
$ ./calc2-prob
Enter 14 tiles.
12478m111999p11s1z
Enter Range
18

The shanten number is 1

Tile    Disc    Prb     Rdy
1m      1       0.339733        0.866829
2m      0       0.304508        0.835609
4m      1       0.321482        0.866829
7m      0       0.193303        0.69113
8m      0       0.210424        0.711039
1p      0       0.118133        0.537921
9p      0       0.12468 0.542963
1s      0       0.238639        0.760441
1z      1       0.339733        0.866829

Time (msec.)    27
```

## 備考
- 計算対象を近似値とした場合, 途中の打牌選択で和了確率を最大にする牌を捨てるよう選択すべきである. しかし計算コストを抑えるため, 有効牌の残り枚数を最大にする牌をランダムに選択するように実装している.
- Bash on Ubuntu on Windows (Ubuntu 18.04)で動作確認済み. 使用したCMakeのバージョンは3.10.2, コンパイラのバージョンはGCC 7.4.0.
