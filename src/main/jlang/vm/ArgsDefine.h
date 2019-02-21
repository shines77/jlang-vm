
#ifndef JLANG_VM_ARGS_DEFINE_H

//////////////////////////////////////////////////////////////

/* If is forward stack pointer ? */
#ifndef USE_FORWARD_STACK_PTR
#define USE_FORWARD_STACK_PTR   1
#endif

#ifndef FRAME_STACK_SIZEOF
#define FRAME_STACK_SIZEOF      ((sizeof(void *) * 2) / sizeof(uint32_t))
#endif

//////////////////////////////////////////////////////////////

#if USE_FORWARD_STACK_PTR

#define __arg0  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 1))
#define __arg1  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 2))
#define __arg2  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 3))
#define __arg3  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 4))
#define __arg4  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 5))
#define __arg5  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 6))
#define __arg6  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 7))
#define __arg7  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 8))
#define __arg8  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 9))
#define __arg9  ((uint8_t)(0 - FRAME_STACK_SIZEOF - 10))

#define __var0  ((uint8_t)(0))
#define __var1  ((uint8_t)(1))
#define __var2  ((uint8_t)(2))
#define __var3  ((uint8_t)(3))
#define __var4  ((uint8_t)(4))
#define __var5  ((uint8_t)(5))
#define __var6  ((uint8_t)(6))
#define __var7  ((uint8_t)(7))
#define __var8  ((uint8_t)(8))
#define __var9  ((uint8_t)(9))
#define __var10 ((uint8_t)(10))
#define __var11 ((uint8_t)(11))
#define __var12 ((uint8_t)(12))
#define __var13 ((uint8_t)(13))
#define __var14 ((uint8_t)(14))
#define __var15 ((uint8_t)(15))

#else

#define __arg0  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 0))
#define __arg1  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 1))
#define __arg2  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 2))
#define __arg3  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 3))
#define __arg4  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 4))
#define __arg5  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 5))
#define __arg6  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 6))
#define __arg7  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 7))
#define __arg8  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 8))
#define __arg9  ((uint8_t)(0 + FRAME_STACK_SIZEOF + 9))

#define __var0  ((uint8_t)(0 - 1))
#define __var1  ((uint8_t)(0 - 2))
#define __var2  ((uint8_t)(0 - 3))
#define __var3  ((uint8_t)(0 - 4))
#define __var4  ((uint8_t)(0 - 5))
#define __var5  ((uint8_t)(0 - 6))
#define __var6  ((uint8_t)(0 - 7))
#define __var7  ((uint8_t)(0 - 8))
#define __var8  ((uint8_t)(0 - 9))
#define __var9  ((uint8_t)(0 - 10))
#define __var10 ((uint8_t)(0 - 11))
#define __var11 ((uint8_t)(0 - 12))
#define __var12 ((uint8_t)(0 - 13))
#define __var13 ((uint8_t)(0 - 14))
#define __var14 ((uint8_t)(0 - 15))
#define __var15 ((uint8_t)(0 - 16))

#endif // USE_FORWARD_STACK_PTR

//////////////////////////////////////////////////////////////

#endif // JLANG_VM_ARGS_DEFINE_H
