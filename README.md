# jlang_vm

A fast virtual machine written by C++.

## 说明

一个还在测试中的用 `C++` 编写的简单虚拟机 (虚拟 CPU)。工程暂时只支持 `VS 2015` 及更高。`Linux` 版稍后再整理，目前只能在 `Windows` 上编译。

## 测试数据

1. `jlang-vm (64bit)` 最快的 `v3` 版本计算 fibonacci(40) 用时 `3750` 毫秒；

2. `Java 1.8 (64bit)` 关掉 JIT，计算 fibonacci(40) 用时 `4500` 毫秒；

3. `AngelScript 2.31.0 (32bit)` 计算 fibonacci(40) 用时 `10000+` 毫秒；

4. `Lua 5.x` 没测，但用时肯定超过 `AngelScript`，有兴趣的朋友可以测一下；

5. `Python 2.7` 计算 fibonacci(40) 用时 `22000+` 毫秒；

注：以上测试数据来自笔记本 `Intel i5-4310M` (`DDR3 1866 MHz`)，操作系统 `Windows 10 64-bit`。

## 参考文献

本代码是在这篇 `知乎` 文章的启发下编写的，该文讨论的核心内容是：如何降低或减少虚拟机（解释器）中的物理 `CPU` 的流水线停滞问题。

 [`在不使用JIT的前提下，解释器能达到多高的速度呢？`](https://www.zhihu.com/question/300109568) ( [https://www.zhihu.com/question/300109568](https://www.zhihu.com/question/300109568) )
