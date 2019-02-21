
#ifndef JLANG_VM_INTERPRETER_V1_H
#define JLANG_VM_INTERPRETER_V1_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/vm/Interpreter.h"
#include "jlang/lang/ErrorCode.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#if !defined(_WIN32)
#include <sys/types.h>
#include <pthread.h>
#endif

#include <list>
#include <memory>
#include <atomic>

using namespace std;

namespace jlang {
namespace v1 {

//
// 00000000:    push ecx
// 00000003:    load ecx, 0x00000014 (uint32)
// 00000009:    call 0x00000020 (short offset 0x0017)
// 0000000D:    pop ecx
// 00000010:    ret

// 00000011:    nop; nop; nop;

// 00000020:    push ebx
// 00000023:    cmp ecx, 0x00000003
// 0000002B:    jl 0x00000050 (short offset 0x0025)

// 0000002F:    dec ecx
// 00000031:    push ecx
// 00000034:    call 0x00000020 (short offset 0xFFEC)

// 00000038:    move ebx, eax
// 0000003B:    pop ecx
// 0000003E:    dec ecx
// 00000040:    call 0x00000020 (short offset 0xFFE0)

// 00000044:    add eax, ebx
// 00000048:    pop ebx
// 0000004B:    ret

// 0000004C:    nop; nop; nop; nop;

// 00000050:    load eax, 0x00000001 (uint32)
// 00000056:    pop ebx
// 0000004B:    ret
//

static const unsigned char fibonacciBinary32[] = {
    // 00000000:    push ecx
    OpCode::push, vmDataType::Reg, vmReg::ecx,
    // 00000003:    load ecx, 0x00000014 (uint32)
    OpCode::load, vmReg::ecx, 0x14, 0x00, 0x00, 0x00,
    // 00000009:    call 0x00000020 (short offset 0x0017)
    OpCode::call, vmCallType::Short, 0x17, 0x00,
    // 0000000D:    pop ecx
    OpCode::pop,  vmDataType::Reg, vmReg::ecx,
    // 00000010:    ret
    OpCode::ret,

    // 00000011:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,
    // 00000014:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,
    // 00000018:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000001C:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,

    // 00000020:    push ebx
    OpCode::push, vmDataType::Reg, vmReg::ebx,
    // 00000023:    cmp ecx, 0x00000003
    OpCode::cmp,  vmComboType::Reg_Imm, vmDataType::UInt32,
                  vmReg::ecx, 0x03, 0x00, 0x00, 0x00,
    // 0000002B:    jl 0x00000050 (short offset 0x0025)
    OpCode::jl,   vmJumpType::Short, 0x25, 0x00,

    // 0000002F:    dec ecx
    OpCode::dec,  vmReg::ecx,
    // 00000031:    push ecx
    OpCode::push, vmDataType::Reg, vmReg::ecx,
    // 00000034:    call 0x00000020 (short offset 0xFFEC)
    OpCode::call, vmCallType::Short, 0xEC, 0xFF,

    // 00000038:    move ebx, eax
    OpCode::move, vmReg::ebx, vmReg::eax,

    // 0000003B:    pop ecx
    OpCode::pop,  vmDataType::Reg, vmReg::ecx,
    // 0000003E:    dec ecx
    OpCode::dec,  vmReg::ecx,
    // 00000040:    call 0x00000020 (short offset 0xFFE0)
    OpCode::call, vmCallType::Short, 0xE0, 0xFF,

    // 00000044:    add eax, ebx
    OpCode::add,  vmComboType::Reg_Reg, vmReg::eax, vmReg::ebx,
    // 00000048:    pop ebx
    OpCode::pop,  vmDataType::Reg, vmReg::ebx,
    // 0000004B:    ret
    OpCode::ret,

    // 0000004C:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,

    // 00000050:    load eax, 0x00000001 (uint32)
    OpCode::load, vmReg::eax, 0x01, 0x00, 0x00, 0x00,
    // 00000056:    pop ebx
    OpCode::pop,  vmDataType::Reg, vmReg::ebx,
    // 00000059:    ret
    OpCode::ret,

    // 0000005A:    nop; nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000005F:    exit
    OpCode::exit
};

static const unsigned char s_testBinary32[] = {
    // 00000000:    push eax
    OpCode::push, vmDataType::Reg, vmReg::eax,
    // 00000003:    push 0x0000000C (int32)
    OpCode::push, vmDataType::Int32, 0x0C, 0x00, 0x00, 0x00,
    // 00000009:    call 0x00000020 (short offset 0x0017)
    OpCode::call, vmCallType::Short, 0x17, 0x00,
    // 0000000D:    pop ebx
    OpCode::pop,  vmDataType::Reg, vmReg::ebx,
    // 00000010:    pop edx
    OpCode::pop,  vmDataType::Reg, vmReg::edx,
    // 00000013:    ret
    OpCode::ret,

    // 00000014:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,
    // 00000018:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000001C:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,

    // 00000020:    push ebx
    OpCode::push, vmDataType::Reg, vmReg::ebx,
    // 00000023:    jmp 0x00000033 (near offset 0x10)
    OpCode::jmp,  vmJumpType::Near, 0x10,
    // 00000026:    cmp eax, ebx
    OpCode::cmp,  vmComboType::Reg_Reg, vmDataType::UInt32,
                  vmReg::eax, vmReg::ebx,
    // 0000002B:    jne 0x00000045 (short offset 0x001A)
    OpCode::jne,   vmJumpType::Short, 0x1A, 0x00,
    // 0000002F:    push ebx
    OpCode::pop,  vmDataType::Reg, vmReg::ebx,
    // 00000032:    ret
    OpCode::ret,

    // 00000033:    jmp 0x00000036 (short offset 0x0004)
    OpCode::jmp,  vmJumpType::Short, 0x04, 0x00,
    // 00000037:    jmp 0x00000026 (long offset 0xFFFFFFEF)
    OpCode::jmp,  vmJumpType::Long, 0xEF, 0xFF, 0xFF, 0xFF,

    // 0000003D:    nop; nop;
    OpCode::nop,  OpCode::nop,
    // 0000003F:    jmp 0x00000026 (long offset 0xFFFFFFE7)
    OpCode::jmp,  vmJumpType::Long, 0xE7, 0xFF, 0xFF, 0xFF,

    // 00000045:    cmp eax, 0x20726178
    OpCode::cmp,  vmComboType::Reg_Imm, vmDataType::UInt32,
                  vmReg::eax, 0x78, 0x61, 0x72, 0x20,
    // 0000004D:    je 0x0000002F (short offset 0xFFE2)
    OpCode::je,   vmJumpType::Short, 0xE2, 0xFF,

    // 00000051:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,
    // 00000055:    exit
    OpCode::exit
};

static const unsigned char s_factorialBinary32[] = {
    // 00000000:    push eex
    OpCode::push, vmDataType::Reg, vmReg::eex,
    // 00000003:    load eex, 0x00000028 (uint32)
    OpCode::load, vmReg::eex, 0x12, 0x00, 0x00, 0x00,
    // 00000009:    call 0x00000020 (short offset 0x0017)
    OpCode::call, vmCallType::Short, 0x17, 0x00,
    // 0000000D:    pop eex
    OpCode::pop,  vmDataType::Reg, vmReg::eex,
    // 00000010:    ret
    OpCode::ret,

    // 00000011:    nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop,
    // 00000014:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,
    // 00000018:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000001C:    nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop,

    // 00000020:    push ebx
    OpCode::push, vmDataType::Reg, vmReg::ebx,
    // 00000023:    cmp eex, 0x00000001
    OpCode::cmp,  vmComboType::Reg_Imm, vmDataType::UInt32,
                  vmReg::eex, 0x01, 0x00, 0x00, 0x00,
    // 0000002B:    jle 0x00000040 (short offset 0x0015)
    OpCode::jle,  vmJumpType::Short, 0x15, 0x00,

    // 0000002F:    move ebx, eex
    OpCode::move, vmReg::ebx, vmReg::eex,
    // 00000032:    dec eex
    OpCode::dec,  vmReg::eex,
    // 00000034:    call 0x00000020 (short offset 0xFFEC)
    OpCode::call, vmCallType::Short, 0xEC, 0xFF,

    // 00000038:    mul eax, ebx
    OpCode::mul,  vmComboType::Reg_Reg, vmReg::eax, vmReg::ebx,
    // 0000003C:    pop ebx
    OpCode::pop,  vmDataType::Reg, vmReg::ebx,
    // 0000003F:    ret
    OpCode::ret,

    // 00000040:    load eax, 0x00000001 (uint32)
    OpCode::load, vmReg::eax, 0x01, 0x00, 0x00, 0x00,
    // 00000046:    pop ebx
    OpCode::pop,  vmDataType::Reg, vmReg::ebx,
    // 00000049:    ret
    OpCode::ret,

    // 0000004A:    nop; nop; nop; nop; nop;
    OpCode::nop,  OpCode::nop, OpCode::nop, OpCode::nop, OpCode::nop,
    // 0000004F:    exit
    OpCode::exit
};

static bool verifyRegType(uint32_t regType, uint32_t dataType) {
    switch (dataType) {
    case vmDataType::Int8:
    case vmDataType::UInt8:
        return (regType == vmRegType::r8);

    case vmDataType::Int16:
    case vmDataType::UInt16:
        return (regType == vmRegType::r16);

    case vmDataType::Int32:
    case vmDataType::UInt32:
        return (regType == vmRegType::r32);

    case vmDataType::Int64:
    case vmDataType::UInt64:
        return (regType == vmRegType::r64);

    case vmDataType::Pointer:
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        return (regType == vmRegType::r64);
#else
        return (regType == vmRegType::r32);
#endif

    default:
        return false;
    }
}

static bool verifyRegById(reg_t reg, uint32_t dataType) {
    uint32_t regType = vmReg::getType(reg);
    return verifyRegType(regType, dataType);
}

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
        uint32_t * pInitValue = (uint32_t *)&(imageData[5]);
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

#if defined(_WIN32)

typedef DWORD   ThreadId_t;
typedef HANDLE  ThreadHandle_t;

typedef DWORD   ProcessId_t;
typedef HANDLE  ProcessHandle_t;

#else

typedef pid_t       ThreadId_t;
typedef pthread_t   ThreadHandle_t;

typedef pid_t       ProcessId_t;
typedef pthread_t   ProcessHandle_t;

#endif // _WIN32

template <typename BasicType>
class ExecutionEngine;

template <typename BasicType>
class vmThreadBase {
public:
    typedef BasicType                       basic_type;
    typedef size_t                          size_type;
    typedef ExecutionEngine<basic_type>  engine_type;
    typedef vmReturn<basic_type>            return_type;
    typedef vmThreadBase<basic_type>        this_type;

private:
    vmThreadId              id_;
    vmFrame<basic_type>     frame_;
    vmStack<basic_type>     stack_;
    vmHeap<basic_type>      heap_;
    engine_type *           engine_;

