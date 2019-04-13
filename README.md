# jlang-vm

A fast virtual machine written by C++.

## 简介

这是一个仍处于研究中的 `C++` 编写的虚拟机 (虚拟 `CPU`)。

`Windows` 下的工程默认支持 `VS 2015` 及更高版本，如需创建比 `VS 2015` 更旧版本的 `Visual Studio` 工程，请自行使用 `CMake` 创建。项目已在 `Linux` 的 `GCC 4.8.4` 和 `GCC 6.4.0` 下测试并编译通过（2018年12月25日 更新）。

## 在 Linux 下编译 jlang-vm

1. 先切换到本仓库所在的根路径，例如：

```shell
cd /home/git/jlang-vm
```

2. 然后使用 `CMake` 生成 `makefile`，命令如下：

```shell
cmake -G "Unix Makefiles"
```

或者

```shell
cmake ./
```

3. 最后使用 `make` 命令编译工程：

```shell
make
```

## Java 版 Fibonacci Test

1. 在 `Windows` 下编译并运行：

    ```shell
    cd .\java\Fibonacci
    run.bat
    ```

2. 在 `Linux` 下编译并运行：

    ```shell
    cd ./java/Fibonacci
    ./run.sh
    ```

    以上命令运行后的效果如下：

    ```shell
    Input a number (1-50): ? 40

    fibonacci(40) = 102334155

    elapsed time: 27154 ms.
    ```

## 测试数据

1. `jlang-vm (64bit)` 最快的 `v3` 版本计算 fibonacci(40) 用时 `3750` 毫秒；

2. `Java 1.8 (64bit)` 关掉 JIT，计算 fibonacci(40) 用时 `4500` 毫秒；

3. `AngelScript 2.31.0 (32bit)` 计算 fibonacci(40) 用时 `10000+` 毫秒；

4. `Lua 5.x` 没测，但用时肯定超过 `AngelScript`，有兴趣的朋友可以测一下；

5. `Python 2.7` 计算 fibonacci(40) 用时 `22000+` 毫秒；

注：以上测试数据来自笔记本 `Intel i5-4310M` (`DDR3 1866 MHz`)，操作系统 `Windows 10 64-bit`。

## 参考文章

本代码是在这篇 `知乎` 文章的启发下编写的，该文讨论的核心内容是：如何降低或减少虚拟机（解释器）中的物理 `CPU` 的流水线停滞问题。

 [`在不使用JIT的前提下，解释器能达到多高的速度呢？`](https://www.zhihu.com/question/300109568)
 
 ( [https://www.zhihu.com/question/300109568](https://www.zhihu.com/question/300109568) )
