
#ifndef JLANG_VM_INTERPRETER_V4_H
#define JLANG_VM_INTERPRETER_V4_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//////////////////////////////////////////////////////////////

#undef FRAME_STACK_SIZEOF
#undef USE_FORWARD_STACK_PTR

/* If is forward stack pointer ? */
#define USE_FORWARD_STACK_PTR   1

#define FRAME_STACK_SIZEOF      ((sizeof(void *) * 1) / sizeof(uint32_t))

//////////////////////////////////////////////////////////////

#include "jlang/vm/ArgsDefine.h"

#define JLANG_VM_ARGS_DEFINE_H

#include "jlang/vm/Interpreter.h"
#include "jlang/vm/Interpreter_v3.h"
#include "jlang/lang/ErrorCode.h"

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include <list>
#include <memory>
#include <atomic>

//////////////////////////////////////////////////////////////

using namespace std;

namespace jlang {
namespace v4 {

//
// 00000000:    store var0, 0x00000014 (int32)
// 00000006:    fast_call 0x00000010, 8 (short offset 0x0005, local_size = 8)
// 0000000B:    ret_n 8

// 0000000E:    nop; nop;

// 00000010:    cmp_imm_u32 arg1, 0x00000003
// 00000016:    jl_near 0x00000030 (near offset 0x0018)

// 00000018:    move var0, arg1
// 0000001B:    dec var0
// 0000001D:    fast_call 0x00000010, 8 (short offset 0xFFEE, local_size = 8)

// 00000022:    copy_from var1, eax
// 00000024:    dec var0
// 00000026:    fast_call 0x00000010, 8 (short offset 0xFFE5, local_size = 8)

// 0000002B:    add eax, var1
// 0000002D:    ret_n 8  (pop var0, var1)

// 00000030:    ret_eax_n, 8, 0x00000001 (uint32)
//

static const unsigned char fibonacciBinary32[] = {
    // 00000000:    store var0, 0x00000014 (int32)
    OpCode::store, __var0, 0x14, 0x00, 0x00, 0x00,
    // 00000006:    call (0x00000010, 8) (short offset 0x0005, local_size = 8)
    OpCode::fast_call_short, 0x05, 0x00, 0x08, 0x00,
    // 0000000B:    ret_n 8
    OpCode::ret_n, 0x08, 0x00,

    // 0000000E:    nop; nop;
    OpCode::nop,  OpCode::nop,

    // 00000010:    cmp_imm_u32 arg1, 0x00000003
    OpCode::cmp_imm_u32, __arg1, 0x03, 0x00, 0x00, 0x00,
    // 00000016:    jl_near 0x00000030 (near offset 0x18)
    OpCode::jl_near, 0x18,

    // 00000018:    move var0, arg1
    OpCode::move, __var0, __arg1,
    // 0000001B:    dec var0
    OpCode::dec,  __var0,
    // 0000001D:    fast_call (0x00000010, 8) (short offset 0xFFEE, local_size = 8)
    OpCode::fast_call_short, 0xEE, 0xFF, 0x08, 0x00,

    // 00000022:    copy_from var1, eax
    OpCode::copy_from_eax, __var1,
    // 00000024:    dec var0
    OpCode::dec,  __var0,
    // 00000026:    fast_call (0x00000010, 8) (short offset 0xFFE5, local_size = 8)
    OpCode::fast_call_short, 0xE5, 0xFF, 0x08, 0x00,

    // 0000002B:    add eax, var1
    OpCode::add_eax, __var1,
    // 0000002D:    ret_n 8
    OpCode::ret_n,  0x08, 0x00,

    // 00000030:    ret eax, 0x00000001 (uint32)
    OpCode::ret_eax_n, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00,

    // 00000037:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,
    // 0000003A:    nop; nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000003F:    exit
    OpCode::exit
};

typedef v3::ForwardPtr  vmImagePtr;

#if USE_FORWARD_STACK_PTR
typedef v3::ForwardPtr  vmStackPtr;
typedef v3::ForwardPtr  vmFramePtr;
#else
typedef v3::BackwardPtr vmStackPtr;
typedef v3::BackwardPtr vmFramePtr;
#endif

struct vmContextRegs {
    vmImagePtr  ip_;
    vmFramePtr  fp_;
    Register    regs_;
    Integer     flags;

    vmContextRegs() : ip_(nullptr), fp_(nullptr) {
        regs_.uval = 0;
        flags.uval = 0;
    }
    ~vmContextRegs() {}

    void clear() {
        ip_.clear();
        fp_.clear();
        regs_.uval = 0;
        flags.uval = 0;
    }
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
        uint32_t * pInitValue = (uint32_t *)&(imageData[2]);
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

template <typename BasicType>
class vmImageInfo {
public:
    typedef BasicType basic_type;

private:
    unsigned char * ip_;
    unsigned char * ip_start_;
    unsigned char * ip_limit_;

public:
    vmImageInfo() : ip_(nullptr), ip_start_(nullptr), ip_limit_(nullptr) {
    }
    ~vmImageInfo() {
        clear();
    }

    bool isInited() const { return (ip_start_ != nullptr); }

    bool isEof() const { return (ip_ == ip_limit_); }
    bool isOverflow() const { return (ip_ >= ip_limit_); }
    bool isUnderflow() const { return (ip_ < ip_start_); }

    unsigned char * getPtr() const { return ip_; }
    void setPtr(void * ip) {
        this->ip_ = (unsigned char *)ip;
    }

    uint32_t getOffset() const { return (uint32_t)(ip_ - ip_start_); }
    ptrdiff_t getOffset64() const { return (ip_ - ip_start_); }

    unsigned char * getStart() const { return ip_start_; }
    unsigned char * getLimit() const { return ip_limit_; }

    void clear() {
        this->ip_ = nullptr;
        this->ip_limit_ = nullptr;
        this->ip_start_ = nullptr;
    }

    void reset() {
        this->ip_ = this->ip_start_;
    }

    void setting(void * imageStart, size_t imageSize, void * imageEntry) {
        this->ip_ = (unsigned char *)imageEntry;
        this->ip_limit_ = (unsigned char *)imageStart + imageSize;
        this->ip_start_ = (unsigned char *)imageStart;
    }

