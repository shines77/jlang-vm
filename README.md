# jlang_vm

A fast virtual machine written by C++.

## 说明

一个还在测试中的用 `C++` 编写的简单虚拟机(虚拟CPU)。

## 测试与比较

* 在我的笔记本上，最快的 `v3` 版本 `fibonacci(40)` 用时 `3750` 毫秒；

* `Java 1.8` 关掉 `JIT`，`fibonacci(40)` 用时 `4500` 毫秒；

* `AngelScript` 上 `fibonacci(40)` 用时 `10000+` 毫秒；

* `Lua 5.x` 上没测，但我可以肯定它比 `AngelScript` 要慢一些；

* `Python 2.7` 上 `fibonacci(40)` 用时 `22000+` 毫秒；
