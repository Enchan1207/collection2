# collection2 {#mainpage}

collection2 (collection of collection) は、コレクションを扱う基本的なデータ構造のC++による実装をまとめたライブラリです。
ヘッダオンリライブラリであるため、ビルドせずに使うことができます。また組込みシステム上での動作を主目的としているため、動的メモリ確保や例外等の機能は使用していません (全てのコレクションの長さはイミュータブルです)。

### Provided data structures

collection2では以下のデータ構造が提供されます。

 - キュー (`collection2::Queue`)
 - スタック (`collection2::Stack`)
 - リングバッファ (`collection2::Buffer`)
 - 連結リスト (`collection2::List`)

### Basic usage

#### Queue

キュー(`collection2::Queue`)は、データを先入れ先出しのリストで保持するデータ構造です。

```cpp
// キューを定義
const int queueLength = 8;
int queueData[queueLength] = {0};
collection2::Queue<int> queue(queueData, queueLength);

// 追加
queue.enqueue(1);
queue.enqueue(2);
queue.enqueue(3);
queue.enqueue(4);
queue.enqueue(5);
queue.enqueue(6);
queue.enqueue(7);
queue.enqueue(8);

// 範囲外
auto result = queue.enqueue(9);
if(result == collection2::OperationResult::Overflow){
    std::cerr << "queue overflow" << std::endl;
}

// 取り出し
for (int i = 0; i < queueLength; i++) {
    int value = 0;
    auto result = queue.dequeue(&value);
    if(result != collection2::OperationResult::Success){
        std::cerr << "An error occurred during dequeue." << std::endl;
        continue;
    }
    std::cout << value << ",";
}
std::cout << std::endl;
```

実行結果:

```sh
1,2,3,4,5,6,7,8,
```

このサンプルコードでは、まず長さ8のキューを定義し、数値をキューに追加します。次に先頭から順に値を取り出し、標準出力に書き出します。

コンストラクタ `collection2::Queue::Queue` の第一引数にはキューが自由に使えるメモリ上の領域、第二引数にはその大きさを指定します。
データ領域の大きさは(`queue_size_t = uint16_t`の範囲内であれば)どのような値でも構いませんが、内部的に使用されるのはその数を上回らない最大の2の冪数となります。  
(例: 長さ15の領域であれば8個、長さ16の領域であれば16個、長さ17の領域であれば16個)  
メモリ効率の観点から、キューに渡すデータ領域のサイズはなるべく2の冪数に合わせる方がよいでしょう。
なお、この制限はバッファ (`collection2::Buffer`) についても適用されます。

データの追加および取り出しは インスタンスメソッド `collection2::Queue::enqueue`, `collection2::Queue::dequeue` により行います。

```cpp
collection2::OperationResult result = queue.enqueue(123);
collection2::OperationResult result = queue.dequeue(&value);
```

操作結果は `collection2::OperationResult` により示されます。キューが満杯の場合は `OperationResult::Overflow`, 空の場合は `OperationResult::Empty` が返り、成功時は `OperationResult::Success` が返ります。  
他のコレクション構造についても `OperationResult` は共通の意味合いで使用されます。

#### Stack

スタック(`collection2::Stack`)は、データを後入れ先出しのリストで保持するデータ構造です。

```cpp
// スタックを定義
const int stackLength = 5;
int stackData[stackLength] = {0};
collection2::Stack<int> stack(stackData, stackLength);

// 追加
stack.enqueue(1);
stack.enqueue(2);
stack.enqueue(3);
stack.enqueue(4);
stack.enqueue(5);

// 範囲外
auto result = stack.enqueue(6);
if(result == collection2::OperationResult::Overflow){
    std::cerr << "stack overflow" << std::endl;
}

// 取り出し
for (int i = 0; i < stackLength; i++) {
    int value = 0;
    auto result = stack.pop(&value);
    if(result != collection2::OperationResult::Success){
        std::cerr << "An error occurred during pop." << std::endl;
        continue;
    }
    std::cout << value << ",";
}
std::cout << std::endl;
```

実行結果:

```sh
5,4,3,2,1,
```

このサンプルコードでは、まず長さ5のスタックを定義し、数値を追加します。次に最後に追加した順に値を取り出し、標準出力に書き出します。

スタックにはキューのようなサイズ制限はありません。コンストラクタ第二引数で指定された数の要素すべてをスタック領域として使用します。  
データの追加・取り出しには `collection2::Stack::push`、`collection2::Stack::pop` を使用します。

#### Buffer

リングバッファ(`collection2::Buffer`)は、データの一時的な記憶に用いるバッファの先頭と末尾をつなげることで 限られたメモリ空間内で柔軟にデータの追加・削除を行えるようにしたデータ構造です。

```cpp
// バッファを定義
const int bufferLength = 4;
int bufferData[bufferLength] = {0};
collection2::Buffer<int> buffer(bufferData, bufferLength);

// 追加
buffer.append(1);
buffer.append(2);
buffer.append(3);
buffer.append(4);

// バッファサイズを超えて追加することが可能
auto result = buffer.append(5);
if(result == collection2::OperationResult::Success){
    std::cerr << "you can add new data to full buffer" << std::endl;
}

// ただし、溢れた分のデータは捨てられる
for (int i = 0; i < stackLength; i++) {
    int value = 0;
    auto result = buffer.pop(&value);
    if(result != collection2::OperationResult::Success){
        std::cerr << "An error occurred during pop." << std::endl;
        continue;
    }
    std::cout << value << ",";
}

std::cout << std::endl;
```

実行結果:

```sh
2,3,4,5,
```

このサンプルコードでは、まず長さ4のバッファを定義し、4つのデータを挿入します。その後、いっぱいになったバッファにさらに5つ目のデータを挿入し、最後にバッファの先頭から値を取り出します。  
リングバッファにおいては、いっぱいになった状態でデータの追加を行うと自動で先頭のデータが押し出され、クリアされます。そのため、`1,2,3,4,5` の順で挿入すると、先頭の `1` が押し出され `2,3,4,5` のみがバッファ内に残ります。

#### List

双方向連結リスト(`collection2::List`)は、各要素が次のデータと前のデータ両方の参照先を保持しているリスト構造です。

```cpp
// リストを定義
const int listLength = 3;
collection2::Node<int> listData[listLength];
collection2::List<int> list(listData, listLength);

// 末尾に追加
list.append(123);

// 位置を指定して追加
list.insert(0, 456);

// 参照
std::cout << *list.get(0) << std::endl;

// 位置を指定して取り出し
int value = 0;
list.remove(0, &value);
std::cout << value << std::endl;

// 末尾から取出し
value = 0;
list.pop(&value);
std::cout << value << std::endl;
```

実行結果:

```sh
456
456
123
```

このサンプルコードでは、まず長さ3のリストを定義し、1つのデータを挿入します。次に位置を指定してもう1つのデータを追加し、リストを破壊せずに先頭にある要素を参照します。  
その後、位置を指定してリスト内のデータを取り出し、最後にリスト末尾からデータを取り出します。

### License

このライブラリはMITライセンスのもとで公開されています。
