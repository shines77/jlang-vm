# jlang-vm

## Introduction [English]

A fast virtual machine written by C++.

## 简介 [Chinese]

这是一个仍处于研究中的 `C++` 编写的虚拟机 (虚拟 `CPU`)。

`Windows` 下的工程默认支持 `VS 2015` 及更高版本，如需创建比 `VS 2015` 更旧版本的 `Visual Studio` 工程，请自行使用 `CMake` 创建。代码已在 `Linux` 的 `GCC 4.8.4` 和 `GCC 6.4.0` 下测试并编译通过（2018年12月25日 更新）。

## 参考文章 / Reference Article

本项目是在下面这篇 `知乎` 帖子的启发下编写的，该文讨论的核心内容是：如何降低或减少虚拟机（解释器）中的物理 `CPU` 的流水线停滞问题。

 [`在不使用JIT的前提下，解释器能达到多高的速度呢？`](https://www.zhihu.com/question/300109568)

 ( [https://www.zhihu.com/question/300109568](https://www.zhihu.com/question/300109568) )

## 测试数据 / Benchmark

|测试对象|耗时 (毫秒)|
|:--:|---:|
|jlang-vm (C++, 64bit)|3750 ms|
|Java 1.8 (64bit)|4531 ms|
|C#(Mono, Linux 64bit)|大约 5000 ms|
|AngelScript 2.31.0 (32bit)|10000+ ms|
|Lua 5.x|大约 9000 ms|
|Python 2.7|22000+ ms|

1. `jlang-vm (64bit)` 最快的 `v3` 版本计算 fibonacci(40) 用时约 `3750` 毫秒；

2. `Java 1.8 (64bit)` 关掉 JIT，计算 fibonacci(40) 用时约 `4500` 毫秒；

3. `C#(Mono, Linux 64bit)` 计算 fibonacci(40) 用时约 `5000+` 毫秒，由于是不同系统下测试的结果，耗时为大致估算，详细请看后面的 `C# 版 Fibonacci Test` 小节；

4. `AngelScript 2.31.0 (32bit)` 计算 fibonacci(40) 用时约 `10000+` 毫秒；

5. `Lua 5.4`：在我的 AMD Ryzen 1700X 台式机上测的，用时 9.759 秒，大概相当于 `Intel i5-4310M` 笔记本上 `9000` 毫秒左右；

6. `Python 2.7` 计算 fibonacci(40) 用时约 `22000+` 毫秒；

注：以上数据均为笔记本电脑 `Intel i5-4310M` (`DDR3 1866 MHz`) 上测试的结果。

操作系统：`Windows 10 64-bit`，编译环境：`VC 2015 update 3`。

## 为什么要手动输入 Fibonacci 的参数 n ?

因为很多语言如果不从外部输入，直接把 `n` 写在代码里，会被优化成常量输出，就失去了测试的意义。

所以在所有语言的测试中，都统一使用手动输入参数 `n` 。

## 在 Linux 下编译 jlang-vm / Compile on Linux

1. 先切换到本仓库所在的目录，使用 `CMake` 生成 `makefile`，最后使用 `make` 编译，如下：

    ```shell
    cd {your_jlang-vm_dir}
    cmake .
    make
    ```

2. 运行 `jlang-vm` 测试：

    ```shell
    .\jlang-cm
    ```

## python 版 / python version

1. 在 `Linux` 下：

    ```shell
    cd ./python
    ./run.sh
    ```

2. 在 `Windows` 下：

    ```shell
    cd .\python
    run.bat
    ```

3. 或者手动选择 python 版本，如下：

    ```shell
    cd ./python
    python2 ./fibonacci/fibonacci.py
    python3 ./fibonacci/fibonacci.py
    ```

## Java 版 / Java version

1. 在 `Linux` 下：

    ```shell
    cd ./java/Fibonacci
    ./run.sh
    ```

2. 在 `Windows` 下：

    ```shell
    cd .\java\Fibonacci
    run.bat
    ```

3. 运行效果：

    ```shell
    Input a number (n = 1-40): ? 40

    fibonacci(40) = 102334155

    elapsed time: 4531 ms.
    ```

4. `run.sh` 的内容如下：

    ```bash
    #!/bin/bash
    # 编译源代码
    javac ./src/net/i77soft/algorithm/Program.java

    cd ./src/
    # -Xint 表示禁用 JIT
    java -Xint net.i77soft.algorithm.Program
    ```

5. `JDK 1.8` 的安装方法：

    ```shell
    sudo apt-get install openjdk-8-jdk
    ```

## C# 版 / C# version

由于 Visual Studio 的 `C#` 中找不到有效的关闭 `JIT` 并开启纯解释执行模式的方法。纯解释器模式只有 `Mono`（`C#` 的克隆版）才支持，但 `Mono` 我只在 `Linux` 下安装过，`Windows` 下没尝试过。

1. 如何在 `Linux` 下的 `Mono` 测试 Fibonacci（`Mono` 的安装方法请自行百度）：

    ```shell
    cd ./csharp/Fibonacci
    ./run.sh
    ```

2. `run.sh` 文件的内容如下：

    ```shell
    #!/bin/bash

    mcs ./mono/Fibonacci.cs
    mono --interpreter ./mono/Fibonacci.exe
    ```

由于测试机器的 `CPU` 物理性能跟对比的笔记本不一样，且不是 `Windows` 环境，

不方便对比，但比同机器同系统下的 `Java` 版相比，略慢。
