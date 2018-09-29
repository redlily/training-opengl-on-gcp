# Server 3D renderer with EGL and OpenGL.

<img src='image/rendering-10-40--10.png' />

EGLとOpenGLを使用してX Window Systemを使用しないサーバ・レンダリング。

## 概要

GPU搭載のLinuxサーバ上で3Dレンダリングを実現するための最小のシステム構成例と最小の3D描画のための実装を提供します。

## ディレクトリ構成

- main.cpp  
サーバ・レンダリングを実際に行うためのC++用のソースコード。
- math_utils.hpp  
3Dのための数学系ユーティリティの処理をまとめたC++用のヘッダファイル。
- model.h  
3Dモデルの情報を焼き込んだC++用のヘッダファイル。
- PC01_Kohaku.png  
3Dモデル用のテクスチャイメージ。
- shader.vs, shader.fs  
描画用の頂点シェーダとフラグメントシェーダ (ピクセルシェーダ)。
- image  
レンダリングされた画像を保存するためのディレクトリ、apache, nginx等でレンダリングサーバを作る場合は `chmod 0777` 等で権限を与えておいてください。
- index.php  
レンダリングプログラムをWeb APIとして動作させるためのPHP用のサンプルプログラム。

## インストール・パッケージ

- NVIDIAのGPUドライバ  
EGLを動作させるためのパッケージ、インストール手順はOSやハードウェア構成によって違うのでNVIDIAの公式ページを参照。
- EGL  
ハードウェアとOpenGLをつなげるためのAPI。
- OpenGL ES 2.0  
3D描画を行うためのAPI、その中でも組み込み系に特化したパッケージ。
- g++  
C/C++コンパイラ。
- png++  
PNGの読み書き用のライブラリ。

## 環境構築例

### Ubuntu 16.04

#### 環境

- CPU vCPU x 1
- メモリ 3.75GB
- GPU NVIDIA tesla K80 x 1
- OS Ubuntu 16.04 Minimal

#### インストール

```bash
# NVIDIAのGPUドライバ用のリポジトリ追加

$ wget http://us.download.nvidia.com/tesla/396.44/nvidia-diag-driver-local-repo-ubuntu1604-396.44_1.0-1_amd64.deb
$ dpkg -i nvidia-diag-driver-local-repo-ubuntu1604-396.44_1.0-1_amd64.deb
$ apt-key add /var/nvidia-diag-driver-local-repo-396.44/7fa2af80.pub
$ apt-get update
$ rm nvidia-diag-driver-local-repo-ubuntu1604-396.44_1.0-1_amd64.deb

# GPUドライバインストール

$ apt-get install cuda-drivers

# 各種必要なパッケージのインストール

$ apt-get install g++
$ apt-get install libpng++-dev
$ apt-get install libgles2-mesa-dev libegl1-mesa-dev
$ apt-get install apache2
```

### CentOS 7

追加予定

## レンダラーのコンパイルと実行

### コンパイル

```
$ g++ -o main main.cpp -lEGL -lGLESv2 -lpng
```

### 実行

```
$ ./main ${x軸の回転角} ${y軸の回転角} ${z軸の回転角}
```

実際にレンダリングされた画像はimageディレクトリに `image/rendering-${x}-${y}-${z}.png` の形式で保存されます。

## 参考文献

https://devblogs.nvidia.com/egl-eye-opengl-visualization-without-x-server/

X Serverを使用しないEGLによるOpenGLの初期化と、その他にここで触れていないGPUクラスタでのEGLの初期化例が書かれています。

## その他

これは実験用のリポジトリなので不要になったら消す。
