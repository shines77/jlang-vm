# jlang_vm

A fast virtual machine written by C++.

## 说明

一个还在测试中的用 `C++` 编写的简单虚拟机 (虚拟 CPU)。工程暂时只支持 VS 2015 及更高。

## 测试数据

* `jlang-vm` 最快的 `v3` 版本计算  `fibonacci(40)` 用时 `3750` 毫秒；

* `Java 1.8 (64bit)` 关掉 `JIT`，计算 `fibonacci(40)` 用时 `4500` 毫秒；

* `AngelScript (32bit)` 上计算  `fibonacci(40)` 用时 `10000+` 毫秒；

* `Lua 5.x` 上没测，但我可以肯定它比 `AngelScript` 要慢一些；

* `Python 2.7` 上计算 `fibonacci(40)` 用时 `22000+` 毫秒；


注：以上测试数据来自 `Intel i5-4310M` (`1866 DDR3`) 笔记本，操作系统 `Windows 10 64-bit`。
