# fibonacci MSIL 分析

C# 源码：

```csharp
public static int fibonacci32(int n)
{
    if (n >= 3)
        return fibonacci32(n - 1) + fibonacci32(n - 2);
    else
        return 1;
}
```

`MSIL` 反汇编：

```csharp
    .method public hidebysig static 
        int32 fibonacci32 (
            int32 n
        ) cil managed
    {
        // Method begins at RVA 0x208c
        // Code size 24 (0x18)
        .maxstack 8

        IL_0000: ldarg.0
        IL_0001: ldc.i4.3
        IL_0002: blt.s IL_0016

        IL_0004: ldarg.0
        IL_0005: ldc.i4.1
        IL_0006: sub
        IL_0007: call int32 Fibonacci.Program::fibonacci32(int32)
        IL_000c: ldarg.0
        IL_000d: ldc.i4.2
        IL_000e: sub
        IL_000f: call int32 Fibonacci.Program::fibonacci32(int32)
        IL_0014: add
        IL_0015: ret

        IL_0016: ldc.i4.1
        IL_0017: ret
    } // end of method Program::fibonacci32
```
