# fibonacci MSIL 分析

## 1.1 C# 源码

```csharp
public static int fibonacci32(int n)
{
    if (n >= 3)
        return fibonacci32(n - 1) + fibonacci32(n - 2);
    else
        return 1;
}
```

## 1.2. IL 反汇编

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

在线反汇编工具：[https://sharplab.io](https://sharplab.io)

`SharpLab` 项目仓库地址：[https://github.com/ashmind/SharpLab](https://github.com/ashmind/SharpLab)

## 1.3. JIT 反汇编

`JIT Asm` 反汇编：

```csharp
Fibonacci.Program.fibonacci32(Int32)
    L0000: push ebp
    L0001: mov ebp, esp
    L0003: push edi
    L0004: push esi
    L0005: mov esi, ecx
    L0007: cmp esi, 0x3
    L000a: jl L0027
    L000c: lea ecx, [esi-0x1]
    L000f: call dword [0x28d816b0]
    L0015: mov edi, eax
    L0017: dec esi
    L0018: dec esi
    L0019: mov ecx, esi
    L001b: call dword [0x28d816b0]
    L0021: add eax, edi
    L0023: pop esi
    L0024: pop edi
    L0025: pop ebp
    L0026: ret
    L0027: mov eax, 0x1
    L002c: pop esi
    L002d: pop edi
    L002e: pop ebp
    L002f: ret
```
