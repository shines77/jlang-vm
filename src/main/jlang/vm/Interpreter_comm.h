
#ifndef JLANG_VM_INTERPRETER_COMMON_H
#define JLANG_VM_INTERPRETER_COMMON_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include <cstdint>
#include <list>
#include <memory>
#include <atomic>

//////////////////////////////////////////////////////////////

#define USE_VMSTACK_CALLSTACK       1

#define USE_TEST_IMAGE              0
#define USE_FACTORIAL_IMAGE         1
#define USE_FIBONACCI_IMAGE         2
#define USE_FIBONACCI_IMAGE_NEW     3
#define USE_FIBONACCI_IMAGE_FAST    4

#define BINARY_IMAGE_MODE           USE_FIBONACCI_IMAGE_FAST

//////////////////////////////////////////////////////////////

#ifndef NDEBUG
#define USE_DEBUG_PRINT         1
#else
#define USE_DEBUG_PRINT         0
#endif
#define USE_REGS_TEST           1

//////////////////////////////////////////////////////////////

#define MakeComboType(t1, t2)   (((t1) * 16) | (t2))

#define VM_STACK_PUSH(sp, type) \
    sp = (unsigned char *)((char *)(sp) + sizeof(type))

#define VM_STACK_POP(sp, type) \
    sp = (unsigned char *)((char *)(sp) - sizeof(type));

#define ADDR_ALIGNMENT          16
#define CHECK_ADDR_ALIGNMENT(addr)  \
    (((uintptr_t)(ptrdiff_t)(addr) & (uintptr_t)((ADDR_ALIGNMENT) - 1)) == 0)

//////////////////////////////////////////////////////////////

#define SKIP_JMP_TYPE32(offset, frame, jumpType)    \
    switch (jumpType) {                             \
    case vmJumpType::Near:                          \
        frame.nextInt8();                           \
        break;                                      \
    case vmJumpType::Short:                         \
        frame.nextInt16();                          \
        break;                                      \
    case vmJumpType::Long:                          \
        frame.nextInt32();                          \
        break;                                      \
    case vmJumpType::Ptr32:                         \
        frame.nextInt32();                          \
        break;                                      \
    default:                                        \
        assert(false);                              \
        Debug.print("%08X:  jmp \t"                 \
                    "Error: Unknown jump type. jumpType = %u\n", \
                    offset, (uint32_t)jumpType);    \
        break;                                      \
    }

#define SKIP_JMP_TYPE64(offset, frame, jumpType)    \
    switch (jumpType) {                             \
    case vmJumpType::Near:                          \
        frame.nextInt8();                           \
        break;                                      \
    case vmJumpType::Short:                         \
        frame.nextInt16();                          \
        break;                                      \
    case vmJumpType::Long:                          \
        frame.nextInt32();                          \
        break;                                      \
    case vmJumpType::Ptr32:                         \
        frame.nextInt32();                          \
        break;                                      \
    case vmJumpType::Ptr64:                         \
        frame.nextInt64();                          \
        break;                                      \
    default:                                        \
        assert(false);                              \
        Debug.print("%08X:  jmp \t"                 \
                    "Error: Unknown jump type. jumpType = %u\n", \
                    offset, (uint32_t)jumpType);    \
        break;                                      \
    }

//////////////////////////////////////////////////////////////

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
#define SKIP_JMP_TYPE   SKIP_JMP_TYPE64
#else
#define SKIP_JMP_TYPE   SKIP_JMP_TYPE32
#endif // _WIN64

//////////////////////////////////////////////////////////////

namespace jlang {

#if !defined(_WIN32)
typedef unsigned long   DWORD;
typedef void *          HANDLE;
#endif // !_WIN32

class DebugOutput {
public:
    DebugOutput() {}
    ~DebugOutput() {}

    void print(const char * fmt, ...) {
#if USE_DEBUG_PRINT
        va_list args;
        va_start(args, fmt);
        this->vprint(fmt, args);
        va_end(args);
#endif
    }

    void vprint(const char * fmt, va_list args) {
#if USE_DEBUG_PRINT
        vprintf(fmt, args);
#endif
    }
};

static DebugOutput Debug;

struct IntWrapper32 {
    int32_t low;
};

struct UIntWrapper32 {
    uint32_t low;
};

struct IntWrapper64 {
    int32_t low;
    int32_t high;
};

struct UIntWrapper64 {
    uint32_t low;
    uint32_t high;
};

struct Integer32 {
    union {
        struct IntWrapper32 i32;
        struct UIntWrapper32 u32;
        int32_t ival;
        uint32_t uval;
    };
};

struct Integer64 {
    union {
        struct IntWrapper64 i32;
        struct UIntWrapper64 u32;
        int64_t ival;
        uint64_t uval;
    };
};

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
typedef Integer64 Integer;
#else
typedef Integer32 Integer;
#endif

class OpCode {
public:
    enum Type {
        error,
        push,
        push_i32,
        push_i64,
        push_u32 = push_i32,
        push_u64 = push_i64,
        push_i32_0,
        push_i64_0,
        push_u32_0 = push_i32_0,
        push_u64_0 = push_i64_0,
        pop,
        pop_i32,
        pop_i64,
        pop_u32 = pop_i32,
        pop_u64 = pop_i64,
        add_sp,
        add_sp_4,
        add_sp_8,
        sub_sp,
        sub_sp_4,
        sub_sp_8,
        load,
        load_eax,
        move,
        move_to_eax,
        copy_to_eax,
        store,
        cmp,
        cmp_i32,
        cmp_u32,
        cmp_imm_i32,
        cmp_imm_u32,
        cmp_i64,
        cmp_u64,
        cmp_imm_i64,
        cmp_imm_u64,
        test,
        jz,
        jnz,
        je,
        jne,
        jl,
        jl_near,
        jl_short,
        jl_long,
        jle,
        jle_short,
        jg,
        jge,
        js,
        jns,
        jmp,
        jmp_near,
        jmp_short,
        jmp_long,
        call,
        call_near,
        call_short,
        call_long,
        ret,
        ret_n_sm,
        ret_n,
        ret_eax,
        ret_eax_n,
        nop,
        nop_n,
        inc,
        dec,
        add,
        add_imm,
        add_eax,
        add_eax_imm,
        sub,
        sub_imm,
        sub_eax,
        sub_eax_imm,
        mul,
        imul,
        div,
        idiv,
        push_all,
        pop_all,
        exit,
        last,

        cond_jmp_first = jz,
        cond_jmp_last = jge,
    };
};

struct vmDataType {
    enum Type {
        Int8,
        UInt8,
        Int16,
        UInt16,
        Int32,
        UInt32,
        Int = Int32,
        UInt = UInt32,
        Int64,
        UInt64,
        Pointer,
        Void,
        Data,
        Reg,
        Last
    };
};

struct vmCallType {
    enum Type {
        Short,
        Long,
        Ptr32,
        Ptr64,
        Last
    };
};

struct vmJumpType {
    enum Type {
        Near,
        Short,
        Long,
        Ptr32,
        Ptr64,
        Last
    };
};

struct vmComboMask {
    enum Type {
        None = 0,
        Reg = 1,
        Imm = 2,
        Addr = 4,
        Max = 8,
    };
};

struct vmComboType {
    enum Type {
        Reg_Reg = MakeComboType(vmComboMask::Reg, vmComboMask::Reg),
        Reg_Imm = MakeComboType(vmComboMask::Reg, vmComboMask::Imm),
        Reg_Addr = MakeComboType(vmComboMask::Reg, vmComboMask::Addr),
        Last
    };
};

struct vmCondType {
    enum Type {
        jz,
        jnz,
        je,
        jne,
        jl,
        jle,
        jg,
        jge,
        cond_first = jz,
        cond_last = jge,
        last
    };
};

typedef uint32_t reg_t;

struct vmRegType {
    enum Type {
        r8,
        r8_high,
        r16,
        r32,
        r64,
        r128,
        r256,
        r512,
        last
    };
};

struct vmReg {
    // A total of 32 registers.
    enum Type {
        first = 0,

        // low 8 bit register
        spl = first, bpl, sil, dil,

        al, bl, cl, dl,
        el, fl, gl, hl,
        il, jl, kl, ll,

        r0l,  r1l,  r2l,  r3l,
        r4l,  r5l,  r6l,  r7l,
        r8l,  r9l,  r10l, r11l,
        r12l, r13l, r14l, r15l,

        reg8l_first = spl,
        reg8l_last = r15l,

        // high 8 bit register
        sph, bph, sih, dih,

        ah, bh, ch, dh,
        eh, fh, gh, hh,
        ih, jh, kh, lh,

        r0h,  r1h,  r2h,  r3h,
        r4h,  r5h,  r6h,  r7h,
        r8h,  r9h,  r10h, r11h,
        r12h, r13h, r14h, r15h,

        reg8h_first = sph,
        reg8h_last = r15h,

        // 16 bit register
        sp, bp, si, di,

        ax, bx, cx, dx,
        ex, fx, gx, hx,
        ix, jx, kx, lx,

        r0x,  r1x,  r2x,  r3x,
        r4x,  r5x,  r6x,  r7x,
        r8x,  r9x,  r10x, r11x,
        r12x, r13x, r14x, r15x,

        reg16_first = sp,
        reg16_last = r15x,

        // 32 bit register
        esp, ebp, esi, edi,

        eax, ebx, ecx, edx,
        eex, efx, egx, ehx,
        eix, ejx, ekx, elx,

