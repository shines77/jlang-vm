
#ifndef JLANG_VM_INTERPRETER_V2_H
#define JLANG_VM_INTERPRETER_V2_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//////////////////////////////////////////////////////////////

#undef FRAME_STACK_SIZEOF
#undef USE_FORWARD_STACK_PTR

/* If is forward stack pointer ? */
#define USE_FORWARD_STACK_PTR   1

#define FRAME_STACK_SIZEOF      ((sizeof(void *) * 2) / sizeof(uint32_t))

//////////////////////////////////////////////////////////////

#include "jlang/vm/ArgsDefine.h"

#define JLANG_VM_ARGS_DEFINE_H

#include "jlang/vm/Interpreter.h"
#include "jlang/vm/Interpreter_v3.h"
#include "jlang/lang/ErrorCode.h"
#include "jlang/support/Console.h"

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include <list>
#include <memory>
#include <atomic>

using namespace std;

namespace jlang {
namespace v2 {

//
// 00000000:    push_u32 0x00000014 (int32)
// 00000005:    call 0x00000010 (short offset 0x0008)
// 00000008:    pop_u32
// 00000009:    ret

// 0000000A:    nop; nop; nop;

// 00000010:    cmp_imm_u32 arg0, 0x00000003
// 00000016:    jl_short 0x00000030 (short offset 0x0017)

// 00000019:    push_u32 0x00000000 (int32)
// 0000001C:    push arg0  (var1)
// 0000001A:    dec var1
// 0000001E:    call 0x00000010 (short offset 0xFFEF)

// 00000021:    copy_eax var0, eax
// 00000023:    dec var1
// 00000025:    call 0x00000010 (short offset 0xFFE8)

// 00000028:    add eax, var0
// 0000002A:    pop_u32  (var1)
// 0000002B:    pop_u32  (var0)
// 0000002C:    ret

// 0000002D:    nop; nop; nop;

// 00000030:    load eax, 0x00000001 (uint32)
// 00000035:    ret
//

static const unsigned char fibonacciBinary32[] = {
    // 00000000:    push_u32 0x00000014 (int32)
    OpCode::push_u32, 0x14, 0x00, 0x00, 0x00,
    // 00000005:    call 0x00000010 (short offset 0x0008)
    OpCode::call_short, 0x08, 0x00,
    // 00000008:    pop_u32
    OpCode::pop_u32,
    // 00000009:    ret
    OpCode::ret,

    // 0000000A:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,
    // 0000000D:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,

    // 00000010:    cmp_imm_u32 arg0, 0x00000003
    OpCode::cmp_imm_u32, __arg0, 0x03, 0x00, 0x00, 0x00,
    // 00000016:    jl_short 0x00000030 (short offset 0x0017)
    OpCode::jl_short, 0x17, 0x00,

    // 00000019:    push_u32 0x00000000 (int32)
    OpCode::push_u32_0,
    // 0000001A:    push arg0  (var1)
    OpCode::push, __arg0,
    // 0000001C:    dec var1
    OpCode::dec,  __var1,
    // 0000001E:    call 0x00000010 (short offset 0xFFEF)
    OpCode::call_short, 0xEF, 0xFF,

    // 00000021:    copy_from var0, eax
    OpCode::copy_from_eax, __var0,
    // 00000023:    dec var1
    OpCode::dec,  __var1,
    // 00000025:    call 0x00000010 (short offset 0xFFE8)
    OpCode::call_short, 0xE8, 0xFF,

    // 00000028:    add eax, var0
    OpCode::add_eax, __var0,
    // 0000002A:    pop_u32  (var1)
    OpCode::pop_u32,
    // 0000002B:    pop_u32  (var0)
    OpCode::pop_u32,
    // 0000002C:    ret
    OpCode::ret,

    // 0000002D:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,

    // 00000030:    load eax, 0x00000001 (uint32)
    OpCode::load_eax, 0x01, 0x00, 0x00, 0x00,
    // 00000035:    ret
    OpCode::ret,

    // 00000036:    nop; nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000003B:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000003F:    exit
    OpCode::exit
};

//
// 00000000:    push_u32 0x00000014 (int32)
// 00000005:    call 0x00000010 (short offset 0x0008)
// 00000008:    pop_u32
// 00000009:    ret

// 0000000A:    nop; nop; nop;
// 0000000D:    nop; nop; nop;

// 00000010:    push_u32 0x00000000 (int32)
// 00000011:    cmp_imm_u32 arg0, 0x00000003
// 00000017:    jl_short 0x00000030 (short offset 0x0016)

// 0000001A:    dec arg0
// 0000001C:    push arg0  (var1)
// 0000001E:    call 0x00000010 (short offset 0xFFEF)

// 00000021:    copy_from var0, eax
// 00000023:    dec var1
// 00000025:    call 0x00000010 (short offset 0xFFE8)

// 00000028:    pop_u32  (var1)
// 00000029:    add eax, var0
// 0000002B:    pop_u32  (var0)
// 0000002C:    ret

// 0000002D:    nop; nop; nop;

// 00000030:    load eax, 0x00000001 (uint32)
// 00000035:    pop_u32  (var0)
// 00000036:    ret
//

static const unsigned char fibonacciBinary32_old[] = {
    // 00000000:    push_u32 0x00000014 (int32)
    OpCode::push_u32, 0x14, 0x00, 0x00, 0x00,
    // 00000005:    call 0x00000010 (short offset 0x0008)
    OpCode::call_short, 0x08, 0x00,
    // 00000008:    pop_u32
    OpCode::pop_u32,
    // 00000009:    ret
    OpCode::ret,

    // 0000000A:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,
    // 0000000D:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,

    // 00000010:    push_u32 0x00000000 (int32)
    OpCode::push_u32_0,
    // 00000011:    cmp_imm_u32 arg0, 0x00000003
    OpCode::cmp_imm_u32, __arg0, 0x03, 0x00, 0x00, 0x00,
    // 00000017:    jl_short 0x00000030 (short offset 0x0016)
    OpCode::jl_short, 0x16, 0x00,

    // 0000001A:    dec arg0
    OpCode::dec,  __arg0,
    // 0000001C:    push arg0  (var1)
    OpCode::push, __arg0,
    // 0000001E:    call 0x00000010 (short offset 0xFFEF)
    OpCode::call_short, 0xEF, 0xFF,

    // 00000021:    copy_eax var0, eax
    OpCode::copy_from_eax, __var0,
    // 00000023:    dec var1
    OpCode::dec,  __var1,
    // 00000025:    call 0x00000010 (short offset 0xFFE8)
    OpCode::call_short, 0xE8, 0xFF,

    // 00000028:    pop_u32  (var1)
    OpCode::pop_u32,
    // 00000029:    add eax, var0
    OpCode::add_eax, __var0,
    // 0000002B:    pop_u32  (var0)
    OpCode::pop_u32,
    // 0000002C:    ret
    OpCode::ret,

    // 0000002D:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,

    // 00000030:    load eax, 0x00000001 (uint32)
    OpCode::load_eax, 0x01, 0x00, 0x00, 0x00,
    // 00000035:    pop_u32 (var0)
    OpCode::pop_u32,
    // 00000036:    ret
    OpCode::ret,

    // 00000037:    nop; nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000003C:    exit
    OpCode::exit
};

class vmBinaryFile {
private:
    vmBinImage image_;

public:
    vmBinaryFile() {}
    ~vmBinaryFile() {}

