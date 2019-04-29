
//
// This is a token define entries file,
// only allowed to be included in the /lang/Error.h file.
// Note: Don't include this file directly.
//

#ifndef JLANG_LANG_ERROR_H

    #error "/lang/ErrorDef.h only allowed to be included in the /lang/Error.h file."

#else

    #ifndef _Err
    #define _Err(err)
    #endif

    // Standard
    _Err(NotImplement)
    _Err(EndOfFile)

    // FileSystem
    _Err(IllegalPathOrFilename)

    // Preprocessing statement errors
    _Err(IllegalPreprocessingKeyword)
    _Err(UnknownPreprocessingKeyword)

    // Include file errors
    _Err(IncludeFile_UnknownQuote)
    _Err(IncludeFile_SuffixQuoteMismatch)
    _Err(IllegalPath)
    _Err(IllegalDirectory)
    _Err(IllegalFilename)
    _Err(IllegalDirectoryOrFilename)
    _Err(IllegalDirectoryOrFilenameCharacters)

    // Macro errors
    _Err(IllegalMacroDefineName)
    _Err(IllegalMacroDefineBody)

    // Standard errors
    _Err(IllegalDoubleDot)
    _Err(IllegalFourDot)

    // Comment errors
    _Err(IllegalLineComment)
    _Err(IllegalBlockComment)
    _Err(IllegalNestedBlockComment)
    _Err(IllegalCommentIsNotCompleted)

    // Constant errors
    _Err(IllegalNumber)
    _Err(IllegalRadixNumber)
    _Err(IllegalRadix2Number)
    _Err(IllegalRadix8Number)
    _Err(IllegalRadix10Number)
    _Err(IllegalRadix16Number)
    _Err(IllegalFloatingNumber)
    _Err(IllegalFloatNumber)
    _Err(IllegalDoubleNumber)

    // Real number errors
    _Err(ErrorIntegerPartOverflow)
    _Err(ErrorFractionalPartOverflow)
    _Err(ErrorExponentPartOverflow)
    _Err(ErrorNegativeIntegerPartOverflow)
    _Err(ErrorNegativeFractionalPartOverflow)
    _Err(ErrorNegativeExponentPartOverflow)
    _Err(ErrorExponentPartOutOfRange)
    _Err(IllegalExponentPart)

    // String constant errors

    // Pod type
    _Err(IllegalPodType)
    _Err(UnknownPodType)
    _Err(UnsupportedPodType)

    // Single character constant errors
    _Err(IllegalSingleCharacterFormat)
    _Err(UnknownEscapedChar)
    _Err(UnknownUnescapedChar)

    // String literal
    _Err(ExpectingStringLiteral)
    _Err(IllegalStringLiteral)
    _Err(IllegalStringLiteralIsNotCompleted)

    // Section statement
    _Err(IllegalSectionStatement)
    _Err(UnsupportedSectionStatement)

    // String section
    _Err(IllegalStringSection)
    _Err(ExpectingStringSectionBeginning)
    _Err(ExpectingStringSectionEnding)
    _Err(IllegalStringSectionIdentifier)
    _Err(IllegalStringSectionStringLiteral)

    // Statements
    _Err(IllegalStatement)

    // Instruction
    _Err(UnsupportedInstruction)
    _Err(IllegalInstruction)

    // Operand
    _Err(UnsupportedOperand)
    _Err(UnsupportedOperandDataType)
    _Err(IllegalOperand)
    _Err(IllegalOperandNumber)
    _Err(ExpectedSecondOperand)

    // Identifer
    _Err(IllegalIdentifer)

    // Function argument list
    _Err(IllegalArgumentType)
    _Err(IllegalArgumentName)
    _Err(IllegalArgumentDelimiter)

    // Function body
    _Err(IllegalFunctionBody)

    // Standard errors
    _Err(Error_NullPtr)

    // vmThread
    _Err(Thread_Not_Create)
    _Err(MainProcess_Create_Failed)
    _Err(MainThread_Create_Failed)

    // vmBinary
    _Err(BinaryFile_Read_Failed)

    #undef _Err

#endif