        r0d,  r1d,  r2d,  r3d,
        r4d,  r5d,  r6d,  r7d,
        r8d,  r9d,  r10d, r11d,
        r12d, r13d, r14d, r15d,

        reg32_first = esp,
        reg32_last = r15d,

        // 64 bit register
        rsp, rbp, rsi, rdi,

        rax, rbx, rcx, rdx,
        rex, rfx, rgx, rhx,
        rix, rjx, rkx, rlx,

        r0,  r1,  r2,  r3,
        r4,  r5,  r6,  r7,
        r8,  r9,  r10, r11,
        r12, r13, r14, r15,

        reg64_first = rsp,
        reg64_last = r15,

        kMaxRegs = 32,

        last = reg64_last
    };

    static bool isValidRegister(reg_t reg) {
#if 1
        return (reg <= vmReg::last);
#else
        return (reg >= vmReg::first && reg <= vmReg::last);
#endif
    }

    static uint32_t getType(reg_t reg) {
        return ((uint32_t)reg / kMaxRegs);
    }

    static uint32_t getIndex(reg_t reg) {
        return ((uint32_t)reg % kMaxRegs);
    }

    static const char * getTypeStr(reg_t reg) {
        uint32_t regType = vmReg::getType(reg);
        switch (regType) {
        case vmRegType::r8:
            return "8bit";

        case vmRegType::r8_high:
            return "8bit high";

        case vmRegType::r16:
            return "16bit";

        case vmRegType::r32:
            return "32bit";

        case vmRegType::r64:
            return "64bit";

        case vmRegType::r128:
            return "128bit";

        case vmRegType::r256:
            return "256bit";

        case vmRegType::r512:
            return "512bit";

        default:
            return "Unknown Type";
        }
    }
};

struct vmRegId {
    enum Type {
        // 64 bit register
        rsp, rbp, rsi, rdi,

        rax, rbx, rcx, rdx,
        rex, rfx, rgx, rhx,
        rix, rjx, rkx, rlx,

        r0,  r1,  r2,  r3,
        r4,  r5,  r6,  r7,
        r8,  r9,  r10, r11,
        r12, r13, r14, r15,

        // 32 bit register
        esp = rsp, ebp = rbp, esi = rsi, edi = rdi,

        eax = rax, ebx = rbx, ecx = rcx, edx = rdx,
        eex = rex, efx = rfx, egx = rgx, ehx = rhx,
        eix = rix, ejx = rjx, ekx = rkx, elx = rlx,

        r0d  = r0,  r1d  = r1,  r2d  = r2,  r3d  = r3,
        r4d  = r4,  r5d  = r5,  r6d  = r6,  r7d  = r7,
        r8d  = r8,  r9d  = r9,  r10d = r10, r11d = r11,
        r12d = r12, r13d = r13, r14d = r14, r15d = r15,
    };
};

struct uRegAX {
    uint8_t low;
    uint8_t high;
};

struct iRegAX {
    int8_t low;
    int8_t high;
};

struct RegAX32 {
    union {
       iRegAX i8;
       uRegAX u8;
       int16_t i16;
       uint16_t u16;
    };
    uint16_t u16_high;  // reserve
};

struct RegEAX32 {
    union {
        int32_t i32;
        uint32_t u32;
    };
};

struct RegAX64 {
    union {
       iRegAX i8;
       uRegAX u8;
       int16_t i16;
       uint16_t u16;
    };
    uint16_t u16_high;  // reserve1
    uint32_t high32;    // reserve2
};

struct RegEAX64 {
    union {
        int32_t i32;
        uint32_t u32;
    };
    uint32_t high32;    // reserve
};

struct RegRAX64 {
    union {
        int64_t i64;
        uint64_t u64;
    };
};

class Register32 {
public:
    union {
        RegAX32   ax;
        RegEAX32  eax;
        int32_t   ival;
        uint32_t  uval;
    };
};

class Register64 {
public:
    union {
        RegAX64   ax;
        RegEAX64  eax;
        RegRAX64  rax;
        int64_t   ival;
        uint64_t  uval;
    };
};

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
typedef Register64  Register;
#else
typedef Register32  Register;
#endif

template <typename BasicType>
class ByteCode {
public:
    typedef BasicType   basic_type;
    typedef size_t      size_type;

public:
    ByteCode() {}
    ~ByteCode() {}
};

class vmBinImage {
private:
    void * data_;
    size_t size_;
    void * entry_;

public:
    vmBinImage() : data_(nullptr), size_(0), entry_(nullptr) {}
    ~vmBinImage() {
        this->deallocate();
    }

    void * data() const { return data_; }
    void * entry() const { return entry_; }

    size_t size() const { return size_; }

    void setEntryOffset(size_t entryOffset) {
        entry_ = (void *)((char *)data_ + entryOffset);
    }

    void allocate(size_t imageSize) {
#if defined(_WIN32)
        if (data_) {
            _aligned_free(data_);
        }
        // Binary image first address must be aligned for 256 bytes.
        data_ = (void *)_aligned_malloc(imageSize, 256);
#else
        if (data_) {
            free(data_);
        }
        // Binary image first address must be aligned for 256 bytes.
        int ret = posix_memalign((void **)&data_, 256, imageSize);
#endif // _WIN32
        size_ = imageSize;
    }

    void deallocate() {
        if (data_) {
#if defined(_WIN32)
            _aligned_free(data_);
#else
            free(data_);
#endif
            data_ = nullptr;
        }
        size_ = 0;
    }
};

class vmAsmFile {
public:
    vmAsmFile() {}
    ~vmAsmFile() {}

    int loadFromFile(const char * filename) {
        return 1;
    }

    int saveToFile(const char * filename) {
        return 1;
    }
};

class vmCallStackDebugInfo {
#ifndef NDEBUG
private:
    char * moduleName;      // Module name, example: Nt.dll
    char * methodName;      // Method name, example: GetPos()
    char * filename;        // Source code filename, example: ErrorCode.h
    uint32_t line;          // Source line no, example: Line 108
#endif

public:
#ifndef NDEBUG
    vmCallStackDebugInfo()
        : moduleName(nullptr), methodName(nullptr),
          filename(nullptr), line(0) {}
#else
    vmCallStackDebugInfo() {}
#endif

    ~vmCallStackDebugInfo() {
        destroy();
    }

    void destroy() {
#ifndef NDEBUG
        if (moduleName) {
            delete moduleName;
            moduleName = nullptr;
        }
        if (methodName) {
            delete methodName;
            methodName = nullptr;
        }
        if (filename) {
            delete filename;
            filename = nullptr;
        }
#endif
    }

#ifndef NDEBUG
    bool isDebug() const { return true; }

    const char * getModuleName() const {
        return moduleName;
    }

    const char * getMethodName() const {
        return methodName;
    }

    const char * getFileName() const {
        return filename;
    }

    uint32_t getLine() const {
        return line;
    }
#else
    bool isDebug() const { return false; }

    const char * getModuleName() const {
        return nullptr;
    }

    const char * getMethodName() const {
        return nullptr;
    }

    const char * getFileName() const {
        return nullptr;
    }

    uint32_t getLine() const {
        return 0;
    }
#endif
};

class vmCallStackInfo {
private:
    unsigned char * callFP_;
    unsigned char * returnFP_;

public:
    vmCallStackDebugInfo debugInfo;

public:
    vmCallStackInfo() : callFP_(nullptr), returnFP_(nullptr) {}
    vmCallStackInfo(unsigned char * callFP, unsigned char * returnFP)
        : callFP_(callFP), returnFP_(returnFP) {}
    ~vmCallStackInfo() {}

    unsigned char * getCallFP() const { return callFP_; }
    unsigned char * getReturnFP() const { return returnFP_; }

    void setCallFP(unsigned char * callFP) { callFP_ = callFP; }
    void setReturnFP(unsigned char * returnFP) { returnFP_ = returnFP; }
};

class vmCallStack {
private:
    ::std::list<vmCallStackInfo *> stack_;

public:
    vmCallStack() {}
    ~vmCallStack() {}

    void destroy() {
        ::std::list<vmCallStackInfo *>::iterator iter;
        for (iter = stack_.begin(); iter != stack_.end(); ++iter) {
            vmCallStackInfo * info = *iter;
            if (info) {
                delete info;
                *iter = nullptr;
            }
        }
        stack_.clear();
    }

    int push(unsigned char * callFP, unsigned char * returnFP) {
        vmCallStackInfo * info = new vmCallStackInfo(callFP, returnFP);
        assert(info != nullptr);
        stack_.push_back(info);
        return 1;
    }

    unsigned char * pop() {
        unsigned char * returnFP = nullptr;
        if (!stack_.empty()) {
            vmCallStackInfo * info = stack_.back();
            if (info) {
                returnFP = info->getReturnFP();
                assert(returnFP != nullptr);
                delete info;
            }
            stack_.pop_back();
        }
        return returnFP;
    }
};

typedef unsigned int vmThreadId;
typedef unsigned int vmProcessId;

template <typename BasicType, bool IsBackwardPtr>
class vmStack;

template <typename BasicType>
class vmFrame {
public:
    typedef BasicType                   basic_type;
    typedef vmStack<basic_type, false>  stack_type;

    static const size_t kMaxRegs = vmReg::kMaxRegs;

private:
    unsigned char * fp_;
    stack_type *    stack_;
    unsigned char * fp_limit_;
    unsigned char * fp_start_;