    unsigned char get() const {
        return *ip_;
    }
};

template <typename BasicType>
class ExecutionEngine;

template <typename BasicType = uintptr_t>
class ExecutionContext : public IExecutionContext<BasicType>,
                         public vmContextRegs {
public:
    typedef BasicType                       basic_type;
    typedef IExecutionContext<basic_type>   base_type;
    typedef size_t                          size_type;
    typedef ExecutionEngine<basic_type>     engine_type;
    typedef vmReturn<basic_type>            return_type;
    typedef vmContextRegs                   ctx_reg_type;
    typedef ExecutionContext<basic_type>    this_type;

    static const size_type kDefaultStackSize = 8 * 1048576U;

private:
#if USE_FORWARD_STACK_PTR
    vmStack<basic_type, false>  stack_;
    vmStack<basic_type, false>  callstack_;
#else
    vmStack<basic_type, true> stack_;
    vmStack<basic_type, true> callstack_;
#endif
    vmImageInfo<basic_type> image_;
    vmHeap<basic_type>      heap_;
    engine_type *           engine_;

public:
    ExecutionContext(engine_type * engine = nullptr) : engine_(engine) {}
    virtual ~ExecutionContext() {
        destroy();
    }

    vmThreadId getId() { return 1; }
    void setId(vmThreadId id) {
    }

    bool isInited() const {
        return (image_.isInited() && stack_.isInited());
    }

    engine_type * getEngine() { return engine_; }
    void setEngine(engine_type * engine) {
        engine_ = engine;
    }

    void setImageInfo(void * imageStart, size_t imageSize,
                      void * imageEntry) {
        image_.setting(imageStart, imageSize, imageEntry);
    }

    void create(size_type stackSize = kDefaultStackSize) {
        stack_.create(stackSize);
        callstack_.create(stackSize);
    }

    void destroy() {
        callstack_.destroy();
        stack_.destroy();
        image_.clear();
    }

    unsigned char * getIP() const {
        return ip_.ptr();
    }

    unsigned char * getFP() const {
        return fp_.ptr();
    }

    uint32_t getIpOffset(vmImagePtr & ip) {
        return (uint32_t)(ptrdiff_t)(ip.ptr() - image_.getStart());
    }

    bool fp_isOverflow(vmStackPtr & fp) const {
        if (stack_.isBackwardPtr())
            return (fp.ptr() <= stack_.first());
        else
            return (fp.ptr() >= stack_.last());
    }

    int32_t getArgIndex(int8_t index) {
#if USE_FORWARD_STACK_PTR
        return (index + (FRAME_STACK_SIZEOF + 1));
#else
        return (index - FRAME_STACK_SIZEOF);
#endif
    }

    JM_FORCEINLINE void push_callstack(vmFramePtr & fp, void * returnIP, intptr_t localSize) {
        void * framePoint = fp.get<void *>();
        fp.next64(localSize);
        fp.push_Pointer(framePoint);
        fp.push_Pointer(returnIP);
        assert(!fp_isOverflow(fp));
    }

    JM_FORCEINLINE void * pop_callstack(vmFramePtr & fp) {
        void * returnIP = fp.pop_Pointer();
        void * framePoint = fp.pop_Pointer();
        fp.set(framePoint);
        return returnIP;
    }

    JM_FORCEINLINE void push_callstack_fast(vmFramePtr & fp, void * returnIP, int32_t localSize) {
        fp.next(localSize);
        fp.push_Pointer(returnIP);
        assert(!fp_isOverflow(fp));
    }

    JM_FORCEINLINE void * pop_callstack_fast(vmFramePtr & fp, int32_t localSize) {
        void * returnIP = fp.pop_Pointer();
        fp.back(localSize);
        assert((localSize & 0x03) == 0);
        return returnIP;
    }

    JM_FORCEINLINE void inline_push_callstack(vmFramePtr & fp, vmStackPtr & cp,
                                              void * returnIP, intptr_t localSize, int retType) {
        void * framePoint = fp.get<void *>();
        fp.next64(localSize);
        fp.push_Pointer(framePoint);
        fp.push_Pointer(returnIP);
        cp.push_Int32(retType);
        assert(!fp_isOverflow(fp));
    }

    JM_FORCEINLINE void * inline_pop_callstack(vmFramePtr & fp, vmStackPtr & cp,
                                               int & retType) {
        void * returnIP = fp.pop_Pointer();
        void * framePoint = fp.pop_Pointer();
        fp.set(framePoint);
        retType = cp.pop_Int32();
        return returnIP;
    }

    JM_FORCEINLINE void inline_push_callstack_fast(vmFramePtr & fp, vmStackPtr & cp,
                                                   void * returnIP, int32_t localSize, int retType) {
        fp.next(localSize);
        fp.push_Pointer(returnIP);
        cp.push_Int32(retType);
        assert(!fp_isOverflow(fp));
    }

    JM_FORCEINLINE void * inline_pop_callstack_fast(vmFramePtr & fp, vmStackPtr & cp,
                                                    int32_t localSize, int & retType) {
        void * returnIP = fp.pop_Pointer();
        fp.back(localSize);
        assert((localSize & 0x03) == 0);
        retType = cp.pop_Int32();
        return returnIP;
    }

    template <typename U>
    JM_FORCEINLINE static bool getCondition(U v1, U v2, uint8_t CmpType) {
        switch (CmpType) {
        case OpCode::jz:
            return (v1 == 0 && v2 == 0);

        case OpCode::jnz:
            return (v1 != 0 && v2 != 0);

        case OpCode::je:
            return (v1 == v2);

        case OpCode::jne:
            return (v1 != v2);

        case OpCode::jl:
        case OpCode::jl_near:
        case OpCode::jl_short:
        case OpCode::jl_long:
            return (v1 < v2);

        case OpCode::jle:
            return (v1 <= v2);

        case OpCode::jg:
            return (v1 > v2);

        case OpCode::jge:
            return (v1 >= v2);

        case OpCode::js:
            return (v1 > 0);

        case OpCode::jns:
            return (v1 <= 0);

        case OpCode::jmp:
        case OpCode::jmp_near:
        case OpCode::jmp_short:
        case OpCode::jmp_long:
            return true;

        default:
            return false;
        }
    }

    JM_FORCEINLINE static void skipCondJmp(vmImagePtr & ip, unsigned char jmpType) {
        switch (jmpType) {
        case OpCode::jmp_near:
        case OpCode::jl_near:
            ip.next(1 + sizeof(int16_t));
            break;

        case OpCode::jmp_short:
        case OpCode::jl_short:
            ip.next(1 + sizeof(int16_t));
            break;

        case OpCode::jmp_long:
        case OpCode::jl_long:
            ip.next(1 + sizeof(int32_t));
            break;

        default:
            assert(false);
            break;
        }
    }

    //
    // error inst.
    //
    JM_FORCEINLINE void op_error(vmImagePtr & ip) {
        console.trace("%08X:  error\n", getIpOffset(ip));
        ip.next();
    }

    //
    // push arg0 (int32)
    //
    JM_FORCEINLINE void op_push(vmImagePtr & ip, vmStackPtr & sp, vmFramePtr & fp) {
        int8_t index = ip.getValue<0, int8_t>();
        int32_t value = fp.getArgValueUInt32(index);
        sp.writeInt32(value);

        console.trace("%08X:  push args[%d]  (0x%08X, int32)\n",
                      getIpOffset(ip), getArgIndex(index), value);
        ip.next(1 + sizeof(int8_t));
    }

    //
    // push 0x00000008 (int32)
    //
    JM_FORCEINLINE void op_push_i32(vmImagePtr & ip, vmStackPtr & sp) {
        int32_t value = ip.getValue<0, int32_t>();
        sp.writeInt32(value);
        console.trace("%08X:  push_i32 0x%08X (int32)\n", getIpOffset(ip), value);
        ip.next(1 + sizeof(int32_t));
    }

    //
    // push 0x00000000 00000008 (int64)
    //
    JM_FORCEINLINE void op_push_i64(vmImagePtr & ip, vmStackPtr & sp) {
        int64_t value = ip.getValue<0, int64_t>();
        sp.writeInt64(value);
        console.trace("%08X:  push_i64 0x%016X (int64)\n", getIpOffset(ip), value);
        ip.next(1 + sizeof(int64_t));
    }

    //
    // push_0 (int32)
    //
    JM_FORCEINLINE void op_push_i32_0(vmImagePtr & ip, vmStackPtr & sp) {
        int32_t value = 0;
        sp.writeInt32(value);
        console.trace("%08X:  push_i32_0 (int32)\n", getIpOffset(ip));
        ip.next();
    }

    //
    // push_0 (int64)
    //
    JM_FORCEINLINE void op_push_i64_0(vmImagePtr & ip, vmStackPtr & sp) {
        int64_t value = 0;
        sp.writeInt64(value);
        console.trace("%08X:  push_i64_0 (int64)\n", getIpOffset(ip));
        ip.next();
    }

    //
    // pop uint32
    //
    JM_FORCEINLINE void op_pop(vmImagePtr & ip, vmStackPtr & sp) {
        sp.backUInt32();
        uint32_t value = sp.getUInt32();
        console.trace("%08X:  pop  (0x%08X)\n", getIpOffset(ip), value);
        ip.next();
    }

    //
    // pop int32
    //
    JM_FORCEINLINE void op_pop_i32(vmImagePtr & ip, vmStackPtr & sp) {
        sp.backInt32();
        int32_t value = sp.getInt32();
        console.trace("%08X:  pop_i32  (0x%08X)\n", getIpOffset(ip), value);
        ip.next();
    }

    //
    // pop int64
    //
    JM_FORCEINLINE void op_pop_i64(vmImagePtr & ip, vmStackPtr & sp) {
        sp.backInt64();
        int64_t value = sp.getInt64();
        console.trace("%08X:  pop_i64  (0x%016X)\n", getIpOffset(ip), value);
        ip.next();
    }

    //
    // add_sp 16
    //
    JM_FORCEINLINE void op_add_sp(vmImagePtr & ip, vmStackPtr & sp) {
        uint8_t localSize = ip.getValue<0, uint8_t>();
        sp.next(localSize);

        console.trace("%08X:  add_sp %u\n", getIpOffset(ip), (uint32_t)localSize);
        ip.next(1 + sizeof(uint8_t));
    }

    //
    // add_sp_4
    //
    JM_FORCEINLINE void op_add_sp_4(vmImagePtr & ip, vmStackPtr & sp) {
        sp.next(sizeof(uint32_t));
        console.trace("%08X:  add_sp_4\n", getIpOffset(ip));
        ip.next();
    }

    //
    // load eax, 0x00000006
    //
    JM_FORCEINLINE void op_load_eax(vmImagePtr & ip, vmFramePtr & fp, Register & regs) {
        uint32_t value = ip.getValue<0, uint32_t>();
        regs.eax.u32 = value;
        console.trace("%08X:  load eax, 0x%08X\n", getIpOffset(ip), value);
        ip.next(1 + sizeof(uint32_t));
    }

    //
    // store arg0, 0x00000006
    //
    JM_FORCEINLINE void op_store(vmImagePtr & ip, vmFramePtr & fp) {
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value = ip.getValue<0, uint32_t, uint32_t, 2>();
        fp.putArgValueUInt32(index, value);
        console.trace("%08X:  store args[%d], 0x%08X\n",
                      getIpOffset(ip), getArgIndex(index), value);
        ip.next(1 + sizeof(int8_t) + sizeof(uint32_t));
    }

    //
    // move arg0, arg1
    //
    JM_FORCEINLINE void op_move(vmImagePtr & ip, vmFramePtr & fp) {
        int8_t index1 = ip.getValue<0, int8_t>();
        int8_t index2 = ip.getValue<0, int8_t, int8_t, 2>();
        uint32_t value = fp.getArgValueUInt32(index2);
        fp.putArgValueUInt32(index1, value);
        console.trace("%08X:  move args[%d], args[%d] - 0x%08X\n",
                      getIpOffset(ip), getArgIndex(index1), getArgIndex(index2), value);
        ip.next(1 + sizeof(int8_t) + sizeof(int8_t));
    }

    //
    // move eax, arg1
    //
    JM_FORCEINLINE void op_move_to_eax(vmImagePtr & ip, vmFramePtr & fp) {
        console.trace("%08X:  move eax, args[%d]\n", getIpOffset(ip), 0);
        ip.next();
    }

    //
    // copy arg0, eax
    //
    JM_FORCEINLINE void op_copy_from_eax(vmImagePtr & ip, vmFramePtr & fp, Register & regs) {
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value = regs.eax.u32;
        fp.putArgValueUInt32(index, value);
        console.trace("%08X:  copy args[%d], eax = (0x%08X)\n",
                      getIpOffset(ip), getArgIndex(index), value);
        ip.next(1 + sizeof(int8_t));
    }

    JM_FORCEINLINE void op_cmp(vmImagePtr & ip, vmFramePtr & fp) {
        console.trace("%08X:  cmp\n", getIpOffset(ip));
        ip.next();
    }

    //
    // cmp arg0, arg1 (int32)
    //
    JM_FORCEINLINE bool op_cmp_i32(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index1 = ip.getValue<0, int8_t>();
        int8_t index2 = ip.getValue<0, int8_t, int8_t, 2>();
        int32_t value1 = fp.getArgValueInt32(index1);
        int32_t value2 = fp.getArgValueInt32(index2);
        ip.next(1 + sizeof(int8_t) * 2);

        console.trace("%08X:  cmp  args[%d], args[%d] - (%d, %d) (int32)\n",
                      offset, getArgIndex(index1), getArgIndex(index2),
                      value1, value2);

        unsigned char jmpType = ip.getUInt8();
        bool condition = this_type::getCondition(value1, value2, jmpType);
        flags.u32.low = (uint32_t)condition;
        if (likely(!condition))
            console.trace("%08X:  cmp  condition [false]\n", offset);
        else
            console.trace("%08X:  cmp  condition [true]\n", offset);
        return condition;
    }

    //
    // cmp arg0, arg1 (uint32)
    //
    JM_FORCEINLINE bool op_cmp_u32(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index1 = ip.getValue<0, int8_t>();
        int8_t index2 = ip.getValue<0, int8_t, int8_t, 2>();
        uint32_t value1 = fp.getArgValueUInt32(index1);
        uint32_t value2 = fp.getArgValueUInt32(index2);
        ip.next(1 + sizeof(int8_t) * 2);

        console.trace("%08X:  cmp  args[%d], args[%d] - (%u, %u) (uint32)\n",
                      offset, getArgIndex(index1), getArgIndex(index2),
                      value1, value2);

        unsigned char jmpType = ip.getUInt8();
        bool condition = this_type::getCondition(value1, value2, jmpType);
        flags.u32.low = (uint32_t)condition;
        if (likely(!condition))
            console.trace("%08X:  cmp  condition [false]\n", offset);
        else
            console.trace("%08X:  cmp  condition [true]\n", offset);
        return condition;
    }

    //
    // cmp arg0, 0x00000008 (int32)
    //
    JM_FORCEINLINE bool op_cmp_imm_i32(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index = ip.getValue<0, int8_t>();
        int32_t value1 = fp.getArgValueInt32(index);
        int32_t value2 = ip.getValue<0, int32_t, int32_t, 2>();
        ip.next(1 + sizeof(int8_t) + sizeof(int32_t));

        console.trace("%08X:  cmp  args[%d], 0x%08X (int32)\n",
                      offset, getArgIndex(index), value2);

        unsigned char jmpType = ip.getUInt8();
        bool condition = this_type::getCondition(value1, value2, jmpType);
        flags.u32.low = (uint32_t)condition;
        if (likely(!condition))
            console.trace("%08X:  cmp  condition [false]\n", offset);
        else
            console.trace("%08X:  cmp  condition [true]\n", offset);
        return condition;
    }

    //
    // cmp arg0, 0x00000008 (uint32)
    //
    JM_FORCEINLINE bool op_cmp_imm_u32(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value1 = fp.getArgValueUInt32(index);
        uint32_t value2 = ip.getValue<0, uint32_t, uint32_t, 2>();
        ip.next(1 + sizeof(int8_t) + sizeof(uint32_t));

        console.trace("%08X:  cmp  args[%d], 0x%08X (uint32)\n",
                      offset, getArgIndex(index), value2);

        unsigned char jmpType = ip.getUInt8();
        bool condition = this_type::getCondition(value1, value2, jmpType);
        flags.u32.low = (uint32_t)condition;
        if (likely(!condition))
            console.trace("%08X:  cmp  condition [false]\n", offset);
        else
            console.trace("%08X:  cmp  condition [true]\n", offset);
        return condition;
    }

    JM_FORCEINLINE void op_jl(vmImagePtr & ip) {
        console.trace("%08X:  jl\n", getIpOffset(ip));
        ip.next();
    }

    //
    // jl_near 0x06
    //
    JM_FORCEINLINE bool op_jl_near(vmImagePtr & ip) {
        uint32_t offset = getIpOffset(ip);
        int8_t jmpOffset = ip.getValue<0, int8_t>();
        if (likely(flags.u32.low != (uint32_t)true)) {
            ip.next(1 + sizeof(int8_t));
            uint32_t jmpEntry = getIpOffset(ip) + jmpOffset;
            console.trace("%08X:  jl   0x%08X (near)\n", offset, jmpEntry);
            return false;
        }
        else {
            ip.next(1L + sizeof(int8_t) + jmpOffset);
            console.trace("%08X:  jl   0x%08X (near)\n\n", offset, getIpOffset(ip));
            return true;
        }
    }

    //
    // jl_short 0x16, 0x00
    //
    JM_FORCEINLINE bool op_jl_short(vmImagePtr & ip) {
        uint32_t offset = getIpOffset(ip);
        int16_t jmpOffset = ip.getValue<0, int16_t>();
        if (likely(flags.u32.low != (uint32_t)true)) {
            ip.next(1 + sizeof(int16_t));
            uint32_t jmpEntry = getIpOffset(ip) + jmpOffset;
            console.trace("%08X:  jl   0x%08X (short)\n", offset, jmpEntry);
            return false;
        }
        else {
            ip.next(1L + sizeof(int16_t) + jmpOffset);
            console.trace("%08X:  jl   0x%08X (short)\n\n", offset, getIpOffset(ip));
            return true;
        }
    }

    //
    // jl_long 0x29, 0x00, 0x00, 0x00
    //
    JM_FORCEINLINE bool op_jl_long(vmImagePtr & ip) {
        uint32_t offset = getIpOffset(ip);
        int32_t jmpOffset = ip.getValue<0, int32_t>();
        if (likely(flags.u32.low != (uint32_t)true)) {
            ip.next(1 + sizeof(int32_t));
            uint32_t jmpEntry = getIpOffset(ip) + jmpOffset;
            console.trace("%08X:  jl   0x%08X (long)\n", offset, jmpEntry);
            return false;
        }
        else {
            ip.next(1L + sizeof(int8_t) + jmpOffset);
            console.trace("%08X:  jl   0x%08X (long)\n\n", offset, getIpOffset(ip));
            return true;
        }
    }

    //
    // jmp 0x00102030 (ptr32)
    //
    JM_FORCEINLINE void op_jmp(vmImagePtr & ip) {
        uint32_t offset = getIpOffset(ip);
        uint32_t jmpEntry = ip.getValue<0, uint32_t>();
        ip.set(image_.getStart() + jmpEntry);
        console.trace("%08X:  jmp  0x%08X (ptr32)\n\n", offset, getIpOffset(ip));
    }

    //
    // jmp_near 0x08
    //
    JM_FORCEINLINE void op_jmp_near(vmImagePtr & ip) {
        uint32_t offset = getIpOffset(ip);
        int8_t jmpOffset = ip.getValue<0, int8_t>();
        ip.next(1L + sizeof(int8_t) + jmpOffset);
        console.trace("%08X:  jmp  0x%08X (near)\n\n", offset, getIpOffset(ip));
    }

    //
    // jmp_short 0x08, 0x00
    //
    JM_FORCEINLINE void op_jmp_short(vmImagePtr & ip) {
        uint32_t offset = getIpOffset(ip);
        int16_t jmpOffset = ip.getValue<0, int16_t>();
        ip.next(1L + sizeof(int16_t) + jmpOffset);
        console.trace("%08X:  jmp  0x%08X (short)\n\n", offset, getIpOffset(ip));
    }

    //
    // jmp_long 0x18, 0x00, 0x00, 0x00
    //
    JM_FORCEINLINE void op_jmp_long(vmImagePtr & ip) {
        uint32_t offset = getIpOffset(ip);
        int32_t jmpOffset = ip.getValue<0, int32_t>();
        ip.next(1L + sizeof(int32_t) + jmpOffset);
        console.trace("%08X:  jmp  0x%08X (long)\n\n", offset, getIpOffset(ip));
    }

    //
    // call 0x00102030 (ptr32)
    //
    JM_FORCEINLINE void op_call(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        uint32_t callEntry = ip.getValue<0, uint32_t>();
        uint16_t localSize = ip.getValue<0, uint16_t, uint16_t, 5>();
        ip.next(1 + sizeof(uint32_t) + sizeof(uint16_t));

        void * returnIP = ip.get<void *>();
        push_callstack(fp, returnIP, localSize);

        unsigned char * newIP = image_.getStart() + callEntry;
        assert(CHECK_ADDR_ALIGNMENT(newIP));
        ip.set(newIP);

        console.trace("%08X:  call 0x%08X, %u (ptr32)\n\n",
                      offset, (uint32_t)localSize, getIpOffset(ip));
    }

    //
    // call_short 0x08, 0x00, 0x08, 0x00
    //
    JM_FORCEINLINE void op_call_short(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int16_t callOffset = ip.getValue<0, int16_t>();
        uint16_t localSize = ip.getValue<0, uint16_t, uint16_t, 3>();
        ip.next(1 + sizeof(int16_t) + sizeof(uint16_t));

        void * returnIP = ip.get<void *>();
        push_callstack(fp, returnIP, localSize);

        void * newIP = PointerAdd(returnIP, callOffset);
        assert(CHECK_ADDR_ALIGNMENT(newIP));
        ip.set(newIP);

        console.trace("%08X:  call 0x%08X, %u (short)\n\n",
                      offset, (uint32_t)localSize, getIpOffset(ip));
    }

    //
    // call_long 0x18, 0x00, 0x00, 0x00, 0x08, 0x00
    //
    JM_FORCEINLINE void op_call_long(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int32_t callOffset = ip.getValue<0, int32_t>();
        uint16_t localSize = ip.getValue<0, uint16_t, uint16_t, 5>();
        ip.next(1 + sizeof(int32_t) + sizeof(uint16_t));

        void * returnIP = ip.get<void *>();
        push_callstack(fp, returnIP, localSize);

        void * newIP = PointerAdd(returnIP, callOffset);
        assert(CHECK_ADDR_ALIGNMENT(newIP));
        ip.set(newIP);

        console.trace("%08X:  call 0x%08X, %u (long)\n\n",
                      offset, (uint32_t)localSize, getIpOffset(ip));
    }

    //
    // fast_call_short 0x05, 0x00, 0x08, 0x00
    //
    JM_FORCEINLINE void op_fast_call_short(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int16_t callOffset = ip.getValue<0, int16_t>();
        uint16_t localSize = ip.getValue<0, uint16_t, uint16_t, 3>();
        ip.next(1 + sizeof(int16_t) + sizeof(uint16_t));

        void * returnIP = ip.get<void *>();
        push_callstack_fast(fp, returnIP, localSize);

        void * newIP = PointerAdd(returnIP, callOffset);
        assert(CHECK_ADDR_ALIGNMENT(newIP));
        ip.set(newIP);

        console.trace("%08X:  fast_call 0x%08X, %u (short)\n\n",
                      offset, (uint32_t)localSize, getIpOffset(ip));
    }

    //
    // ret
    //
    JM_FORCEINLINE bool op_ret(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        void * returnIP = pop_callstack(fp);
        ip.set(returnIP);

        if (returnIP != nullptr) {
            console.trace("%08X:  ret  0x%08X\n\n", offset, getIpOffset(ip));
            return false;
        }
        else {
            console.trace("%08X:  ret  (done)\n\n", offset);
            return true;
        }
    }

    //
    // ret_n_sm 0x08
    //
    JM_FORCEINLINE bool op_ret_n_sm(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        uint8_t localSize = ip.getValue<0, uint8_t>();
        void * returnIP = pop_callstack_fast(fp, localSize);
        ip.set(returnIP);

        if (returnIP == nullptr) {
            console.trace("%08X:  ret_n_sm [%u] (done)\n\n", offset, (uint32_t)localSize);
        }
        else {
            console.trace("%08X:  ret_n_sm [%u] 0x%08X\n\n",
                          offset, (uint32_t)localSize, getIpOffset(ip));
        }

        return (returnIP == nullptr);
    }

    //
    // ret_n 0x08, 0x00
    //
    JM_FORCEINLINE bool op_ret_n(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        uint16_t localSize = ip.getValue<0, uint16_t>();
        void * returnIP = pop_callstack_fast(fp, localSize);
        ip.set(returnIP);

        if (returnIP == nullptr) {
            console.trace("%08X:  ret_n [%u] (done)\n\n", offset, (uint32_t)localSize);
        }
        else {
            console.trace("%08X:  ret_n [%u] 0x%08X\n\n",
                          offset, (uint32_t)localSize, getIpOffset(ip));
        }

        return (returnIP == nullptr);
    }

    //
    // ret_eax 0x00000001
    //
    JM_FORCEINLINE bool op_ret_eax(vmImagePtr & ip, vmFramePtr & fp, Register & regs) {
        uint32_t offset = getIpOffset(ip);
        uint32_t value = ip.getValue<0, uint32_t>();
        regs.eax.u32 = value;

        void * returnIP = pop_callstack(fp);
        ip.set(returnIP);

        if (returnIP != nullptr) {
            console.trace("%08X:  ret_eax 0x%08X (eax = 0x%08X)\n\n",
                          offset, getIpOffset(ip), value);
            return false;
        }
        else {
            console.trace("%08X:  ret_eax (done) (eax = 0x%08X)\n\n", offset, value);
            return true;
        }
    }

    //
    // ret_eax_n 0x08, 0x00, 0x00000001
    //
    JM_FORCEINLINE bool op_ret_eax_n(vmImagePtr & ip, vmFramePtr & fp, Register & regs) {
        uint32_t offset = getIpOffset(ip);
        uint16_t localSize = ip.getValue<0, uint16_t>();
        uint32_t value = ip.getValue<0, uint32_t, uint32_t, 3>();
        regs.eax.u32 = value;

        void * returnIP = pop_callstack_fast(fp, localSize);
        ip.set(returnIP);

        if (returnIP != nullptr) {
            console.trace("%08X:  ret_eax_n [%u] 0x%08X (eax = 0x%08X)\n\n",
                          offset, (uint32_t)localSize, getIpOffset(ip), value);
            return false;
        }
        else {
            console.trace("%08X:  ret_eax_n [%u] (eax = 0x%08X) (done)\n\n",
                          offset, (uint32_t)localSize, value);
            return true;
        }
    }

    //
    // inline_call_near 0x08
    //
    JM_FORCEINLINE void op_inline_call_near(vmImagePtr & ip, vmFramePtr & fp,
                                            vmStackPtr & cp, int retType) {
        uint32_t offset = getIpOffset(ip);
        int8_t callOffset = ip.getValue<0, int8_t>();
        ip.next(1 + sizeof(int8_t));

        void * returnIP = ip.get<void *>();
        inline_push_callstack(fp, cp, returnIP, 0, retType);

        void * newIP = PointerAdd(returnIP, callOffset);
        assert(CHECK_ADDR_ALIGNMENT(newIP));
        ip.set(newIP);

        console.trace("%08X:  call 0x%08X (near)\n\n", offset, getIpOffset(ip));
    }

    //
    // inline_call_short 0x05, 0x00, 0x08, 0x00
    //
    JM_FORCEINLINE void op_inline_call_short(vmImagePtr & ip, vmFramePtr & fp,
                                             vmStackPtr & cp, int retType) {
        uint32_t offset = getIpOffset(ip);
        int16_t callOffset = ip.getValue<0, int16_t>();
        uint16_t localSize = ip.getValue<0, uint16_t, uint16_t, 3>();
        ip.next(1 + sizeof(int16_t) + sizeof(uint16_t));

        void * returnIP = ip.get<void *>();
        inline_push_callstack(fp, cp, returnIP, localSize, retType);

        void * newIP = PointerAdd(returnIP, callOffset);
        assert(CHECK_ADDR_ALIGNMENT(newIP));
        ip.set(newIP);

        console.trace("%08X:  fast_call 0x%08X, %u (short)\n\n",
                      offset, (uint32_t)localSize, getIpOffset(ip));
    }

    //
    // inline_fast_call_short 0x05, 0x00, 0x08, 0x00
    //
    JM_FORCEINLINE void op_inline_fast_call_short(vmImagePtr & ip, vmFramePtr & fp,
                                                  vmStackPtr & cp, int retType) {
        uint32_t offset = getIpOffset(ip);
        int16_t callOffset = ip.getValue<0, int16_t>();
        uint16_t localSize = ip.getValue<0, uint16_t, uint16_t, 3>();
        ip.next(1 + sizeof(int16_t) + sizeof(uint16_t));

        void * returnIP = ip.get<void *>();
        inline_push_callstack_fast(fp, cp, returnIP, localSize, retType);

        void * newIP = PointerAdd(returnIP, callOffset);
        assert(CHECK_ADDR_ALIGNMENT(newIP));
        ip.set(newIP);

        console.trace("%08X:  fast_call 0x%08X, %u (short)\n\n",
                      offset, (uint32_t)localSize, getIpOffset(ip));
    }

    //
    // inline_call_long 0x18, 0x00, 0x00, 0x00
    //
    JM_FORCEINLINE void op_inline_call_long(vmImagePtr & ip, vmFramePtr & fp,
                                            vmStackPtr & cp, int retType) {
        uint32_t offset = getIpOffset(ip);
        int32_t callOffset = ip.getValue<0, int32_t>();
        ip.next(1 + sizeof(int32_t));

        void * returnIP = ip.get<void *>();
        inline_push_callstack(fp, cp, returnIP, retType);

        void * newIP = PointerAdd(returnIP, callOffset);
        assert(CHECK_ADDR_ALIGNMENT(newIP));
        ip.set(newIP);

        console.trace("%08X:  call 0x%08X (long)\n\n", offset, getIpOffset(ip));
    }

    //
    // ret
    //
    JM_FORCEINLINE int op_inline_ret(vmImagePtr & ip, vmFramePtr & fp,
                                     vmStackPtr & cp, bool & done) {
        uint32_t offset = getIpOffset(ip);

        int retType;
        void * returnIP = inline_pop_callstack(fp, cp, retType);
        ip.set(returnIP);

        if (returnIP != nullptr) {
            console.trace("%08X:  ret  0x%08X\n\n", offset, getIpOffset(ip));
            done = false;
        }
        else {
            console.trace("%08X:  ret  (done)\n\n", offset);
            done = true;
        }

        return retType;
    }

    //
    // ret_n 0x08, 0x00
    //
    JM_FORCEINLINE int op_inline_ret_n(vmImagePtr & ip, vmFramePtr & fp,
                                       vmStackPtr & cp, bool & done) {
        uint32_t offset = getIpOffset(ip);
        uint16_t localSize = ip.getValue<0, uint16_t>();

        int retType;
        void * returnIP = inline_pop_callstack_fast(fp, cp, localSize, retType);
        ip.set(returnIP);

        if (returnIP != nullptr) {
            console.trace("%08X:  ret_n [%u] 0x%08X\n\n",
                          offset, (uint32_t)localSize, getIpOffset(ip));
            done = false;
        }
        else {
            console.trace("%08X:  ret_n [%u] (done)\n\n", offset, (uint32_t)localSize);
            done = true;
        }

        return retType;
    }

    //
    // ret_eax_n 0x08, 0x00000001
    //
    JM_FORCEINLINE int op_inline_ret_eax_n(vmImagePtr & ip, vmFramePtr & fp, vmStackPtr & cp,
                                           Register & regs, bool & done) {
        uint32_t offset = getIpOffset(ip);
        uint16_t localSize = ip.getValue<0, uint16_t>();
        uint32_t value = ip.getValue<0, uint32_t, uint32_t, 3>();
        regs.eax.u32 = value;

        int retType;
        void * returnIP = inline_pop_callstack_fast(fp, cp, localSize, retType);
        ip.set(returnIP);

        if (returnIP == nullptr) {
            console.trace("%08X:  ret_eax_n %u (done) (eax = 0x%08X)\n\n",
                          offset, (uint32_t)localSize, value);
        }
        else {
            console.trace("%08X:  ret_eax_n %u, 0x%08X (eax = 0x%08X)\n\n",
                          offset, getIpOffset(ip), (uint32_t)localSize, value);
        }

        done = (returnIP == nullptr);
        return retType;
    }

    //
    // nop
    //
    JM_FORCEINLINE void op_nop(vmImagePtr & ip) {
        console.trace("%08X:  nop\n", getIpOffset(ip));
        ip.next();
    }

    //
    // nop_n 0x08
    //
    JM_FORCEINLINE void op_nop_n(vmImagePtr & ip) {
        uint8_t skip_n = ip.getValue<0, uint8_t>();
        console.trace("%08X:  nop_n %u\n", getIpOffset(ip), (uint32_t)skip_n);
        ip.next(1 + sizeof(uint8_t) + skip_n);
    }

    //
    // inc arg0
    //
    JM_FORCEINLINE void op_inc(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value = fp.getArgValueUInt32(index);
        value++;
        fp.putArgValueUInt32(index, value);

        console.trace("%08X:  inc  arg[%d]  (0x%08X)\n",
                      offset, getArgIndex(index), value);
        ip.next(1 + sizeof(int8_t));
    }

    //
    // dec arg0
    //
    JM_FORCEINLINE void op_dec(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value = fp.getArgValueUInt32(index);
        value--;
        fp.putArgValueUInt32(index, value);

        console.trace("%08X:  dec  args[%d]  (0x%08X)\n",
                      offset, getArgIndex(index), value);
        ip.next(1 + sizeof(int8_t));
    }

    //
    // add arg0, arg1
    //
    JM_FORCEINLINE void op_add(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index1 = ip.getValue<0, int8_t>();
        int8_t index2 = ip.getValue<0, int8_t, int8_t, 2>();
        uint32_t value1 = fp.getArgValueUInt32(index1);
        uint32_t value2 = fp.getArgValueUInt32(index2);

        uint32_t newValue = value1 + value2;
        fp.putArgValueUInt32(index1, newValue);

        console.trace("%08X:  add  args[%d], args[%d] = (0x%08X)\n",
                      offset, getArgIndex(index1), getArgIndex(index2),
                      newValue);
        ip.next(1 + sizeof(int8_t) * 2);
    }

    //
    // add arg0, 0x00000006
    //
    JM_FORCEINLINE void op_add_imm(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value2 = ip.getValue<0, uint32_t, uint32_t, 2>();
        uint32_t value1 = fp.getArgValueUInt32(index);

        uint32_t newValue = value1 + value2;
        fp.putArgValueUInt32(index, newValue);

        console.trace("%08X:  add  args[%d], 0x%08X = (0x%08X)\n",
                      offset, getArgIndex(index), value2, newValue);
        ip.next(1 + sizeof(int8_t) + sizeof(uint32_t));
    }

    //
    // add eax, arg0
    //
    JM_FORCEINLINE void op_add_eax(vmImagePtr & ip, vmFramePtr & fp, Register & regs) {
        uint32_t offset = getIpOffset(ip);
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value = fp.getArgValueUInt32(index);

        uint32_t newValue = regs.eax.u32 + value;
        regs.eax.u32 = newValue;

        console.trace("%08X:  add  eax, args[%d] = (0x%08X)\n",
                      offset, getArgIndex(index), newValue);
        ip.next(1 + sizeof(int8_t));
    }

    //
    // add eax, 0x00000006
    //
    JM_FORCEINLINE void op_add_eax_imm(vmImagePtr & ip, Register & regs) {
        uint32_t offset = getIpOffset(ip);
        uint32_t value = ip.getValue<0, uint32_t, uint32_t>();

        uint32_t newValue = regs.eax.u32 + value;
        regs.eax.u32 = newValue;

        console.trace("%08X:  add  eax, 0x%08X = (0x%08X)\n",
                      offset, value, newValue);
        ip.next(1 + sizeof(uint32_t));
    }

    //
    // sub arg0, arg1
    //
    JM_FORCEINLINE void op_sub(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index1 = ip.getValue<0, int8_t>();
        int8_t index2 = ip.getValue<0, int8_t, int8_t, 2>();
        uint32_t value1 = fp.getArgValueUInt32(index1);
        uint32_t value2 = fp.getArgValueUInt32(index2);
        uint32_t newValue = value1 - value2;
        fp.putArgValueUInt32(index1, newValue);

        console.trace("%08X:  sub  args[%d], args[%d] = (0x%08X)\n",
                      offset, getArgIndex(index1), getArgIndex(index2),
                      newValue);
        ip.next(1 + sizeof(int8_t) * 2);
    }

    //
    // sub arg0, 0x00000006
    //
    JM_FORCEINLINE void op_sub_imm(vmImagePtr & ip, vmFramePtr & fp) {
        uint32_t offset = getIpOffset(ip);
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value2 = ip.getValue<0, uint32_t, uint32_t, 2>();
        uint32_t value1 = fp.getArgValueUInt32(index);

        uint32_t newValue = value1 - value2;
        fp.putArgValueUInt32(index, newValue);

        console.trace("%08X:  sub  args[%d], 0x%08X = (0x%08X)\n",
                      offset, getArgIndex(index), value2, newValue);
        ip.next(1 + sizeof(int8_t) + sizeof(uint32_t));
    }

    //
    // sub eax, arg0
    //
    JM_FORCEINLINE void op_sub_eax(vmImagePtr & ip, vmFramePtr & fp, Register & regs) {
        uint32_t offset = getIpOffset(ip);
        int8_t index = ip.getValue<0, int8_t>();
        uint32_t value = fp.getArgValueUInt32(index);
        uint32_t newValue = regs.eax.u32 - value;
        regs.eax.u32 = newValue;

        console.trace("%08X:  sub  eax, args[%d] = (0x%08X)\n",
                      offset, getArgIndex(index), newValue);
        ip.next(1 + sizeof(int8_t));
    }

    //
    // add eax, 0x00000006
    //
    JM_FORCEINLINE void op_sub_eax_imm(vmImagePtr & ip, Register & regs) {
        uint32_t offset = getIpOffset(ip);
        uint32_t value = ip.getValue<0, uint32_t>();

        uint32_t newValue = regs.eax.u32 - value;
        regs.eax.u32 = newValue;

        console.trace("%08X:  sub  eax, 0x%08X = (0x%08X)\n",
                      offset, value, newValue);
        ip.next(1 + sizeof(uint32_t));
    }

    //
    // Exit the program
    //
    JM_FORCEINLINE void op_exit(vmImagePtr & ip, return_type & retValue) {
        console.trace("%08X:  end\n\n", getIpOffset(ip));
        ip.next();
    }

    //
    // Unknown opcode.
    //
    JM_FORCEINLINE void op_unknown(vmImagePtr & ip, unsigned char opcode) {
        console.trace("%08X:  Error: Unknown opcode: %u\n", getIpOffset(ip), (uint32_t)opcode);
        ip.next();
    }

    //
    // Execute the vm bytecode.
    //
    int execute(return_type & retVal) {
        int ec = 0;
        if (isInited()) {
            register vmImagePtr ip;
            register vmFramePtr fp;
            register Register   regs;

            // Init environment
            ip.set(image_.getPtr());
            fp.set(stack_.current());
            regs.uval = 0;

            // Push call program entry.
            push_callstack(fp, nullptr, 0);

            // Main loop
            while (ip.ptr() < image_.getLimit()) {
                unsigned char opcode = ip.getUInt8();
                switch (opcode) {
                case OpCode::error:
                    op_error(ip);
                    break;

                case OpCode::load_eax:
                    op_load_eax(ip, fp, regs);
                    break;

                case OpCode::store:
                    op_store(ip, fp);
                    break;

                case OpCode::move:
                    op_move(ip, fp);
                    break;

                case OpCode::move_to_eax:
                    op_move_to_eax(ip, fp);
                    break;

                case OpCode::copy_from_eax:
                    op_copy_from_eax(ip, fp, regs);
                    break;

                case OpCode::cmp:
                    op_cmp(ip, fp);
                    break;

                case OpCode::cmp_i32:
                    op_cmp_i32(ip, fp);
                    break;

                case OpCode::cmp_u32:
                    op_cmp_u32(ip, fp);
                    break;

                case OpCode::cmp_imm_i32:
                    op_cmp_imm_i32(ip, fp);
                    break;

                case OpCode::cmp_imm_u32:
                    op_cmp_imm_u32(ip, fp);
                    break;

                case OpCode::jl:
                    op_jl(ip);
                    break;

                case OpCode::jl_near:
                    op_jl_near(ip);
                    break;

                case OpCode::jl_short:
                    op_jl_short(ip);
                    break;

                case OpCode::jl_long:
                    op_jl_long(ip);
                    break;

                case OpCode::jmp:
                    op_jmp(ip);
                    break;

                case OpCode::jmp_near:
                    op_jmp_near(ip);
                    break;

                case OpCode::jmp_short:
                    op_jmp_short(ip);
                    break;

                case OpCode::jmp_long:
                    op_jmp_long(ip);
                    break;

                case OpCode::call:
                    op_call(ip, fp);
                    break;

                case OpCode::call_short:
                    op_call_short(ip, fp);
                    break;

                case OpCode::call_long:
                    op_call_long(ip, fp);
                    break;

                case OpCode::fast_call_short:
                    op_fast_call_short(ip, fp);
                    break;

                case OpCode::ret:
                    {
                        bool isDone = op_ret(ip, fp);
                        if (!isDone)
                            break;
                        else
                            goto Execute_Finished;
                    }

                case OpCode::ret_n_sm:
                    {
                        bool isDone = op_ret_n_sm(ip, fp);
                        if (!isDone)
                            break;
                        else
                            goto Execute_Finished;
                    }

                case OpCode::ret_n:
                    {
                        bool isDone = op_ret_n(ip, fp);
                        if (!isDone)
                            break;
                        else
                            goto Execute_Finished;
                    }

                case OpCode::ret_eax:
                    {
                        bool isDone = op_ret_eax(ip, fp, regs);
                        if (!isDone)
                            break;
                        else
                            goto Execute_Finished;
                    }

                case OpCode::ret_eax_n:
                    {
                        bool isDone = op_ret_eax_n(ip, fp, regs);
                        if (!isDone)
                            break;
                        else
                            goto Execute_Finished;
                    }

                case OpCode::nop:
                    op_nop(ip);
                    break;

                case OpCode::nop_n:
                    op_nop_n(ip);
                    break;

                case OpCode::inc:
                    op_inc(ip, fp);
                    break;

                case OpCode::dec:
                    op_dec(ip, fp);
                    break;

                case OpCode::add:
                    op_add(ip, fp);
                    break;

                case OpCode::add_imm:
                    op_add_imm(ip, fp);
                    break;

                case OpCode::add_eax:
                    op_add_eax(ip, fp, regs);
                    break;

                case OpCode::add_eax_imm:
                    op_add_eax_imm(ip, regs);
                    break;

                case OpCode::sub:
                    op_sub(ip, fp);
                    break;

                case OpCode::sub_imm:
                    op_sub_imm(ip, fp);
                    break;

                case OpCode::sub_eax:
                    op_sub_eax(ip, fp, regs);
                    break;

                case OpCode::sub_eax_imm:
                    op_sub_eax_imm(ip, regs);
                    break;

                case OpCode::exit:
                    op_exit(ip, retVal);
                    goto Execute_Finished;

                default:
                    op_unknown(ip, opcode);
                    break;
                }
            }
Execute_Finished:
            retVal.setDataType(return_type::Basic);
            retVal.setValue(regs.eax.u32);
        }
        return ec;
    }

    enum {
        ret_first,
        ret_00,
        ret_01,
        ret_02,
        ret_last
    };

    int execute_inline(return_type & retVal) {
        int ec = 0;
        if (isInited()) {
            register vmImagePtr ip;
            register vmStackPtr sp;
            register vmFramePtr fp;
            register vmStackPtr cp;
            register Register   regs;

            // Init environment
            ip.set(image_.getPtr());
            sp.set(stack_.current());
            fp.set(stack_.current());
            cp.set(callstack_.current());
            regs.uval = 0;

            // Push call program entry.
            inline_push_callstack(cp, fp, nullptr, 0, ret_first);

            // Main loop
            bool done;
            do {
                op_push_i32(ip, sp);
                op_inline_call_short(ip, fp, cp, ret_00);
                goto fibonacci_n;
fibonacci_ret_00:
                {
                    op_pop_i32(ip, sp);
                    int retType = op_inline_ret(ip, fp, cp, done);
                    if (likely(done)) {
                        retVal.setDataType(return_type::Basic);
                        retVal.setValue(regs.eax.u32);
                        break;
                    }
                    else {
                        console.trace("Error: Unknown error.\n");
                    }
                    goto Execute_Finished;
                }
fibonacci_n:
                op_cmp_imm_i32(ip, fp);
                bool is_large = op_jl_near(ip);
                if (likely(!is_large)) {
                    op_add_sp_4(ip, sp);
                    op_push(ip, sp, fp);
                    op_dec(ip, fp);
                    op_inline_fast_call_short(ip, fp, cp, ret_01);
                    goto fibonacci_n;
fibonacci_ret_01:
                    op_copy_from_eax(ip, fp, regs);
                    op_dec(ip, fp);
                    op_inline_fast_call_short(ip, fp, cp, ret_02);
                    goto fibonacci_n;
fibonacci_ret_02:
                    op_add_eax(ip, fp, regs);
                    int retType = op_inline_ret_n(ip, fp, cp, done);
                    if (retType == ret_01)
                        goto fibonacci_ret_01;
                    else if (retType == ret_02)
                        goto fibonacci_ret_02;
                    else
                        goto fibonacci_ret_00;
                }
                else {
                    int retType = op_inline_ret_eax_n(ip, fp, cp, regs, done);
                    if (retType == ret_01)
                        goto fibonacci_ret_01;
                    else if (retType == ret_02)
                        goto fibonacci_ret_02;
                    else
                        goto fibonacci_ret_00;
                }
            } while (1);
        }

Execute_Finished:
        return ec;
    }

    int run(return_type & retVal) {
        ip_.set(image_.getPtr());
        fp_.set(stack_.current());
        return execute(retVal);
    }

    int run_inline(return_type & retVal) {
        ip_.set(image_.getPtr());
        fp_.set(stack_.current());
        return execute_inline(retVal);
    }
};

template <typename BasicType = uintptr_t>
class ExecutionEngine {
public:
    typedef BasicType                       basic_type;
    typedef size_t                          size_type;
    typedef ExecutionContext<basic_type>    context_type;
    typedef vmReturn<basic_type>            return_type;
    typedef ExecutionEngine<basic_type>     this_type;

private:
    vmBinaryFile binary_;
    context_type context_;

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

    int run_inline(return_type & ret) {
        binary_.setInput(ret.getValue());
        int ec = context_.run_inline(ret);
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

    int run_inline(return_type & ret) {
        int ec = engine_.run_inline(ret);
        return ec;
    }
};

} // namespace v4
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

#endif // JLANG_VM_INTERPRETER_V4_H
