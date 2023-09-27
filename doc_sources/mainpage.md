# Home {#mainpage}

\tableofcontents{html}

collection2 (collection of collection) は、コレクションを扱う基本的なデータ構造をまとめたライブラリです。

### Features

 - ヘッダオンリーライブラリであるため、ライブラリ自体のビルドを必要としません。
 - 小規模組込みシステム上での動作に特化しています:
    - C++11でコンパイルしています。
    - 動的メモリ割当て (`malloc`, `new` 等) は行っていません。データ領域はコンストラクタで指定します。
    - 例外 (`try`, `throw`) を使用していません。コレクションに対する操作結果は `collection2::OperationResult` で返ります。
    - STLに依存しません。単体でコンパイル可能です。
    - **全てのテストケースがAVRマイコン(QEMU)でパスすることを確認しています。**

### Provided data structures

collection2が提供するデータ構造は以下の通りです。

 - キュー (`collection2::Queue`)
 - スタック (`collection2::Stack`)
 - リングバッファ (`collection2::Buffer`)
 - 連結リスト (`collection2::List`)
 - ツリー (`collection2::Tree`)

### Installation

現行バージョンでは以下のインストール方法をサポートしています:

 - ホストマシン上でビルドする
 - 既存のCMakeプロジェクトに依存関係として追加する

\note 構成およびインストールには [CMake](https://cmake.org/) が必要です。

#### 1. マシンに直接インストールする

プロジェクトルートで以下のコマンドを実行します:

```sh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --install .
```

この手順により、collection2が `/usr/local` 以下にインストールされます。


#### 2. 既存のCMakeプロジェクトに追加する

ホストマシンに直接インストールせず、既存のCMakeプロジェクトに依存関係としてインストールすることも可能です。  
既存プロジェクトの `CMakeLists.txt` に以下の記述を追加します:

```CMake
# Fetch library
include(FetchContent)
FetchContent_Declare(
    collection2
    GIT_REPOSITORY https://github.com/Enchan1207/collection2
    GIT_TAG v0.3.0
)
FetchContent_MakeAvailable(collection2)
```

次に、collection2を使用するターゲットに以下の記述を追加します:

```CMake
# Include directories are automatically set when linking
target_link_libraries(main PRIVATE
    collection2 
)
```

以上で完了です。

### Usage

各クラスの詳細な使用法については [API References](@ref apirefs) を参照してください。  
ソースファイルから自動生成されたドキュメントは <a href="annotated.html">**Classes**</a> より参照できます。

### Author, License

Author: [Enchan](https://github.com/Enchan1207)  
License: [MIT](https://opensource.org/license/mit/)