    Register regs_[kMaxRegs];

#if USE_VMSTACK_CALLSTACK
    // Needn't declare vmCallStack
#else
    vmCallStack callStack_;
#endif

public:
    vmFrame(stack_type * stack = nullptr)
      : fp_(nullptr), stack_(stack),
        fp_limit_(nullptr), fp_start_(nullptr) {
        initRegs();
    }
    ~vmFrame() {
        destroy();
    }

    bool isInited() const { return (fp_start_ != nullptr); }

    bool isEof() const { return (fp_ == fp_limit_); }
    bool isOverflow() const { return (fp_ >= fp_limit_); }
    bool isUnderflow() const { return (fp_ < fp_start_); }

    unsigned char * getFP() const { return fp_; }
    void setFP(void * fp) {
        this->fp_ = (unsigned char *)fp;
    }

    uint32_t getFPOffset() const { return (uint32_t)(fp_ - fp_start_); }
    ptrdiff_t getFPOffset64() const { return (ptrdiff_t)(fp_ - fp_start_); }

    unsigned char * getFPStart() const { return fp_start_; }
    unsigned char * getFPLimit() const { return fp_limit_; }

    stack_type * getStack() const { return stack_; }
    void setStack(stack_type * stack) {
        this->stack_ = stack;
    }

    size_t getMaxRegs() const { return kMaxRegs; }

    void initRegs() {
        memset((void *)&regs_[0], 0, sizeof(regs_));
#if defined(USE_REGS_TEST)
#if defined(_WIN64)
        regs_[vmRegId::rsp].rax.u64 = ' rsp';
        regs_[vmRegId::rbp].rax.u64 = ' rbp';
        regs_[vmRegId::rsi].rax.u64 = ' rsi';
        regs_[vmRegId::rdi].rax.u64 = ' rdi';

        regs_[vmRegId::rax].rax.u64 = ' rax';
        regs_[vmRegId::rbx].rax.u64 = ' rbx';
        regs_[vmRegId::rcx].rax.u64 = ' rcx';
        regs_[vmRegId::rdx].rax.u64 = ' rdx';
        regs_[vmRegId::rex].rax.u64 = ' rex';
        regs_[vmRegId::rfx].rax.u64 = ' rfx';
        regs_[vmRegId::rgx].rax.u64 = ' rgx';
        regs_[vmRegId::rhx].rax.u64 = ' rhx';
#elif defined(_WIN32)
        regs_[vmRegId::esp].eax.u32 = ' rsp';
        regs_[vmRegId::ebp].eax.u32 = ' rbp';
        regs_[vmRegId::esi].eax.u32 = ' rsi';
        regs_[vmRegId::edi].eax.u32 = ' rdi';

        regs_[vmRegId::eax].eax.u32 = ' rax';
        regs_[vmRegId::ebx].eax.u32 = ' rbx';
        regs_[vmRegId::ecx].eax.u32 = ' rcx';
        regs_[vmRegId::edx].eax.u32 = ' rdx';
        regs_[vmRegId::eex].eax.u32 = ' rex';
        regs_[vmRegId::efx].eax.u32 = ' rfx';
        regs_[vmRegId::egx].eax.u32 = ' rgx';
        regs_[vmRegId::ehx].eax.u32 = ' rhx';
#endif // _WIN64
#endif // USE_REGS_TEST
    }

    void destroy() {
        clear();
        stack_ = nullptr;
    }

    void clear() {
        this->fp_ = nullptr;
        this->fp_limit_ = nullptr;
        this->fp_start_ = nullptr;
    }

    void reset() {
        this->fp_ = this->fp_start_;
    }

    void setting(void * imageStart, size_t imageSize, void * imageEntry) {
        this->fp_ = (unsigned char *)imageEntry;
        this->fp_limit_ = (unsigned char *)imageStart + imageSize;
        this->fp_start_ = (unsigned char *)imageStart;
    }

    unsigned char get() const {
        return *fp_;
    }

    int8_t getInt8() const {
        return *(int8_t *)fp_;
    }

    uint8_t getUInt8() const {
        return *(uint8_t *)fp_;
    }

    int16_t getInt16() const {
        return *(int16_t *)fp_;
    }

    uint16_t getUInt16() const {
        return *(uint16_t *)fp_;
    }

    int32_t getInt32() const {
        return *(int32_t *)fp_;
    }

    uint32_t getUInt32() const {
        return *(uint32_t *)fp_;
    }

    int64_t getInt64() const {
        return *(int64_t *)fp_;
    }

    uint64_t getUInt64() const {
        return *(uint64_t *)fp_;
    }

    void * getPointer() const {
        return (void *)(*(uintptr_t *)fp_);
    }

    basic_type getValue(uint32_t dataType) const {
        switch (dataType) {
        case vmDataType::Int8:
            return (basic_type)getInt8();

        case vmDataType::UInt8:
            return (basic_type)getUInt8();

        case vmDataType::Int16:
            return (basic_type)getInt16();

        case vmDataType::UInt16:
            return (basic_type)getUInt16();

        case vmDataType::Int32:
            return (basic_type)getInt32();

        case vmDataType::UInt32:
            return (basic_type)getUInt32();

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmDataType::Int64:
            return (basic_type)getInt64();

        case vmDataType::UInt64:
            return (basic_type)getUInt64();
#endif
        case vmDataType::Pointer:
            return (basic_type)(uintptr_t)getPointer();

        default:
            assert(false);
            return (basic_type)(uintptr_t)getPointer();
        }
    }

    uint32_t getValue32(uint32_t dataType) const {
        switch (dataType) {
        case vmDataType::Int8:
            return (uint32_t)getInt8();

        case vmDataType::UInt8:
            return (uint32_t)getUInt8();

        case vmDataType::Int16:
            return (uint32_t)getInt16();

        case vmDataType::UInt16:
            return (uint32_t)getUInt16();

        case vmDataType::Int32:
            return (uint32_t)getInt32();

        case vmDataType::UInt32:
            return (uint32_t)getUInt32();

        case vmDataType::Pointer:
            return (basic_type)(uintptr_t)getPointer();

        default:
            assert(false);
            return (basic_type)(uintptr_t)getPointer();
        }
    }

    uint64_t getValue64(uint32_t dataType) const {
        switch (dataType) {
        case vmDataType::Int64:
            return (uint64_t)getInt64();

        case vmDataType::UInt64:
            return (uint64_t)getUInt64();

        default:
            assert(false);
            return (uint64_t)getUInt64();
        }
    }

    basic_type getValueByReg(uint32_t regType) const {
        switch (regType) {
        case vmRegType::r8:
        case vmRegType::r8_high:
            return (basic_type)getUInt8();

        case vmRegType::r16:
            return (basic_type)getUInt16();

        case vmRegType::r32:
            return (basic_type)getUInt32();

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)getUInt64();
#endif
        default:
            assert(false);
            return (basic_type)getUInt64();
        }
    }

    void setInt8(int8_t value) {
        *(int8_t *)fp_ = value;
    }

    void setUInt8(uint8_t value) {
        *(uint8_t *)fp_ = value;
    }

    void setInt16(int16_t value) {
        *(int16_t *)fp_ = value;
    }

    void setUInt16(uint16_t value) {
        *(uint16_t *)fp_ = value;
    }

    void setInt32(int32_t value) {
        *(int32_t *)fp_ = value;
    }

    void setUInt32(uint32_t value) {
        *(uint32_t *)fp_ = value;
    }

    void setInt64(int64_t value) {
        *(int64_t *)fp_ = value;
    }

    void setUInt64(uint64_t value) {
        *(uint64_t *)fp_ = value;
    }

    void setPointer(void * ptr) {
        *(uintptr_t *)fp_ = (uintptr_t)ptr;
    }

    void setValue(uint32_t dataType, basic_type value) {
        switch (dataType) {
        case vmDataType::Int8:
            setInt8((int8_t)value);
            break;

        case vmDataType::UInt8:
            setUInt8((uint8_t)value);
            break;

        case vmDataType::Int16:
            setInt16((int16_t)value);
            break;

        case vmDataType::UInt16:
            setUInt16((uint16_t)value);
            break;

        case vmDataType::Int32:
            setInt32((int32_t)value);
            break;

        case vmDataType::UInt32:
            setUInt32((uint32_t)value);
            break;

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmDataType::Int64:
            setInt64((int64_t)value);
            break;

        case vmDataType::UInt64:
            setUInt64((uint64_t)value);
            break;
#endif
        case vmDataType::Pointer:
            setPointer((void *)(uintptr_t)value);
            break;

        default:
            assert(false);
            setPointer((void *)(uintptr_t)value);
            break;
        }
    }

    void setValue32(uint32_t dataType, uint32_t value) {
        switch (dataType) {
        case vmDataType::Int8:
            setInt8((int8_t)value);
            break;

        case vmDataType::UInt8:
            setUInt8((uint8_t)value);
            break;

        case vmDataType::Int16:
            setInt16((int16_t)value);
            break;

        case vmDataType::UInt16:
            setUInt16((uint16_t)value);
            break;

        case vmDataType::Int32:
            setInt32((int32_t)value);
            break;

        case vmDataType::UInt32:
            setUInt32((uint32_t)value);
            break;

        case vmDataType::Pointer:
            setPointer((void *)(uintptr_t)value);
            break;

        default:
            assert(false);
            setPointer((void *)(uintptr_t)value);
            break;
        }
    }

    void setValue64(uint32_t dataType, uint64_t value) {
        switch (dataType) {
        case vmDataType::Int64:
            setInt64((int64_t)value);
            break;

        case vmDataType::UInt64:
            setUInt64((uint64_t)value);
            break;

        default:
            assert(false);
            setUInt64((uint64_t)value);
            break;
        }
    }

