
#ifndef JLANG_BASIC_VALUETYPE_H
#define JLANG_BASIC_VALUETYPE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/coredef.h"

namespace jlang {

class JlvmContext;

class ValueType {
public:
    enum MachineType {
        Unknown             = -1,

        FirstType           = (Unknown + 1),   // The first value type

        Bit1                = 0x0000,
        Int8                = 0x0001,
        UInt8               = 0x0002,
        Int16               = 0x0003,
        UInt16              = 0x0004,
        Int32               = 0x0005,
        UInt32              = 0x0006,
        Int64               = 0x0007,
        UInt64              = 0x0008,
        Int128              = 0x0009,
        UInt128             = 0x000A,

        Pointer             = 0x000B,
        Void                = 0x000C,

        IntegerTypeFirst    = Bit1,
        IntegerTypeLast     = UInt128,

        Fp8                 = 0x0010,
        Fp16                = 0x0011,
        Fp32                = 0x0012,
        Fp64                = 0x0013,
        Fp80                = 0x0014,
        Fp128               = 0x0015,
        PPCFp128            = 0x0016,

        MaxPodType          = (PPCFp128 + 1),

        FloatingTypeFirst   = Fp8,
        FloatingTypeLast    = PPCFp128,

        MetaData            = 0x0020,

        LastType            = 0x003F,   // This always remains at the end of the list.

        // This is the current maximum for ValueTypeLast.
        // MachineType::MaxAllowed is used for asserts and to size bit vectors.
        // This value must be a multiple of 32.
        MaxAllowed          = 0x0040,

        // SIMD type
        x86_mmx             = 0x0080,
        x86_xmm             = 0x0081,
        x64_vmm             = 0x0082,

        SIMDTypeFirst       = x86_mmx,
        SIMDTypeLast        = x64_vmm,

        MaxType
    }; // enum MachineType

    enum {
        TypeMask            = 0x0000000F,
        IntegerBits         = 0x00000000,
        FloatingBits        = 0x00000010,
        MetaDataBits        = 0x00000020,
        SIMDBits            = 0x00000080
    };

private:
    ValueType::MachineType type_;

public:
    ValueType() : type_(ValueType::Unknown) {}
    ValueType(MachineType type) : type_(type) {}
    virtual ~ValueType() {}

    // operators
    bool operator > (const ValueType & v) const { return type_ >  v.type_; }
    bool operator < (const ValueType & v) const { return type_ <  v.type_; }
    bool operator ==(const ValueType & v) const { return type_ == v.type_; }
    bool operator !=(const ValueType & v) const { return type_ != v.type_; }
    bool operator >=(const ValueType & v) const { return type_ >= v.type_; }
    bool operator <=(const ValueType & v) const { return type_ <= v.type_; }

    MachineType getType() const { return type_; }
    void setType(MachineType type) { type_ = type; }

    bool isPodType() const {
        return (type_ < ValueType::MaxPodType && type_ > ValueType::Unknown);
    }

    bool isIntegerType() const {
#if 1
        return ((type_ & (~ValueType::TypeMask)) == ValueType::IntegerBits);
#else
        return (type_ <= ValueType::IntegerTypeLast && type_ >= ValueType::IntegerTypeFirst);
#endif
    }

    bool isFloatingType() const {
#if 1
        return ((type_ & (~ValueType::TypeMask)) == ValueType::FloatingBits);
#else
        return (type_ >= ValueType::FloatingTypeFirst && type_ <= ValueType::FloatingTypeLast);
#endif
    }

    bool isMetaDataType() const {
#if 0
        return ((type_ & (~ValueType::TypeMask)) == ValueType::MetaDataBits);
#else
        return (type_ == ValueType::MetaData);
#endif
    }

    bool isSIMDType() const {
#if 1
        return ((type_ & (~ValueType::TypeMask)) == ValueType::SIMDBits);
#else
        return (type_ >= ValueType::SIMDTypeFirst && type_ <= ValueType::SIMDTypeLast);
#endif
    }

    unsigned int getWordLength() const {
        switch (type_) {
            default:
                // getWordLength() called on extended MachineType.
                break;
            case MachineType::Unknown:
                // Value type is a unknown type.
                break;
            case MachineType::MetaData:
                // Value type is metadata.
                break;
            case MachineType::Bit1   :
                return 1;
            case MachineType::Int8   :
            case MachineType::UInt8  :
            case MachineType::Fp8    :
                return 8;
            case MachineType::Int16  :
            case MachineType::UInt16 :
            case MachineType::Fp16   :
                return 16;
            case MachineType::Int32  :
            case MachineType::UInt32 :
            case MachineType::Fp32   :
                return 32;
            case MachineType::Int64  :
            case MachineType::UInt64 :
            case MachineType::Fp64   :
            case MachineType::x86_mmx :
                return 64;
            case MachineType::Int128 :
            case MachineType::UInt128:
            case MachineType::Fp128  :
            case MachineType::x86_xmm :
                return 128;
            case MachineType::x64_vmm :
                return 256;
        }
        return 0;
    }

    /// getStoredBytes - Return the number of bytes overwritten by a store
    /// of the specified value type.
    unsigned int getStoredBytes() const {
        return (getWordLength() + 7) / 8;
    }

    /// getStoredBits - Return the number of bits overwritten by a store
    /// of the specified value type.
    unsigned int getStoredBits() const {
        return getStoredBytes() * 8;
    }
};

class PodType : public ValueType {
public:
    PodType() {}
    virtual ~PodType() {}
};

} // namespace jlang

#endif // JLANG_BASIC_VALUETYPE_H
