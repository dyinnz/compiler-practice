//
// Created by coder on 16-9-27.
//

#pragma once

#include "grammar.h"
#include "tokenizer.h"

namespace golike_grammar {

constexpr int kStartID = 10000;

/**
 * 16 keywords, 9 unsupported keywords
 */
DECLARE_SYMBOL(kBreak, 11)
DECLARE_SYMBOL(kCase, 12)
// DECLARE_SYMBOL(kChan, 13)
DECLARE_SYMBOL(kConst, 14)
DECLARE_SYMBOL(kContinue, 15)

DECLARE_SYMBOL(kDefault, 16)
// DECLARE_SYMBOL(kDefer, 17)
DECLARE_SYMBOL(kElse, 18)
// DECLARE_SYMBOL(kFallthrough, 19)
DECLARE_SYMBOL(kFor, 20)

DECLARE_SYMBOL(kFunc, 21)
// DECLARE_SYMBOL(kGo, 22)
DECLARE_SYMBOL(kGoto, 23)
DECLARE_SYMBOL(kIf, 24)
DECLARE_SYMBOL(kImport, 25)

// DECLARE_SYMBOL(kInterface, 26)
// DECLARE_SYMBOL(kMap, 27)
DECLARE_SYMBOL(kPackage, 28)
// DECLARE_SYMBOL(kRange, 29)
// DECLARE_SYMBOL(kReturn, 30)

// DECLARE_SYMBOL(kSelect, 31)
DECLARE_SYMBOL(kStruct, 32)
DECLARE_SYMBOL(kSwitch, 33)
DECLARE_SYMBOL(kType, 34)
DECLARE_SYMBOL(kVar, 35)

/**
 * Identifier
 */
DECLARE_SYMBOL(kIdentifier, 40)


/**
 * basic type
 */
DECLARE_SYMBOL(kIntType, 41)
DECLARE_SYMBOL(kFloatType, 42)
DECLARE_SYMBOL(kStringType, 43)


/**
 * Value
 */
DECLARE_SYMBOL(kIntValue, 51)
DECLARE_SYMBOL(kFloatValue, 52)
DECLARE_SYMBOL(kStringValue, 53)

/**
 * Line Feed
 */
DECLARE_SYMBOL(kLF, 60)

/**
 * one-char operator
 */
DECLARE_SYMBOL(kLeftBrace, '{')
DECLARE_SYMBOL(kRightBrace, '}')
DECLARE_SYMBOL(kLeftParen, '(')
DECLARE_SYMBOL(kRightParen, ')')
DECLARE_SYMBOL(kLeftSquare, '[')
DECLARE_SYMBOL(kRightSquare, ']')

DECLARE_SYMBOL(kDot, '.')
DECLARE_SYMBOL(kComma, ',')
DECLARE_SYMBOL(kColon, ':')
DECLARE_SYMBOL(kSemicolon, ';')

DECLARE_SYMBOL(kAssign, '=')

DECLARE_SYMBOL(kAdd, '+')
DECLARE_SYMBOL(kSub, '-')
DECLARE_SYMBOL(kMul, '*')
DECLARE_SYMBOL(kDiv, '/')
DECLARE_SYMBOL(kMod, '%')

DECLARE_SYMBOL(kBitAnd, '&')
DECLARE_SYMBOL(kBitOr, '|')
DECLARE_SYMBOL(kBitXor, '^')
DECLARE_SYMBOL(kLogicalNegative, '!')

DECLARE_SYMBOL(kLT, '<')
DECLARE_SYMBOL(kGT, '>')

/**
 * multi-char operator
 */
DECLARE_SYMBOL(kLeftShift, 61)  // <<
DECLARE_SYMBOL(kRightShift, 62)  // >>
DECLARE_SYMBOL(kInc, 63) // ++
DECLARE_SYMBOL(kDec, 64) // --
DECLARE_SYMBOL(kLogicalAnd, 65) // &&
DECLARE_SYMBOL(kLogicalOr, 66) // ||

DECLARE_SYMBOL(kLE, 70) // <=
DECLARE_SYMBOL(kGE, 71) // >=
DECLARE_SYMBOL(kEQ, 72) // ==
DECLARE_SYMBOL(kNE, 73) // !=

DECLARE_SYMBOL(kLeftAssign, 81) // <<=
DECLARE_SYMBOL(kRightAssign, 82) // >>=
DECLARE_SYMBOL(kAddAssign, 83)  // +=
DECLARE_SYMBOL(kSubAssign, 84) // -=
DECLARE_SYMBOL(kMulAssign, 85) // *=
DECLARE_SYMBOL(kDivAssign, 86) // /=
DECLARE_SYMBOL(kModAssign, 87) // %=
DECLARE_SYMBOL(kXorAssign, 88) // ^=
DECLARE_SYMBOL(kAndAssign, 89) // &=
DECLARE_SYMBOL(kOrAssign, 90)  // |=

/**
 * @brief   build tokenizer
 * @return  a tokenizer
 */
Tokenizer BuildAnsiCTokenizer();

} // end of golike_grammar