    void back() {
        this->fp_--;
    }

    void back(int offset) {
        this->fp_ -= offset;
    }

    void backInt8() {
        this->fp_ -= sizeof(int8_t);
    }

    void backUInt8() {
        this->fp_ -= sizeof(uint8_t);
    }

    void backInt16() {
        this->fp_ -= sizeof(int16_t);
    }

    void backUInt16() {
        this->fp_ += sizeof(uint16_t);
    }

    void backInt32() {
        this->fp_ -= sizeof(int32_t);
    }

    void backUInt32() {
        this->fp_ -= sizeof(uint32_t);
    }

    void backInt64() {
        this->fp_ -= sizeof(int64_t);
    }

    void backUInt64() {
        this->fp_ -= sizeof(uint64_t);
    }

    void backPointer() {
        this->fp_ -= sizeof(void *);
    }

    void next() {
        this->fp_++;
    }

    void next(int offset) {
        this->fp_ += offset;
    }

    void nextInt8() {
        this->fp_ += sizeof(int8_t);
    }

    void nextUInt8() {
        this->fp_ += sizeof(uint8_t);
    }

    void nextInt16() {
        this->fp_ += sizeof(int16_t);
    }

    void nextUInt16() {
        this->fp_ += sizeof(uint16_t);
    }

    void nextInt32() {
        this->fp_ += sizeof(int32_t);
    }

    void nextUInt32() {
        this->fp_ += sizeof(uint32_t);
    }

    void nextInt64() {
        this->fp_ += sizeof(int64_t);
    }

    void nextUInt64() {
        this->fp_ += sizeof(uint64_t);
    }

    void nextPointer() {
        this->fp_ += sizeof(void *);
    }

    void nextValue(uint32_t dataType) {
        switch (dataType) {
        case vmDataType::Int8:
            nextInt8();
            break;

        case vmDataType::UInt8:
            nextUInt8();
            break;

        case vmDataType::Int16:
            nextInt16();
            break;

        case vmDataType::UInt16:
            nextUInt16();
            break;

        case vmDataType::Int32:
            nextInt32();
            break;

        case vmDataType::UInt32:
            nextUInt32();
            break;

        case vmDataType::Int64:
            nextInt64();
            break;

        case vmDataType::UInt64:
            nextUInt64();
            break;

        case vmDataType::Pointer:
            nextPointer();
            break;

        default:
            assert(false);
            nextPointer();
            break;
        }
    }

    void nextValueByReg(uint32_t regType) {
        switch (regType) {
        case vmRegType::r8:
        case vmRegType::r8_high:
            nextUInt8();
            break;

        case vmRegType::r16:
            nextUInt16();
            break;

        case vmRegType::r32:
            nextUInt32();
            break;

        case vmRegType::r64:
            nextUInt64();
            break;

        default:
            assert(false);
            nextUInt64();
            break;
        }
    }

    uint8_t getRegValue8L(uint8_t regIndex) {
        assert(regIndex >= 0 && regIndex < kMaxRegs);
        return regs_[regIndex].ax.u8.low;
    }

    uint8_t getRegValue8H(uint8_t regIndex) {
        assert(regIndex >= 0 && regIndex < kMaxRegs);
        return regs_[regIndex].ax.u8.high;
    }

    uint16_t getRegValue16(uint16_t regIndex) {
        assert(regIndex >= 0 && regIndex < kMaxRegs);
        return regs_[regIndex].ax.u16;
    }

    uint32_t getRegValue32(uint32_t regIndex) {
        assert(regIndex >= 0 && regIndex < kMaxRegs);
        return regs_[regIndex].eax.u32;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t getRegValue64(uint64_t regIndex) {
        assert(regIndex >= 0 && regIndex < kMaxRegs);
        return regs_[regIndex].rax.u64;
    }
#endif

    basic_type getRegValue(uint32_t regType, uint32_t regIndex) {
        switch (regType) {
        case vmRegType::r8:
            return getRegValue8L(regIndex);

        case vmRegType::r8_high:
            return getRegValue8H(regIndex);

        case vmRegType::r16:
            return getRegValue16(regIndex);

        case vmRegType::r32:
            return getRegValue32(regIndex);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return getRegValue64(regIndex);
#endif
        default:
            assert(false);
            return (basic_type)0xCCCCCCCCCCCCCCCCULL;
        }
    }

    basic_type getRegValue(reg_t reg) {
        uint32_t regType = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        return getRegValue(regType, regIndex);
    }

    void setRegValue8L(uint32_t regIndex, uint8_t value) {
        regs_[regIndex].ax.u8.low = value;
    }

    void setRegValue8H(uint32_t regIndex, uint8_t value) {
        regs_[regIndex].ax.u8.high = value;
    }

    void setRegValue16(uint32_t regIndex, uint16_t value) {
        regs_[regIndex].ax.u16 = value;
    }

    void setRegValue32(uint32_t regIndex, uint32_t value) {
        regs_[regIndex].eax.u32 = value;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    void setRegValue64(uint32_t regIndex, uint64_t value) {
        regs_[regIndex].rax.u64 = value;
    }
#endif

    void setRegValue(uint32_t regType, uint32_t regIndex, basic_type value) {
        switch (regType) {
        case vmRegType::r8:
            setRegValue8L((uint8_t)value);
            break;
        case vmRegType::r8_high:
            setRegValue8H((uint8_t)value);
            break;
        case vmRegType::r16:
            setRegValue16((uint16_t)value);
            break;
        case vmRegType::r32:
            setRegValue32((uint32_t)value);
            break;
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            setRegValue64((uint64_t)value);
            break;
#endif
        default:
            // Do nothing !!
            assert(false);
            break;
        }
    }

    void setRegValue(reg_t reg, basic_type value) {
        uint32_t regType = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        setRegValue(regType, regIndex, value);
    }

    int8_t loadRegValue8L(uint32_t regIndex) {
        int8_t value = getInt8();
        regs_[regIndex].ax.i8.low = value;
        nextInt8();
        return value;
    }

    int8_t loadRegValue8H(uint32_t regIndex) {
        int8_t value = getInt8();
        regs_[regIndex].ax.i8.high = value;
        nextInt8();
        return value;
    }

    int16_t loadRegValue16(uint32_t regIndex) {
        int16_t value = getInt16();
        regs_[regIndex].ax.i16 = value;
        nextInt16();
        return value;
    }

    int32_t loadRegValue32(uint32_t regIndex) {
        int32_t value = getInt32();
        regs_[regIndex].eax.i32 = value;
        nextInt32();
        return value;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    int64_t loadRegValue64(uint32_t regIndex) {
        int64_t value = getInt64();
        regs_[regIndex].rax.i64 = value;
        nextInt64();
        return value;
    }
#endif

    basic_type loadRegValue(uint32_t regType, uint32_t regIndex) {
        switch (regType) {
        case vmRegType::r8:
            return (basic_type)loadRegValue8L(regIndex);

        case vmRegType::r8_high:
            return (basic_type)loadRegValue8H(regIndex);

        case vmRegType::r16:
            return (basic_type)loadRegValue16(regIndex);

        case vmRegType::r32:
            return (basic_type)loadRegValue32(regIndex);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)loadRegValue64(regIndex);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)0xCCCCCCCCCCCCCCCCULL;
        }
    }

    basic_type loadRegValue(reg_t reg) {
        uint32_t regType = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        return loadRegValue(regType, regIndex);
    }

    uint8_t moveRegValue8L(uint32_t regIndex1, uint32_t regIndex2) {
        uint8_t value = getRegValue8L(regIndex2);
        setRegValue8L(regIndex1, value);
        return value;
    }

    uint8_t moveRegValue8H(uint32_t regIndex1, uint32_t regIndex2) {
        uint8_t value = getRegValue8H(regIndex2);
        setRegValue8H(regIndex1, value);
        return value;
    }

    uint16_t moveRegValue16(uint32_t regIndex1, uint32_t regIndex2) {
        uint16_t value = getRegValue16(regIndex2);
        setRegValue16(regIndex1, value);
        return value;
    }

    uint32_t moveRegValue32(uint32_t regIndex1, uint32_t regIndex2) {
        uint32_t value = getRegValue32(regIndex2);
        setRegValue32(regIndex1, value);
        return value;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t moveRegValue64(uint32_t regIndex1, uint32_t regIndex2) {
        uint64_t value = getRegValue64(regIndex2);
        setRegValue64(regIndex1, value);
        return value;
    }
#endif

    basic_type moveRegValue(uint32_t regType1, uint32_t regIndex1,
                            uint32_t regType2, uint32_t regIndex2) {
        switch (regType1) {
        case vmRegType::r8:
            return (basic_type)moveRegValue8L(regIndex1, regIndex2);

        case vmRegType::r8_high:
            return (basic_type)moveRegValue8H(regIndex1, regIndex2);

        case vmRegType::r16:
            return (basic_type)moveRegValue16(regIndex1, regIndex2);

        case vmRegType::r32:
            return (basic_type)moveRegValue32(regIndex1, regIndex2);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)moveRegValue64(regIndex1, regIndex2);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)0xCCCCCCCCCCCCCCCCULL;
        }
    }

    basic_type moveRegValue(reg_t reg1, reg_t reg2) {
        uint32_t regType1  = vmReg::getType(reg1);
        uint32_t regIndex1 = vmReg::getIndex(reg1);
        uint32_t regType2  = vmReg::getType(reg2);
        uint32_t regIndex2 = vmReg::getIndex(reg2);
        return moveRegValue(regType1, regIndex1, regType2, regIndex2);
    }