    static std::atomic<vmThreadId> thread_id_cnt;

public:
    vmThreadBase(engine_type * engine = nullptr) : id_(0), engine_(engine) {
        frame_.setStack(&stack_);
        stack_.setFrame(&frame_);
    }
    virtual ~vmThreadBase() {
        destroy();
    }

    bool isInited() const { return (id_ != 0); }

    engine_type * getEngine() { return engine_; }
    void setEngine(engine_type * engine) {
        engine_ = engine;
    }

    vmThreadId getId() { return id_; }
    void setId(vmThreadId id) {
        id_ = id;
    }

    ThreadId_t getThreadId() const {
#if defined(_WIN32)
        return ::GetCurrentThreadId();
#else
        return getpid();
#endif
    }

    ThreadHandle_t getThreadHandle() const {
#if defined(_WIN32)
        return ::GetCurrentThread();
#else
        return pthread_self();
#endif
    }

    void setImageInfo(void * imageStart, size_t imageSize,
                      void * imageEntry) {
        frame_.setting(imageStart, imageSize, imageEntry);
    }

    inline vmThreadId increaseThreadId() {
        this_type::thread_id_cnt++;
        return this_type::thread_id_cnt.load();
    }

    vmThreadId create(bool isMainThread, size_type stackSize) {
        vmThreadId id = increaseThreadId();
        stack_.create(stackSize);
        id_ = id;
        return id;
    }