    int loadFromFile(const char * filename) {
        static const size_t kImageSize = sizeof(fibonacciBinary32);
        image_.allocate(kImageSize);
        void * imageData = image_.data();
        if (imageData) {
            memcpy(imageData, (const void *)&fibonacciBinary32[0], kImageSize);
        }
        image_.setEntryOffset(0);
        return 1;
    }

    int saveToFile(const char * filename) {
        return 1;
    }

    void setInput(uintptr_t initValue) {
        char * imageData = (char *)image_.data();
        uint32_t * pInitValue = (uint32_t *)&(imageData[1]);
        if (pInitValue) {
            *pInitValue = (uint32_t)initValue;
        }
    }

    void * getImagePtr() const {
        return image_.data();
    }

    size_t getImageSize() const {
        return image_.size();
    }

    void * getImageEntry() const {
        return image_.entry();
    }
};

template <typename BasicType = uintptr_t>
class ExecutionEngine {
public:
    typedef BasicType                           basic_type;
    typedef size_t                              size_type;
    typedef v3::ExecutionContext<basic_type>    context_type;
    typedef vmReturn<basic_type>                return_type;
    typedef ExecutionEngine<basic_type>         this_type;

private:
    vmBinaryFile    binary_;
    context_type    context_;

public:
    ExecutionEngine() {}
    virtual ~ExecutionEngine() {
        destroy();
    }

    bool isInited() const { return (context_.getId() != 0); }

    int create() {
        int ec = binary_.loadFromFile("test.bin");
        if (ec <= 0) {
            return ErrorCode::BinaryFile_Read_Failed;
        }

        context_.setImageInfo(binary_.getImagePtr(), binary_.getImageSize(),
                              binary_.getImageEntry());

        bool success = createContext();
        if (!success) {
            return ErrorCode::MainProcess_Create_Failed;
        }

        return (int)success;
    }

    void destroy() {
        destroyContext();
    }

    bool createContext() {
        if (!context_.isInited()) {
            context_.create();
        }

        return context_.isInited();
    }

    void destroyContext() {
        if (context_.isInited()) {
            context_.destroy();
        }
    }

    int run(return_type & ret) {
        binary_.setInput(ret.getValue());
        int ec = context_.run(ret);
        return ec;
    }
};

template <typename BasicType = uintptr_t>
class Interpreter {
public:
    typedef BasicType                   basic_type;
    typedef ExecutionEngine<basic_type> engine_type;
    typedef vmReturn<basic_type>        return_type;
    typedef Interpreter<basic_type>     this_type;

private:
    engine_type engine_;

public:
    Interpreter() {}
    ~Interpreter() {}

    int create() {
        int ec = engine_.create();
        return ec;
    }

    int run(return_type & ret) {
        int ec = engine_.run(ret);
        return ec;
    }
};

} // namespace v2
} // namespace jlang

#undef JLANG_VM_ARGS_DEFINE_H

#undef __arg0 
#undef __arg1 
#undef __arg2 
#undef __arg3 
#undef __arg4 
#undef __arg5 
#undef __arg6 
#undef __arg7 
#undef __arg8 
#undef __arg9 

#undef __var0 
#undef __var1 
#undef __var2 
#undef __var3 
#undef __var4 
#undef __var5 
#undef __var6 
#undef __var7 
#undef __var8 
#undef __var9 
#undef __var10
#undef __var11
#undef __var12
#undef __var13
#undef __var14
#undef __var15

#endif // JLANG_VM_INTERPRETER_V2_H