    uint8_t incRegValue8L(uint32_t regIndex) {
        uint8_t value = getRegValue8L(regIndex);
        value++;
        setRegValue8L(regIndex, value);
        return value;
    }

    uint8_t incRegValue8H(uint32_t regIndex) {
        uint8_t value = getRegValue8H(regIndex);
        value++;
        setRegValue8H(regIndex, value);
        return value;
    }

    uint16_t incRegValue16(uint32_t regIndex) {
        uint16_t value = getRegValue16(regIndex);
        value++;
        setRegValue16(regIndex, value);
        return value;
    }

    uint32_t incRegValue32(uint32_t regIndex) {
        uint32_t value = getRegValue32(regIndex);
        value++;
        setRegValue32(regIndex, value);
        return value;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t incRegValue64(uint32_t regIndex) {
        uint64_t value = getRegValue64(regIndex);
        value++;
        setRegValue64(regIndex, value);
        return value;
    }
#endif

    basic_type incRegValue(uint32_t regType, uint32_t regIndex) {
        switch (regType) {
        case vmRegType::r8:
            return (basic_type)incRegValue8L(regIndex);

        case vmRegType::r8_high:
            return (basic_type)incRegValue8H(regIndex);

        case vmRegType::r16:
            return (basic_type)incRegValue16(regIndex);

        case vmRegType::r32:
            return (basic_type)incRegValue32(regIndex);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)incRegValue64(regIndex);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)-1;
        }
    }

    basic_type incRegValue(reg_t reg) {
        uint32_t regType  = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        return incRegValue(regType, regIndex);
    }

    uint8_t decRegValue8L(uint32_t regIndex) {
        uint8_t value = getRegValue8L(regIndex);
        value--;
        setRegValue8L(regIndex, value);
        return value;
    }

    uint8_t decRegValue8H(uint32_t regIndex) {
        uint8_t value = getRegValue8H(regIndex);
        value--;
        setRegValue8H(regIndex, value);
        return value;
    }

    uint16_t decRegValue16(uint32_t regIndex) {
        uint16_t value = getRegValue16(regIndex);
        value--;
        setRegValue16(regIndex, value);
        return value;
    }

    uint32_t decRegValue32(uint32_t regIndex) {
        uint32_t value = getRegValue32(regIndex);
        value--;
        setRegValue32(regIndex, value);
        return value;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t decRegValue64(uint32_t regIndex) {
        uint64_t value = getRegValue64(regIndex);
        value--;
        setRegValue64(regIndex, value);
        return value;
    }
#endif

    basic_type decRegValue(uint32_t regType, uint32_t regIndex) {
        switch (regType) {
        case vmRegType::r8:
            return (basic_type)decRegValue8L(regIndex);

        case vmRegType::r8_high:
            return (basic_type)decRegValue8H(regIndex);

        case vmRegType::r16:
            return (basic_type)decRegValue16(regIndex);

        case vmRegType::r32:
            return (basic_type)decRegValue32(regIndex);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)decRegValue64(regIndex);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)-1;
        }
    }

    basic_type decRegValue(reg_t reg) {
        uint32_t regType  = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        return decRegValue(regType, regIndex);
    }

    uint8_t addRegValue8L(uint32_t regIndex1, uint32_t regIndex2) {
        uint8_t value2 = getRegValue8L(regIndex2);
        uint8_t value1 = getRegValue8L(regIndex1);
        uint8_t value = value1 + value2;
        setRegValue8L(regIndex1, value);
        return value;
    }

    uint8_t addRegValue8H(uint32_t regIndex1, uint32_t regIndex2) {
        uint8_t value2 = getRegValue8H(regIndex2);
        uint8_t value1 = getRegValue8H(regIndex1);
        uint8_t value = value1 + value2;
        setRegValue8H(regIndex1, value);
        return value;
    }

    uint16_t addRegValue16(uint32_t regIndex1, uint32_t regIndex2) {
        uint16_t value2 = getRegValue16(regIndex2);
        uint16_t value1 = getRegValue16(regIndex1);
        uint16_t value = value1 + value2;
        setRegValue16(regIndex1, value);
        return value;
    }

    uint32_t addRegValue32(uint32_t regIndex1, uint32_t regIndex2) {
        uint32_t value2 = getRegValue32(regIndex2);
        uint32_t value1 = getRegValue32(regIndex1);
        uint32_t value = value1 + value2;
        setRegValue32(regIndex1, value);
        return value;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t addRegValue64(uint32_t regIndex1, uint32_t regIndex2) {
        uint64_t value2 = getRegValue64(regIndex2);
        uint64_t value1 = getRegValue64(regIndex1);
        uint64_t value = value1 + value2;
        setRegValue64(regIndex1, value);
        return value;
    }
#endif

    basic_type addRegValue(uint32_t regType1, uint32_t regIndex1,
                           uint32_t regType2, uint32_t regIndex2) {
        switch (regType1) {
        case vmRegType::r8:
            return (basic_type)addRegValue8L(regIndex1, regIndex2);

        case vmRegType::r8_high:
            return (basic_type)addRegValue8H(regIndex1, regIndex2);

        case vmRegType::r16:
            return (basic_type)addRegValue16(regIndex1, regIndex2);

        case vmRegType::r32:
            return (basic_type)addRegValue32(regIndex1, regIndex2);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)addRegValue64(regIndex1, regIndex2);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)0xCCCCCCCCCCCCCCCCULL;
        }
    }

    basic_type addRegValue(reg_t reg1, reg_t reg2) {
        uint32_t regType1  = vmReg::getType(reg1);
        uint32_t regIndex1 = vmReg::getIndex(reg1);
        uint32_t regType2  = vmReg::getType(reg2);
        uint32_t regIndex2 = vmReg::getIndex(reg2);
        return addRegValue(regType1, regIndex1, regType2, regIndex2);
    }

    uint8_t addRegValue8L_ri(uint32_t regIndex, uint8_t value) {
        uint8_t oldValue = getRegValue8L(regIndex);
        uint8_t newValue = oldValue + value;
        setRegValue8L(regIndex, newValue);
        return newValue;
    }

    uint8_t addRegValue8H_ri(uint32_t regIndex, uint8_t value) {
        uint8_t oldValue = getRegValue8H(regIndex);
        uint8_t newValue = oldValue + value;
        setRegValue8H(regIndex, newValue);
        return newValue;
    }

    uint16_t addRegValue16_ri(uint32_t regIndex, uint16_t value) {
        uint16_t oldValue = getRegValue16(regIndex);
        uint16_t newValue = oldValue + value;
        setRegValue16(regIndex, newValue);
        return newValue;
    }

    uint32_t addRegValue32_ri(uint32_t regIndex, uint32_t value) {
        uint32_t oldValue = getRegValue32(regIndex);
        uint32_t newValue = oldValue + value;
        setRegValue32(regIndex, newValue);
        return newValue;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t addRegValue64_ri(uint32_t regIndex, uint64_t value) {
        uint64_t oldValue = getRegValue64(regIndex);
        uint64_t newValue = oldValue + value;
        setRegValue64(regIndex, newValue);
        return newValue;
    }
#endif

    basic_type addRegValue_ri(uint32_t regType, uint32_t regIndex,
                              basic_type value) {
        switch (regType) {
        case vmRegType::r8:
            return (basic_type)addRegValue8L_ri(regIndex, (uint8_t)value);

        case vmRegType::r8_high:
            return (basic_type)addRegValue8H_ri(regIndex, (uint8_t)value);

        case vmRegType::r16:
            return (basic_type)addRegValue16_ri(regIndex, (uint16_t)value);

        case vmRegType::r32:
            return (basic_type)addRegValue32_ri(regIndex, (uint32_t)value);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)addRegValue64_ri(regIndex, (uint64_t)value);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)0xCCCCCCCCCCCCCCCCULL;
        }
    }

    basic_type addRegValue_ri(reg_t reg, basic_type value) {
        uint32_t regType  = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        return addRegValue_ri(regType, regIndex, value);
    }

    uint8_t subRegValue8L(uint32_t regIndex1, uint32_t regIndex2) {
        uint8_t value2 = getRegValue8L(regIndex2);
        uint8_t value1 = getRegValue32(regIndex1);
        uint8_t value = value1 - value2;
        setRegValue8L(regIndex1, value);
        return value;
    }

    uint8_t subRegValue8H(uint32_t regIndex1, uint32_t regIndex2) {
        uint8_t value2 = getRegValue8H(regIndex2);
        uint8_t value1 = getRegValue32(regIndex1);
        uint8_t value = value1 - value2;
        setRegValue8H(regIndex1, value);
        return value;
    }

    uint16_t subRegValue16(uint32_t regIndex1, uint32_t regIndex2) {
        uint16_t value2 = getRegValue16(regIndex2);
        uint16_t value1 = getRegValue32(regIndex1);
        uint16_t value = value1 - value2;
        setRegValue16(regIndex1, value);
        return value;
    }

    uint32_t subRegValue32(uint32_t regIndex1, uint32_t regIndex2) {
        uint32_t value2 = getRegValue32(regIndex2);
        uint32_t value1 = getRegValue32(regIndex1);
        uint32_t value = value1 - value2;
        setRegValue32(regIndex1, value);
        return value;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t subRegValue64(uint32_t regIndex1, uint32_t regIndex2) {
        uint64_t value2 = getRegValue64(regIndex2);
        uint64_t value1 = getRegValue64(regIndex1);
        uint64_t value = value1 - value2;
        setRegValue64(regIndex1, value);
        return value;
    }
#endif

    basic_type subRegValue(uint32_t regType1, uint32_t regIndex1,
                           uint32_t regType2, uint32_t regIndex2) {
        switch (regType1) {
        case vmRegType::r8:
            return (basic_type)subRegValue8L(regIndex1, regIndex2);

        case vmRegType::r8_high:
            return (basic_type)subRegValue8H(regIndex1, regIndex2);

        case vmRegType::r16:
            return (basic_type)subRegValue16(regIndex1, regIndex2);

        case vmRegType::r32:
            return (basic_type)subRegValue32(regIndex1, regIndex2);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)subRegValue64(regIndex1, regIndex2);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)0xCCCCCCCCCCCCCCCCULL;
        }
    }

    basic_type subRegValue(reg_t reg1, reg_t reg2) {
        uint32_t regType1  = vmReg::getType(reg1);
        uint32_t regIndex1 = vmReg::getIndex(reg1);
        uint32_t regType2  = vmReg::getType(reg2);
        uint32_t regIndex2 = vmReg::getIndex(reg2);
        return subRegValue(regType1, regIndex1, regType2, regIndex2);
    }

    uint8_t subRegValue8L_ri(uint32_t regIndex, uint8_t value) {
        uint8_t oldValue = getRegValue8L(regIndex);
        uint8_t newValue = oldValue - value;
        setRegValue8L(regIndex, newValue);
        return newValue;
    }

    uint8_t subRegValue8H_ri(uint32_t regIndex, uint8_t value) {
        uint8_t oldValue = getRegValue32(regIndex);
        uint8_t newValue = oldValue - value;
        setRegValue8H(regIndex, newValue);
        return newValue;
    }

    uint16_t subRegValue16_ri(uint32_t regIndex, uint16_t value) {
        uint16_t oldValue = getRegValue32(regIndex);
        uint16_t newValue = oldValue - value;
        setRegValue16(regIndex, newValue);
        return newValue;
    }

    uint32_t subRegValue32_ri(uint32_t regIndex, uint32_t value) {
        uint32_t oldValue = getRegValue32(regIndex);
        uint32_t newValue = oldValue - value;
        setRegValue32(regIndex, newValue);
        return newValue;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t subRegValue64_ri(uint32_t regIndex, uint64_t value) {
        uint64_t oldValue = getRegValue64(regIndex);
        uint64_t newValue = oldValue - value;
        setRegValue64(regIndex, newValue);
        return newValue;
    }
#endif

    basic_type subRegValue_ri(uint32_t regType, uint32_t regIndex,
                              basic_type value) {
        switch (regType) {
        case vmRegType::r8:
            return (basic_type)subRegValue8L_ri(regIndex, (uint8_t)value);

        case vmRegType::r8_high:
            return (basic_type)subRegValue8H_ri(regIndex, (uint8_t)value);

        case vmRegType::r16:
            return (basic_type)subRegValue16_ri(regIndex, (uint16_t)value);

        case vmRegType::r32:
            return (basic_type)subRegValue32_ri(regIndex, (uint32_t)value);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)subRegValue64_ri(regIndex, (uint64_t)value);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)0xCCCCCCCCCCCCCCCCULL;
        }
    }

    basic_type subRegValue_ri(reg_t reg, basic_type value) {
        uint32_t regType  = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        return subRegValue_ri(regType, regIndex, value);
    }

    uint8_t mulRegValue8L(uint32_t regIndex1, uint32_t regIndex2) {
        uint8_t value2 = getRegValue8L(regIndex2);
        uint8_t value1 = getRegValue32(regIndex1);
        uint8_t value = value1 * value2;
        setRegValue8L(regIndex1, value);
        return value;
    }

    uint8_t mulRegValue8H(uint32_t regIndex1, uint32_t regIndex2) {
        uint8_t value2 = getRegValue8H(regIndex2);
        uint8_t value1 = getRegValue32(regIndex1);
        uint8_t value = value1 * value2;
        setRegValue8H(regIndex1, value);
        return value;
    }

    uint16_t mulRegValue16(uint32_t regIndex1, uint32_t regIndex2) {
        uint16_t value2 = getRegValue16(regIndex2);
        uint16_t value1 = getRegValue32(regIndex1);
        uint16_t value = value1 * value2;
        setRegValue16(regIndex1, value);
        return value;
    }

    uint32_t mulRegValue32(uint32_t regIndex1, uint32_t regIndex2) {
        uint32_t value2 = getRegValue32(regIndex2);
        uint32_t value1 = getRegValue32(regIndex1);
        uint32_t value = value1 * value2;
        setRegValue32(regIndex1, value);
        return value;
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    uint64_t mulRegValue64(uint32_t regIndex1, uint32_t regIndex2) {
        uint64_t value2 = getRegValue64(regIndex2);
        uint64_t value1 = getRegValue64(regIndex1);
        uint64_t value = value1 * value2;
        setRegValue64(regIndex1, value);
        return value;
    }
#endif

    basic_type mulRegValue(uint32_t regType1, uint32_t regIndex1,
                           uint32_t regType2, uint32_t regIndex2) {
        switch (regType1) {
        case vmRegType::r8:
            return (basic_type)mulRegValue8L(regIndex1, regIndex2);

        case vmRegType::r8_high:
            return (basic_type)mulRegValue8H(regIndex1, regIndex2);

        case vmRegType::r16:
            return (basic_type)mulRegValue16(regIndex1, regIndex2);

        case vmRegType::r32:
            return (basic_type)mulRegValue32(regIndex1, regIndex2);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            return (basic_type)mulRegValue64(regIndex1, regIndex2);
#endif
        default:
            // Do nothing !!
            assert(false);
            return (basic_type)0xCCCCCCCCCCCCCCCCULL;
        }
    }

    basic_type mulRegValue(reg_t reg1, reg_t reg2) {
        uint32_t regType1  = vmReg::getType(reg1);
        uint32_t regIndex1 = vmReg::getIndex(reg1);
        uint32_t regType2  = vmReg::getType(reg2);
        uint32_t regIndex2 = vmReg::getIndex(reg2);
        return mulRegValue(regType1, regIndex1, regType2, regIndex2);
    }

    void pushCallStack(unsigned char * returnFP) {
#if USE_VMSTACK_CALLSTACK
        assert(stack_ != nullptr);
        stack_->push_callstack(returnFP);
#else
        callStack_.push(nullptr, returnFP);
#endif
    }

    unsigned char * popCallStack() {
#if USE_VMSTACK_CALLSTACK
        assert(stack_ != nullptr);
        return stack_->pop_callstack();
#else
        return callStack_.pop();
#endif
    }

    void callShort(unsigned char * callFP, size_t offset,
                   int16_t callOffset) {
        unsigned char * returnFP = callFP + offset;
        pushCallStack(returnFP);

        unsigned char * newFP = callFP + callOffset;
        // Call entry address must be align for 16 bytes.
        assert(CHECK_ADDR_ALIGNMENT(newFP));
        setFP(newFP);
    }

    void callLong(unsigned char * callFP, size_t offset,
                  int32_t callOffset) {
        unsigned char * returnFP = callFP + offset;
        pushCallStack(returnFP);

        unsigned char * newFP = callFP + callOffset;
        // Call entry address must be align for 16 bytes.
        assert(CHECK_ADDR_ALIGNMENT(newFP));
        setFP(newFP);
    }

    void callPtr32(unsigned char * callFP, size_t offset,
                   uint32_t callEntry) {
        unsigned char * returnFP = callFP + offset;
        pushCallStack(returnFP);

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        void * newFP = (void *)(uint64_t)callEntry;
#else
        void * newFP = (void *)callEntry;
#endif
        // Call entry address must be align for 16 bytes.
        assert(CHECK_ADDR_ALIGNMENT(newFP));
        setFP(newFP);
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    void callPtr64(unsigned char * callFP, size_t offset,
                   uint64_t callEntry) {
        unsigned char * returnFP = callFP + offset;
        pushCallStack(returnFP);

        void * newFP = (void *)callEntry;
        // Call entry address must be align for 16 bytes.
        assert(CHECK_ADDR_ALIGNMENT(newFP));
        setFP(newFP);
    }
#endif

    unsigned char * callReturn() {
        unsigned char * returnFP = popCallStack();
        setFP(returnFP);
        return returnFP;
    }

    void jumpNear(unsigned char * baseFP, int8_t offset) {
        unsigned char * jumpFP = baseFP + offset;
        setFP(jumpFP);
    }

    void jumpShort(unsigned char * baseFP, int16_t offset) {
        unsigned char * jumpFP = baseFP + offset;
        setFP(jumpFP);
    }

    void jumpLong(unsigned char * baseFP, int32_t offset) {
        unsigned char * jumpFP = baseFP + offset;
        setFP(jumpFP);
    }

    void jumpPtr32(unsigned char * baseFP, uint32_t address) {
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        void * jumpFP = (void *)(uint64_t)address;
#else
        void * jumpFP = (void *)address;
#endif
        setFP(jumpFP);
    }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    void jumpPtr64(unsigned char * baseFP, uint64_t address) {
        void * jumpFP = (void *)address;
        setFP(jumpFP);
    }
#endif

    template <typename DataType>
    static bool getCondition(DataType v1, DataType v2, uint8_t condType) {
        switch (condType) {
        case OpCode::jz:
            return (v1 == 0 && v2 == 0);

        case OpCode::jnz:
            return (v1 != 0 && v2 != 0);

        case OpCode::je:
            return (v1 == v2);

        case OpCode::jne:
            return (v1 != v2);

        case OpCode::jl:
            return (v1 < v2);

        case OpCode::jle:
            return (v1 <= v2);

        case OpCode::jg:
            return (v1 > v2);

        case OpCode::jge:
            return (v1 >= v2);

        default:
            Debug.print("Error: Unknown condition jump code. condType = %u\n",
                        (uint32_t)condType);
            return false;
        }
    }

    bool condCmp_rr(reg_t reg1, reg_t reg2,
                    uint32_t dataType, uint8_t condType) {
        uint32_t regIndex1 = vmReg::getIndex(reg1);
        uint32_t regIndex2 = vmReg::getIndex(reg2);
        switch (dataType) {
        case vmDataType::Int8:
            {
                int8_t v1 = regs_[regIndex1].ax.i8.low;
                int8_t v2 = regs_[regIndex2].ax.i8.low;
                return vmFrame::getCondition<int8_t>(v1, v2, condType);
            }

        case vmDataType::UInt8:
            {
                uint8_t v1 = regs_[regIndex1].ax.u8.low;
                uint8_t v2 = regs_[regIndex2].ax.u8.low;
                return vmFrame::getCondition<uint8_t>(v1, v2, condType);
            }

        case vmDataType::Int16:
            {
                int16_t v1 = regs_[regIndex1].ax.i16;
                int16_t v2 = regs_[regIndex2].ax.i16;
                return vmFrame::getCondition<int16_t>(v1, v2, condType);
            }

        case vmDataType::UInt16:
            {
                uint16_t v1 = regs_[regIndex1].ax.u16;
                uint16_t v2 = regs_[regIndex2].ax.u16;
                return vmFrame::getCondition<uint16_t>(v1, v2, condType);
            }

        case vmDataType::Int32:
            {
                int32_t v1 = regs_[regIndex1].eax.i32;
                int32_t v2 = regs_[regIndex2].eax.i32;
                return vmFrame::getCondition<int32_t>(v1, v2, condType);
            }

        case vmDataType::UInt32:
            {
                uint32_t v1 = regs_[regIndex1].eax.u32;
                uint32_t v2 = regs_[regIndex2].eax.u32;
                return vmFrame::getCondition<uint32_t>(v1, v2, condType);
            }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmDataType::Int64:
            {
                int64_t v1 = regs_[regIndex1].rax.i64;
                int64_t v2 = regs_[regIndex2].rax.i64;
                return vmFrame::getCondition<int64_t>(v1, v2, condType);
            }

        case vmDataType::UInt64:
            {
                uint64_t v1 = regs_[regIndex1].rax.u64;
                uint64_t v2 = regs_[regIndex2].rax.u64;
                return vmFrame::getCondition<uint64_t>(v1, v2, condType);
            }
#endif

        case vmDataType::Pointer:
            {
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
                void * v1 = (void *)regs_[regIndex1].rax.u64;
                void * v2 = (void *)regs_[regIndex2].rax.u64;
                return vmFrame::getCondition<void *>(v1, v2, condType);
#else
                void * v1 = (void *)regs_[regIndex1].eax.u32;
                void * v2 = (void *)regs_[regIndex2].eax.u32;
                return vmFrame::getCondition<void *>(v1, v2, condType);
#endif
            }

        default:
            assert(false);
            return false;
        }
    }

    bool condCmp_ri(reg_t reg, basic_type immValue,
                    uint32_t dataType, uint8_t condType) {
        uint32_t regIndex = vmReg::getIndex(reg);
        switch (dataType) {
        case vmDataType::Int8:
            {
                int8_t v1 = regs_[regIndex].ax.i8.low;
                int8_t v2 = (int8_t)immValue;
                return vmFrame::getCondition<int8_t>(v1, v2, condType);
            }

        case vmDataType::UInt8:
            {
                uint8_t v1 = regs_[regIndex].ax.u8.low;
                uint8_t v2 = (uint8_t)immValue;
                return vmFrame::getCondition<uint8_t>(v1, v2, condType);
            }

        case vmDataType::Int16:
            {
                int16_t v1 = regs_[regIndex].ax.i16;
                int16_t v2 = (int16_t)immValue;
                return vmFrame::getCondition<int16_t>(v1, v2, condType);
            }

        case vmDataType::UInt16:
            {
                uint16_t v1 = regs_[regIndex].ax.u16;
                uint16_t v2 = (uint16_t)immValue;
                return vmFrame::getCondition<uint16_t>(v1, v2, condType);
            }

        case vmDataType::Int32:
            {
                int32_t v1 = regs_[regIndex].eax.i32;
                int32_t v2 = (int32_t)immValue;
                return vmFrame::getCondition<int32_t>(v1, v2, condType);
            }

        case vmDataType::UInt32:
            {
                uint32_t v1 = regs_[regIndex].eax.u32;
                uint32_t v2 = (uint32_t)immValue;
                return vmFrame::getCondition<uint32_t>(v1, v2, condType);
            }

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmDataType::Int64:
            {
                int64_t v1 = regs_[regIndex].rax.i64;
                int64_t v2 = (int64_t)immValue;
                return vmFrame::getCondition<int64_t>(v1, v2, condType);
            }

        case vmDataType::UInt64:
            {
                uint64_t v1 = regs_[regIndex].rax.u64;
                uint64_t v2 = (uint64_t)immValue;
                return vmFrame::getCondition<uint64_t>(v1, v2, condType);
            }
#endif
        case vmDataType::Pointer:
            {
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
                void * v1 = (void *)regs_[regIndex].rax.u64;
                void * v2 = (void *)immValue;
                return vmFrame::getCondition<void *>(v1, v2, condType);
#else
                void * v1 = (void *)regs_[regIndex].eax.u32;
                void * v2 = (void *)immValue;
                return vmFrame::getCondition<void *>(v1, v2, condType);
#endif
            }
        default:
            assert(false);
            return false;
        }
    }

    bool condCmp_ra(uint32_t regIndex, void * address,
                    uint32_t dataType, uint8_t condType) {
        //
        return false;
    }

    bool condCmp_aa(void * address1, void * address2,
                    uint32_t dataType, uint32_t condType) {
        //
        return false;
    }

    bool condCmp_ai(void * address, uint32_t dataType,
                    basic_type immValue, uint8_t condType) {
        //
        return false;
    }
};

template <typename BasicType, bool IsBackwardPtr = false>
class vmStack {
public:
    typedef BasicType           basic_type;
    typedef size_t              size_type;
    typedef vmFrame<basic_type> frame_type;

private:
    unsigned char * sp_;
    unsigned char * sp_first_;
    unsigned char * sp_last_;
    frame_type *    frame_;
    size_type       capacity_;

    static const bool kIsBackwardPtr = IsBackwardPtr;

public:
    vmStack(size_type capacity = 0)
        : sp_(nullptr), sp_first_(nullptr), sp_last_(nullptr),
          frame_(nullptr), capacity_(capacity) {
    }
    vmStack(frame_type * frame, size_type capacity = 0)
        : sp_(nullptr), sp_first_(nullptr), sp_last_(nullptr),
          frame_(frame), capacity_(capacity) {
    }
    ~vmStack() {
        destroy();
    }

    bool isBackwardPtr() const { return kIsBackwardPtr; }
    int direction() const {
        if (isBackwardPtr())
            return -1;
        else
            return 1;
    }

    bool isInited() const { return (sp_first_ != nullptr); }

    bool isEof() const {
        if (isBackwardPtr())
            return (sp_ == sp_first_);
        else
            return (sp_ == sp_last_);
    }

    bool isOverflow() const {
        if (isBackwardPtr())
            return (sp_ <= sp_first_);
        else
            return (sp_ >= sp_last_);
    }

    bool isUnderflow() const {
        if (isBackwardPtr())
            return (sp_ > sp_last_);
        else
            return (sp_ < sp_first_);
    }

    bool isReachBegin() const {
        if (isBackwardPtr())
            return (sp_ >= sp_last_);
        else
            return (sp_ <= sp_first_);
    }

    size_type size() const {
        if (isBackwardPtr())
            return (size_type)((uintptr_t)sp_last_ - (uintptr_t)sp_);
        else
            return (size_type)((uintptr_t)sp_ - (uintptr_t)sp_first_);
    }
    size_type capacity() const { return capacity_; }

    frame_type * getFrame() const { return frame_; }
    void setFrame(frame_type * frame) {
        this->frame_ = frame;
    }

    unsigned char * current() const { return sp_; }
    unsigned char * first() const { return sp_first_; }
    unsigned char * last() const { return sp_last_; }

    inline void create(size_type capacity) {
#if defined(_WIN32)
        if (sp_first_) {
            _aligned_free(sp_first_);
        }
        sp_first_ = (unsigned char *)_aligned_malloc(capacity, 64);
#else
        if (sp_first_) {
            free(sp_first_);
        }
        int ret = posix_memalign((void **)&sp_first_, 64, capacity);
#endif // _WIN32

#ifndef NDEBUG
        memset((void *)sp_first_, 0, sizeof(char) * capacity);
#endif
        sp_last_ = sp_first_ + capacity;
        if (isBackwardPtr())
            sp_ = sp_last_;
        else
            sp_ = sp_first_;
        capacity_ = capacity;
    }

    inline void destroy() {
        sp_ = nullptr;
        if (sp_first_) {
#if defined(_WIN32)
            _aligned_free(sp_first_);
#else
            free(sp_first_);
#endif
            sp_first_ = nullptr;
        }
        sp_last_ = nullptr;
        frame_ = nullptr;
        capacity_ = 0;
    }

    void back() {
        this->sp_ -= direction() * 1;
    }

    void back(int offset) {
        this->sp_ -= direction() * offset;
    }

    void backInt8() {
        this->sp_ -= direction() * sizeof(int8_t);
    }

    void backUInt8() {
        this->sp_ -= direction() * sizeof(uint8_t);
    }

    void backInt16() {
        this->sp_ -= direction() * sizeof(int16_t);
    }

    void backUInt16() {
        this->sp_ += direction() * sizeof(uint16_t);
    }

    void backInt32() {
        this->sp_ -= direction() * sizeof(int32_t);
    }

    void backUInt32() {
        this->sp_ -= direction() * sizeof(uint32_t);
    }

    void backInt64() {
        this->sp_ -= direction() * sizeof(int64_t);
    }

    void backUInt64() {
        this->sp_ -= direction() * sizeof(uint64_t);
    }

    void backPointer() {
        this->sp_ -= direction() * sizeof(void *);
    }

    void next() {
        this->sp_ += direction() * 1;
    }

    void next(int offset) {
        this->sp_ += direction() * offset;
    }

    void nextInt8() {
        this->sp_ += direction() * sizeof(int8_t);
    }

    void nextUInt8() {
        this->sp_ += direction() * sizeof(uint8_t);
    }

    void nextInt16() {
        this->sp_ += direction() * sizeof(int16_t);
    }

    void nextUInt16() {
        this->sp_ += direction() * sizeof(uint16_t);
    }

    void nextInt32() {
        this->sp_ += direction() * sizeof(int32_t);
    }

    void nextUInt32() {
        this->sp_ += direction() * sizeof(uint32_t);
    }

    void nextInt64() {
        this->sp_ += direction() * sizeof(int64_t);
    }

    void nextUInt64() {
        this->sp_ += direction() * sizeof(uint64_t);
    }

    void nextPointer() {
        this->sp_ += direction() * sizeof(void *);
    }

    unsigned char get() const {
        return *sp_;
    }

    int8_t getInt8() const {
        return *(int8_t *)sp_;
    }

    uint8_t getUInt8() const {
        return *(uint8_t *)sp_;
    }

    int16_t getInt16() const {
        return *(int16_t *)sp_;
    }

    uint16_t getUInt16() const {
        return *(uint16_t *)sp_;
    }

    int32_t getInt32() const {
        return *(int32_t *)sp_;
    }

    uint32_t getUInt32() const {
        return *(uint32_t *)sp_;
    }

    int64_t getInt64() const {
        return *(int64_t *)sp_;
    }

    uint64_t getUInt64() const {
        return *(uint64_t *)sp_;
    }

    void * getPointer() const {
        return (void *)(*(uintptr_t *)sp_);
    }

    void setInt8(int8_t value) {
        *(int8_t *)sp_ = value;
    }

    void setUInt8(uint8_t value) {
        *(uint8_t *)sp_ = value;
    }

    void setInt16(int16_t value) {
        *(int16_t *)sp_ = value;
    }

    void setUInt16(uint16_t value) {
        *(uint16_t *)sp_ = value;
    }

    void setInt32(int32_t value) {
        *(int32_t *)sp_ = value;
    }

    void setUInt32(uint32_t value) {
        *(uint32_t *)sp_ = value;
    }

    void setInt64(int64_t value) {
        *(int64_t *)sp_ = value;
    }

    void setUInt64(uint64_t value) {
        *(uint64_t *)sp_ = value;
    }

    void setPointer(void * ptr) {
        *(uintptr_t *)sp_ = (uintptr_t)ptr;
    }

    inline basic_type push_reg(reg_t reg) {
        uint32_t regType = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        return push_reg(regType, regIndex);
    }

    inline basic_type push_reg(uint32_t regType, uint32_t regIndex) {
        assert(frame_ != nullptr);
        basic_type value;
        switch (regType) {
        case vmRegType::r8:
            {
                uint8_t value8 = frame_->getRegValue8L(regIndex);
                setUInt8(value8);
                nextUInt8();
                value = (basic_type)value8;
                break;
            }
        case vmRegType::r8_high:
            {
                uint8_t value8 = frame_->getRegValue8H(regIndex);
                setUInt8(value8);
                nextUInt8();
                value = (basic_type)value8;
                break;
            }
        case vmRegType::r16:
            {
                uint16_t value16 = frame_->getRegValue16(regIndex);
                setUInt16(value16);
                nextUInt16();
                value = (basic_type)value16;
                break;
            }
        case vmRegType::r32:
            {
                uint32_t value32 = frame_->getRegValue32(regIndex);
                setUInt32(value32);
                nextUInt32();
                value = (basic_type)value32;
                break;
            }
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            {
                uint64_t value64 = frame_->getRegValue64(regIndex);
                setUInt64(value64);
                nextUInt64();
                value = (basic_type)value64;
                break;
            }
#endif
        default:
            assert(false);
            value = -1;
            break;
        }
        return value;
    }

    inline basic_type pop_reg(uint32_t regType, uint32_t regIndex) {
        assert(frame_ != nullptr);
        basic_type value = 0;
        switch (regType) {
        case vmRegType::r8:
            {
                backUInt8();
                uint8_t value8 = getUInt8();
                frame_->setRegValue8L(regIndex, value8);
                value = (basic_type)value8;
                break;
            }
        case vmRegType::r8_high:
            {
                backUInt8();
                uint8_t value8 = getUInt8();
                frame_->setRegValue8H(regIndex, value8);
                value = (basic_type)value8;
                break;
            }
        case vmRegType::r16:
            {
                backUInt16();
                uint16_t value16 = getUInt16();
                frame_->setRegValue16(regIndex, value16);
                value = (basic_type)value16;
                break;
            }
        case vmRegType::r32:
            {
                backUInt32();
                uint32_t value32 = getUInt32();
                frame_->setRegValue32(regIndex, value32);
                value = (basic_type)value32;
                break;
            }
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
        case vmRegType::r64:
            {
                backUInt64();
                uint64_t value64 = getUInt64();
                frame_->setRegValue64(regIndex, value64);
                value = (basic_type)value64;
                break;
            }
#endif
        default:
            assert(false);
            value = -1;
            break;
        }
        return value;
    }

    inline basic_type pop_reg(reg_t reg) {
        uint32_t regType = vmReg::getType(reg);
        uint32_t regIndex = vmReg::getIndex(reg);
        return pop_reg(regType, regIndex);
    }

    bool push_int32(int32_t value) {
        setInt32(value);
        nextInt32();
        return true;
    }

    bool push_uint32(uint32_t value) {
        setUInt32(value);
        nextUInt32();
        return true;
    }

    bool push_int64(int64_t value) {
        setInt64(value);
        nextInt64();
        return true;
    }

    bool push_uint64(uint64_t value) {
        setUInt64(value);
        nextUInt64();
        return true;
    }

    bool pop_int32(int32_t & value) {
        backInt32();
        value = getInt32();
        return true;
    }

    bool pop_uint32(uint32_t & value) {
        backUInt32();
        value = getUInt32();
        return true;
    }

    bool pop_int64(int64_t & value) {
        backInt64();
        value = getInt64();
        return true;
    }

    bool pop_uint64(uint64_t & value) {
        backUInt64();
        value = getUInt64();
        return true;
    }

    void push_callstack(unsigned char * returnFP) {
        setPointer(returnFP);
        nextPointer();
        assert(!isOverflow());
    }

    unsigned char * pop_callstack() {
        backPointer();
        void * returnFP = getPointer();
        return (unsigned char *)returnFP;
    }
};

template <typename BasicType>
class vmHeap {
public:
    typedef BasicType   basic_type;
    typedef size_t      size_type;

public:
    vmHeap() {}
    ~vmHeap() {}
};

template <typename BasicType = uintptr_t>
class vmReturn {
public:
    typedef BasicType basic_type;

    enum DataType {
        Basic,
        Float,
        Double,
        Pointer,
        Struct,
        Last
    };

private:
    basic_type  retValue_;
    uint32_t    dataType_;
    uint32_t    dataLen_;

public:
    vmReturn() : retValue_(0), dataType_(DataType::Basic), dataLen_(0) {}
    ~vmReturn() {}

    bool isValid() const {
        return (dataType_ >= DataType::Basic && dataType_ <= DataType::Struct);
    }

    basic_type getValue() const { return retValue_; }
    void setValue(basic_type value) { retValue_ = value; }

    uint32_t getDataType() const { return dataType_; }
    uint32_t getDataLength() const { return dataLen_; }

    void setDataType(uint32_t dataType, uint32_t dataLen = 0) {
        dataType_ = dataType;
        switch (dataType) {
        case DataType::Basic:
            dataLen_ = sizeof(basic_type);
            break;

        case DataType::Float:
            dataLen_ = sizeof(float);
            break;

        case DataType::Double:
            dataLen_ = sizeof(double);
            break;

        case DataType::Pointer:
            dataLen_ = sizeof(void *);
            break;

        case DataType::Struct:
            dataLen_ = dataLen;
            break;

        default:
            assert(false);
            dataLen_ = (uint32_t)-1;
            break;
        }
    }
};

} // namespace jlang

#endif // JLANG_VM_INTERPRETER_COMMON_H
