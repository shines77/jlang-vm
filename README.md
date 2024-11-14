# jlang-vm

## Introduction [English]

A virtual machine (virtual CPU) that is still under research and is faster than Java and C# virtual machines.

## 简介 [Chinese]

这是一个仍处于研究中的比 Java、C# 虚拟机更快的虚拟机 (虚拟 `CPU`)。

`Windows` 下的工程默认支持 `VS 2015` 及更高版本，如需创建比 `VS 2015` 更旧版本的 `Visual Studio` 工程，请自行使用 `CMake` 创建。代码已在 `Linux` 的 `GCC 4.8.4` 和 `GCC 6.4.0` 下测试并编译通过（2018年12月25日 更新）。

## 起因 / Cause

本项目是在下面这篇 `知乎` 帖子的启发下编写的，该文讨论的核心内容是：如何降低或减少虚拟机（解释器）中的物理 `CPU` 的流水线停滞问题。

 [`在不使用JIT的前提下，解释器能达到多高的速度呢？`](https://www.zhihu.com/question/300109568)

 ( [https://www.zhihu.com/question/300109568](https://www.zhihu.com/question/300109568) )

## 优化思路 / Optimization approach

上面的帖子中有一个回答者 [方泽图](https://www.zhihu.com/people/fang-ze-tu) 的回答中提到 “`pipeline stall`” (流水线停滞) 的问题，启发了我。几乎所有的 `解释器` (interpreter) 都会使用 switch 语句来做 `指令` (op_code) 的分发 (dispatch)，这就会涉及 `间接跳转` (indirect branch)，会造成分支难以预测 (branch prediction)，预测失败后会造成 `pipeline stall` (流水线停滞)，会清掉预测，重新解码，例如：分支预测惩罚是 13 个指令周期 (每个 CPU 不一样)。

优化的思路是：

- 使用 register 关键字暗示编译器，尽量让 VM 虚拟机的寄存器内存隐射到物理 CPU 的寄存器上，32 位下的寄存器较少，64 位下的寄存器多一些，这个隐射不能保证 100% 成功，编译器会根据实际情况决定；
- 尽量合并 `op_code` 的功能，变成 `宏指令`，减少跳转的次数。尽量让一个 `op_code` 执行更多的功能，比如以下指令：cmp eax, 0x04; jne xxxx; 可以合并为：cmp_jne eax, 0x04, xxxx ；（由于是实验性的，这个设想只实现了一部分，例如：ret_eax, 0x01; 给 eax 赋值为1，并返回；ret_n 0x08; 返回并退栈 8 个字节。）
- 对于较为常用的寄存器，不需要像硬件指令那样，在指令中的 bit 位中解析出是 eax 或 ebx，直接使用：move_to_eax, r0 或 move_to_eax, 0x05 这样的指令；
- 所有 op_code_xxxx() 函数声明为 force_inline，减少不必要的跳转，但缺点是 switch 循环的代码会变大；

此外，我还做一个尝试，尝试把 Fibonacci(N) 的 op_code 写成一个 inline 的版本，即整个递归都在一个 do while() 循环里，中间的跳转也写成 inline 的版本，让编译器去编译成一体成型，没有 switch 跳转的纯虚拟机代码，有点类似于 JIT (Just In Time) 技术。具体代码可以参阅：[/src/main/jlang/vm/Interpreter_v4.h](blob/master/src/main/jlang/vm/Interpreter_v4.h) 中的 execute_inline() 方法。

几年后，我研究了 `WebAssembly` 技术，发现跟它的纯虚拟机 JIT 技术类似，效率也差不多。不过由于是由编译器来完成的 inline，速度还是稍微要差一点，但量级是相当的，比 `WebAssembly` 技术慢约 30-40 %左右，但性能是纯 switch 的常规虚拟机版本的 300-350 % 。

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