    void destroy() {
        stack_.destroy();
        id_ = 0;
    }

    int run(return_type & retValue) {
        assert(isInited());
        if (frame_.isInited() && stack_.isInited()) {
            // Call program entry.
            stack_.push_callstack(nullptr);
            // Main loop
            while (!frame_.isEof()) {
                uint32_t offset = frame_.getFPOffset();
                unsigned char opcode = frame_.get();
                switch (opcode) {
                case OpCode::error:
                    frame_.next();
                    console.trace("%08X:  error\n", offset);
                    break;

                case OpCode::push:
                    {
                        frame_.next();
                        unsigned char type = frame_.get();
                        frame_.next();
                        if (type == vmDataType::Reg) {
                            reg_t reg = (reg_t)frame_.get();
                            uint32_t regType = vmReg::getType(reg);
                            uint32_t regIndex = vmReg::getIndex(reg);
                            if (vmReg::isValidRegister(reg)) {
                                basic_type value = stack_.push_reg(reg);
                                if (regType >= vmRegType::r64) {
                                    console.trace("%08X:  push [reg]  - (%u, %u), value = 0x%016X\n",
                                                  offset, regType, regIndex, (uint64_t)value);
                                }
                                else {
                                    console.trace("%08X:  push [reg]  - (%u, %u), value = 0x%08X\n",
                                                  offset, regType, regIndex,
                                                  (uint32_t)(value & 0xFFFFFFFFUL));
                                }
                                frame_.next();
                            }
                            else {
                                console.trace("%08X:  push [reg]  - regType: %u, regIndex: %u.\n"
                                              "Error: Unknown push register.\n\n",
                                              offset, regType, regIndex);
                                frame_.next();
                            }
                        }
                        else if (type == vmDataType::Data) {
                            uint32_t data = frame_.getUInt32();
                            bool success = stack_.push_uint32(data);
                            console.trace("%08X:  push [data] - data = %u\n", offset, data);
                            frame_.nextUInt32();
                        }
                        else if (type >= vmDataType::Int8 && type <= vmDataType::UInt64) {
                            switch (type) {
                            case vmDataType::Int8:
                                {
                                    break;
                                }
                            case vmDataType::UInt8:
                                {
                                    break;
                                }
                            case vmDataType::Int16:
                                {
                                    break;
                                }
                            case vmDataType::UInt16:
                                {
                                    break;
                                }
                            case vmDataType::Int32:
                                {
                                    int32_t data = frame_.getInt32();
                                    bool success = stack_.push_int32(data);
                                    console.trace("%08X:  push [int32] - value = 0x%08X\n", offset, data);
                                    frame_.nextInt32();
                                    break;
                                }
                            case vmDataType::UInt32:
                                {
                                    uint32_t data = frame_.getUInt32();
                                    bool success = stack_.push_uint32(data);
                                    console.trace("%08X:  push [uint32] - value = 0x%08X\n", offset, data);
                                    frame_.nextUInt32();
                                    break;
                                }
                            case vmDataType::Int64:
                                {
                                    int64_t data = frame_.getInt64();
                                    bool success = stack_.push_int64(data);
                                    console.trace("%08X:  push [int64] - value = 0x%08X%08X\n", offset,
                                                (uint32_t)(data >> 32), (uint32_t)(data && 0xFFFFFFFFL));
                                    frame_.nextInt64();
                                    break;
                                }
                            case vmDataType::UInt64:
                                {
                                    uint64_t data = frame_.getUInt64();
                                    bool success = stack_.push_uint64(data);
                                    console.trace("%08X:  push [uint64] - value = 0x%08X%08X\n", offset,
                                                  (uint32_t)(data >> 32), (uint32_t)(data && 0xFFFFFFFFL));
                                    frame_.nextUInt64();
                                    break;
                                }
                            default:
                                break;
                            }
                        }
                        break;
                    }

                case OpCode::pop:
                    {
                        frame_.next();
                        unsigned char type = frame_.get();
                        frame_.next();
                        if (type == vmDataType::Reg) {
                            reg_t reg = (reg_t)frame_.get();
                            if (vmReg::isValidRegister(reg)) {
                                basic_type value = stack_.pop_reg(reg);
                                if (vmReg::getType(reg) >= vmRegType::r64) {
                                    console.trace("%08X:  pop  [reg] - (%u, %u), value = 0x%016X\n",
                                                  offset, vmReg::getType(reg), vmReg::getIndex(reg),
                                                  (uint64_t)value);
                                }
                                else {
                                    console.trace("%08X:  pop  [reg] - (%u, %u), value = 0x%08X\n",
                                                  offset, vmReg::getType(reg), vmReg::getIndex(reg),
                                                  (uint32_t)(value & 0xFFFFFFFFUL));
                                }
                                frame_.next();
                            }
                            else {
                                console.trace("%08X:  pop  [reg] - regType: %u, regIndex: %u\n"
                                              "Error: Unknown pop register.\n\n",
                                              offset, vmReg::getType(reg), vmReg::getIndex(reg));
                                frame_.next();
                            }
                        }
                        else if (type == vmDataType::Int32 || type == vmDataType::UInt32) {
                            // pop int32
                            int32_t value;
                            bool success = stack_.pop_int32(value);
                            console.trace("%08X:  pop  [int32] - value = 0x%08X\n",
                                          offset, value);
                        }
                        else if (type == vmDataType::Int64 || type == vmDataType::UInt64) {
                            // pop int64
                            int64_t value;
                            bool success = stack_.pop_int64(value);
                            console.trace("%08X:  pop  [int64] - value = 0x%016X\n",
                                          offset, value);
                        }
                        else {
                            // Error
                            console.trace("%08X:  pop  - type: %u\n"
                                          "Error: Unknown pop type.\n\n",
                                          offset, type);
                        }
                        break;
                    }

                case OpCode::load:
                    {
                        frame_.next();
                        reg_t reg = (reg_t)frame_.get();
                        frame_.next();
                        uintptr_t value = frame_.loadRegValue(reg);
                        if (vmReg::getType(reg) >= vmRegType::r64) {
                            console.trace("%08X:  load [reg] - (%u, %u), value = 0x%016X\n",
                                          offset, vmReg::getType(reg), vmReg::getIndex(reg),
                                          (uint64_t)value);
                        }
                        else {
                            console.trace("%08X:  load [reg] - (%u, %u), value = 0x%08X\n",
                                          offset, vmReg::getType(reg), vmReg::getIndex(reg),
                                          (uint32_t)(value & 0xFFFFFFFFUL));
                        }
                        break;
                    }

                case OpCode::move:
                    {
                        frame_.next();
                        reg_t reg1 = (reg_t)frame_.get();
                        frame_.next();
                        reg_t reg2 = (reg_t)frame_.get();
                        frame_.next();
                        uintptr_t value = frame_.moveRegValue(reg1, reg2);
                        if (vmReg::getType(reg1) >= vmRegType::r64) {
                            console.trace("%08X:  move [reg] - (%u, %u), value = 0x%016X\n",
                                          offset, vmReg::getIndex(reg1), vmReg::getIndex(reg2),
                                          (uint64_t)value);
                        }
                        else {
                            console.trace("%08X:  move [reg] - (%u, %u), value = 0x%08X\n",
                                          offset, vmReg::getIndex(reg1), vmReg::getIndex(reg2),
                                          (uint32_t)(value & 0xFFFFFFFFUL));
                        }
                        break;
                    }

                case OpCode::store:
                    {
                        frame_.next();
                        console.trace("%08X:  store\n", offset);
                        break;
                    }

                case OpCode::cmp:
                    {
                        frame_.next();
                        unsigned char cmpType = frame_.get();
                        if (cmpType == vmComboType::Reg_Reg) {
                            frame_.next();
                            unsigned char dataType = frame_.get();
                            frame_.next();

                            reg_t reg1 = (reg_t)frame_.get();
                            assert(verifyRegById(reg1, dataType));
                            frame_.next();

                            reg_t reg2 = (reg_t)frame_.get();
                            assert(verifyRegById(reg1, dataType));
                            frame_.next();

                            console.trace("%08X:  cmp  [reg], [reg] (%u - %u)\n",
                                          offset, reg1, reg2);

                            // Condition jump code: jz, jnz, je, jne, jg, jge, jl, jle.
                            unsigned char condJmp = frame_.get();
                            assert(condJmp >= OpCode::cond_jmp_first &&
                                   condJmp <= OpCode::cond_jmp_last);

                            bool condition = frame_.condCmp_rr(reg1, reg2, dataType, condJmp);
                            if (!condition) {
                                console.trace("%08X:  jmp  condition [false, opcode = %u]\n",
                                              frame_.getFPOffset(), (uint32_t)condJmp);
                                frame_.next();
                                unsigned char jumpType = frame_.get();
                                frame_.next();

                                SKIP_JMP_TYPE(offset, frame_, jumpType);
                            }
                            else {
                                console.trace("%08X:  jmp  condition [true, opcode = %u]\n",
                                              frame_.getFPOffset(), (uint32_t)condJmp);
                                opcode = condJmp;
                                offset = frame_.getFPOffset();
                                goto JMP_START;
                            }
                        }
                        else if (cmpType == vmComboType::Reg_Imm) {
                            frame_.next();
                            unsigned char dataType = frame_.get();
                            frame_.next();

                            reg_t reg = (reg_t)frame_.get();
                            assert(verifyRegById(reg, dataType));
                            frame_.next();

                            Integer value;
                            value.uval = frame_.getValue(dataType);
                            frame_.nextValue(dataType);
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
                            if (dataType == vmDataType::Int64 || dataType == vmDataType::UInt64) {
                                console.trace("%08X:  cmp  [reg], [imm] (%u - 0x%016X)\n",
                                              offset, reg, value.uval);
                            }
                            else {
                                console.trace("%08X:  cmp  [reg], [imm] (%u - 0x%08X)\n",
                                              offset, reg, value.u32.low);
                            }
#else
                            console.trace("%08X:  cmp  [reg], [imm] (%u - 0x%08X)\n",
                                          offset, reg, value.uval);
#endif
                            // Condition jump code: jz, jnz, je, jne, jg, jge, jl, jle.
                            unsigned char condJmp = frame_.get();
                            assert(condJmp >= OpCode::cond_jmp_first &&
                                   condJmp <= OpCode::cond_jmp_last);

                            bool condition = frame_.condCmp_ri(reg, value.uval, dataType, condJmp);
                            if (!condition) {
                                console.trace("%08X:  jmp  condition [false, opcode = %u]\n",
                                              frame_.getFPOffset(), (uint32_t)condJmp);
                                frame_.next();
                                unsigned char jumpType = frame_.get();
                                frame_.next();

                                SKIP_JMP_TYPE(offset, frame_, jumpType);
                            }
                            else {
                                console.trace("%08X:  jmp  condition [true, opcode = %u]\n",
                                              frame_.getFPOffset(), (uint32_t)condJmp);
                                opcode = condJmp;
                                offset = frame_.getFPOffset();
                                goto JMP_START;
                            }
                        }
                        else {
                            frame_.next();
                        }
                        break;
                    }

                case OpCode::jmp:
JMP_START:
                    {
                        unsigned char * jmp_fp = frame_.getFP();
                        frame_.next();
                        unsigned char jumpType = frame_.get();
                        frame_.next();
                        switch (jumpType) {
                        case vmJumpType::Near:
                            {
                                // Get 16 byte (-32768 -- 32767) address offset.
                                int8_t offset = frame_.getInt8();
                                frame_.jumpNear(jmp_fp, offset);
                                console.trace("%08X:  jmp  0x%08X (near)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
                        case vmJumpType::Short:
                            {
                                // Get 16 byte (-32768 -- 32767) address offset.
                                int16_t offset = frame_.getInt16();
                                frame_.jumpShort(jmp_fp, offset);
                                console.trace("%08X:  jmp  0x%08X (short)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
                        case vmJumpType::Long:
                            {
                                // Get 32 byte (-2147483648 -- 2147483647) address offset.
                                int32_t offset = frame_.getInt32();
                                frame_.jumpLong(jmp_fp, offset);
                                console.trace("%08X:  jmp  0x%08X (long)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
                        case vmJumpType::Ptr32:
                            {
                                // Get 32 byte absolute address.
                                uint32_t jumpAddr = frame_.getUInt32();
                                frame_.jumpPtr32(jmp_fp, jumpAddr);
                                console.trace("%08X:  jmp  0x%08X (Ptr32)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
                        case vmJumpType::Ptr64:
                            {
                                // Get 64 byte absolute address.
                                uint64_t jumpAddr = frame_.getUInt64();
                                frame_.jumpPtr64(jmp_fp, jumpAddr);
                                console.trace("%08X:  jmp  0x%08X (Ptr64)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
#endif
                        default:
                            {
                                // Unknown jump type
                                console.trace("%08X:  jmp\t"
                                              "Error: Unknown jump type. jumpType = %u\n",
                                              offset, (uint32_t)jumpType);
                                break;
                            }
                        }
                        console.trace("\n");
                        break;
                    }

                case OpCode::call:
                    {
                        unsigned char * call_fp = frame_.getFP();
                        frame_.next();
                        unsigned char callType = frame_.get();
                        frame_.next();
                        switch (callType) {
                        case vmCallType::Short:
                            {
                                // Get 16 byte (-32768 -- 32767) address offset.
                                int16_t callOffset = frame_.getInt16();
                                frame_.callShort(call_fp, 2 + sizeof(int16_t), callOffset);
                                console.trace("%08X:  call 0x%08X (short)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
                        case vmCallType::Long:
                            {
                                // Get 32 byte (-2147483648 -- 2147483647) address offset.
                                int32_t callOffset = frame_.getInt32();
                                frame_.callLong(call_fp, 2 + sizeof(int32_t), callOffset);
                                console.trace("%08X:  call 0x%08X (long)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
                        case vmCallType::Ptr32:
                            {
                                // Get 32 byte absolute address.
                                uint32_t callEntry = frame_.getUInt32();
                                frame_.callPtr32(call_fp, 2 + sizeof(uint32_t), callEntry);
                                console.trace("%08X:  call 0x%08X (Ptr32)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
                        case vmCallType::Ptr64:
                            {
                                // Get 64 byte absolute address.
                                uint64_t callEntry = frame_.getUInt64();
                                frame_.callPtr64(call_fp, 2 + sizeof(uint64_t), callEntry);
                                console.trace("%08X:  call 0x%08X (Ptr64)\n",
                                              offset, frame_.getFPOffset());
                                break;
                            }
#endif
                        default:
                            {
                                // Unknown call type
                                console.trace("%08X:  call\t"
                                              "Error: Unknown call type. callType = %u\n",
                                              offset, (uint32_t)callType);
                                break;
                            }
                        }
                        console.trace("\n");
                        break;
                    }

                case OpCode::ret:
                    {
                        frame_.next();
                        unsigned char * returnFP = frame_.callReturn();
                        if (returnFP != nullptr) {
                            console.trace("%08X:  ret  0x%08X\n\n", offset, frame_.getFPOffset());
                        }
                        else {
                            console.trace("%08X:  ret  (done)\n\n", offset);
                            retValue.setDataType(return_type::Basic);
                            retValue.setValue(frame_.getRegValue32(vmRegId::eax));
                            goto Execute_Finished;
                        }
                        break;
                    }

                case OpCode::inc:
                    {
                        frame_.next();
                        reg_t reg = (reg_t)frame_.get();
                        frame_.next();
                        uintptr_t value = frame_.incRegValue(reg);
                        if (vmReg::getType(reg) >= vmRegType::r64) {
                            console.trace("%08X:  inc  [reg] - (%u, %u), value = 0x%016X\n",
                                          offset, vmReg::getType(reg), vmReg::getIndex(reg),
                                          (uint64_t)value);
                        }
                        else {
                            console.trace("%08X:  inc  [reg] - (%u, %u), value = 0x%08X\n",
                                          offset, vmReg::getType(reg), vmReg::getIndex(reg),
                                          (uint32_t)(value & 0xFFFFFFFFUL));
                        }
                        break;
                    }

                case OpCode::dec:
                    {
                        frame_.next();
                        reg_t reg = (reg_t)frame_.get();
                        frame_.next();
                        uintptr_t value = frame_.decRegValue(reg);
                        if (vmReg::getType(reg) >= vmRegType::r64) {
                            console.trace("%08X:  dec  [reg] - (%u, %u), value = 0x%016X\n",
                                          offset, vmReg::getType(reg), vmReg::getIndex(reg),
                                          (uint64_t)value);
                        }
                        else {
                            console.trace("%08X:  dec  [reg] - (%u, %u), value = 0x%08X\n",
                                          offset, vmReg::getType(reg), vmReg::getIndex(reg),
                                          (uint32_t)(value & 0xFFFFFFFFUL));
                        }
                        break;
                    }

                case OpCode::add:
                    {
                        frame_.next();
                        unsigned char addType = frame_.get();
                        if (addType == vmComboType::Reg_Reg) {
                            frame_.next();
                            reg_t reg1 = (reg_t)frame_.get();
                            frame_.next();
                            reg_t reg2 = (reg_t)frame_.get();
                            frame_.next();
                            uintptr_t value = frame_.addRegValue(reg1, reg2);
                            if (vmReg::getType(reg1) >= vmRegType::r64) {
                                console.trace("%08X:  add  [reg, reg] - (%u, %u), value = 0x%016X\n",
                                              offset, vmReg::getIndex(reg1), vmReg::getIndex(reg2),
                                              (uint64_t)value);
                            }
                            else {
                                console.trace("%08X:  add  [reg, reg] - (%u, %u), value = 0x%08X\n",
                                              offset, vmReg::getIndex(reg1), vmReg::getIndex(reg2),
                                              (uint32_t)(value & 0xFFFFFFFFUL));
                            }
                        }
                        else if (addType == vmComboType::Reg_Imm) {
                            frame_.next();
                            reg_t reg = (reg_t)frame_.get();
                            frame_.next();

                            Integer value;
                            value.uval = frame_.getValueByReg(vmReg::getType(reg));
                            frame_.nextValueByReg(vmReg::getType(reg));

                            uintptr_t newValue = frame_.addRegValue_ri(reg, value.uval);
                            if (vmReg::getType(reg) >= vmRegType::r64) {
                                console.trace("%08X:  add  [reg, imm] - (%u), value = 0x%016X\n",
                                              offset, vmReg::getIndex(reg),
                                              newValue);
                            }
                            else {
                                console.trace("%08X:  add  [reg, imm] - (%u), value = 0x%08X\n",
                                              offset, vmReg::getIndex(reg),
                                              (uint32_t)(newValue & 0xFFFFFFFFUL));
                            }
                        }
                        else {
                            frame_.next();
                        }
                        break;
                    }

                case OpCode::sub:
                    {
                        frame_.next();
                        unsigned char subType = frame_.get();
                        if (subType == vmComboType::Reg_Reg) {
                            frame_.next();
                            reg_t reg1 = (reg_t)frame_.get();
                            frame_.next();
                            reg_t reg2 = (reg_t)frame_.get();
                            frame_.next();
                            uintptr_t value = frame_.subRegValue(reg1, reg2);
                            if (vmReg::getType(reg1) >= vmRegType::r64) {
                                console.trace("%08X:  sub  [reg, reg] - (%u, %u), value = 0x%016X\n",
                                              offset, vmReg::getIndex(reg1), vmReg::getIndex(reg2),
                                              (uint64_t)value);
                            }
                            else {
                                console.trace("%08X:  sub  [reg, reg] - (%u, %u), value = 0x%08X\n",
                                              offset, vmReg::getIndex(reg1), vmReg::getIndex(reg2),
                                              (uint32_t)(value & 0xFFFFFFFFUL));
                            }
                        }
                        else if (subType == vmComboType::Reg_Imm) {
                            frame_.next();
                            reg_t reg = (reg_t)frame_.get();
                            frame_.next();

                            Integer value;
                            value.uval = frame_.getValueByReg(vmReg::getType(reg));
                            frame_.nextValueByReg(vmReg::getType(reg));

                            uintptr_t newValue = frame_.subRegValue_ri(reg, value.uval);
                            if (vmReg::getType(reg) >= vmRegType::r64) {
                                console.trace("%08X:  sub  [reg, imm] - (%u), value = 0x%016X\n",
                                              offset, vmReg::getIndex(reg),
                                              newValue);
                            }
                            else {
                                console.trace("%08X:  sub  [reg, imm] - (%u), value = 0x%08X\n",
                                              offset, vmReg::getIndex(reg),
                                              (uint32_t)(newValue & 0xFFFFFFFFUL));
                            }
                        }
                        else {
                            frame_.next();
                        }
                        break;
                    }

                case OpCode::mul:
                    {
                        frame_.next();
                        unsigned char mulType = frame_.get();
                        if (mulType == vmComboType::Reg_Reg) {
                            frame_.next();
                            reg_t reg1 = (reg_t)frame_.get();
                            frame_.next();
                            reg_t reg2 = (reg_t)frame_.get();
                            frame_.next();
                            uintptr_t value = frame_.mulRegValue(reg1, reg2);
                            if (vmReg::getType(reg1) >= vmRegType::r64) {
                                console.trace("%08X:  mul  [reg, reg] - (%u, %u), value = 0x%016X\n",
                                              offset, vmReg::getIndex(reg1), vmReg::getIndex(reg2),
                                              (uint64_t)value);
                            }
                            else {
                                console.trace("%08X:  mul  [reg, reg] - (%u, %u), value = 0x%08X\n",
                                              offset, vmReg::getIndex(reg1), vmReg::getIndex(reg2),
                                              (uint32_t)(value & 0xFFFFFFFFUL));
                            }
                        }
                        else if (mulType == vmComboType::Reg_Imm) {
                            frame_.next();
                        }
                        else {
                            frame_.next();
                        }
                        break;
                    }

                case OpCode::nop:
                    frame_.next();
                    console.trace("%08X:  nop\n", offset);
                    break;

                case OpCode::exit:
                    frame_.next();
                    console.trace("%08X:  end\n\n", offset);
                    retValue.setDataType(return_type::Basic);
                    retValue.setValue(frame_.getRegValue32(vmRegId::eax));
                    goto Execute_Finished;

                default:
                    frame_.next();

                    break;
                }
            }
Execute_Finished:
            (void *)(0);
        }

        return 0;
    }
};

template <typename BasicType>
std::atomic<vmThreadId> vmThreadBase<BasicType>::thread_id_cnt(0);

template <typename BasicType = uintptr_t>
class vmThread : public vmThreadBase<BasicType> {
public:
    typedef BasicType                       basic_type;
    typedef vmThreadBase<basic_type>        base_type;
    typedef typename base_type::size_type   size_type;
    typedef vmThread<basic_type>            this_type;

    static const size_type kDefaultStackSize = 2 * 1048576U;

public:
    vmThread() : vmThreadBase<basic_type>() {}
    virtual ~vmThread() {
    }

    bool isMainThread() const { return false; }

    vmThreadId create(size_type stackSize = kDefaultStackSize) {
        return create(false, stackSize);
    }

    void start() {
        //startThread();
    }

    void stop() {
        //stopThread();
    }

    void suspend() {
        //suspendThread();
    }

    void resume() {
        //resumeThread();
    }

    void terminate(uint32_t exitCode) {
        this->destroy();
#if defined(_WIN32)
        ::TerminateThread(getThreadHandle(), exitCode);
#endif
    }
};

template <typename BasicType = uintptr_t>
class vmProcess : public vmThreadBase<BasicType> {
public:
    typedef BasicType                       basic_type;
    typedef vmThreadBase<basic_type>        base_type;
    typedef typename base_type::size_type   size_type;
    typedef vmThread<basic_type>            thread_type;
    typedef vmProcess<basic_type>           this_type;

    static const size_type kDefaultStackSize = 8 * 1048576U;

public:
    vmProcess() : vmThreadBase<basic_type>() {}
    virtual ~vmProcess() {
    }

    bool isMainThread() const { return true; }

    ProcessId_t getProcessId() const {
#if defined(_WIN32)
        return ::GetCurrentProcessId();
#else
        return getpid();
#endif
    }

    ProcessHandle_t getProcessHandle() const {
#if defined(_WIN32)
        return ::GetCurrentProcess();
#else
        return pthread_self();
#endif
    }

    vmThreadId create(size_type stackSize = kDefaultStackSize) {
        return base_type::create(true, stackSize);
    }

    void terminate(int exitCode) {
        this->destroy();
        ::exit(exitCode);
    }
};

template <typename BasicType = uintptr_t>
class ExecutionEngine {
public:
    typedef BasicType                   basic_type;
    typedef size_t                      size_type;
    typedef vmProcess<basic_type>       context_type;
    typedef vmReturn<basic_type>        return_type;
    typedef ExecutionEngine<basic_type> this_type;

private:
    vmBinaryFile binary_;
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

} // namespace v1
} // namespace jlang

#endif // JLANG_VM_INTERPRETER_V1_H
