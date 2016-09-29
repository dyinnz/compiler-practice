//
// Created by coder on 16-9-27.
//

#include "golike_grammar.h"

namespace golike_grammar {

/**
 * 16 keywords, 9 unsupported keywords
 */
TERMINAL(kBreak)
TERMINAL(kCase)
// TERMINAL(kChan)
TERMINAL(kConst)
TERMINAL(kContinue)

TERMINAL(kDefault)
// TERMINAL(kDefer)
TERMINAL(kElse)
// TERMINAL(kFallthrough)
TERMINAL(kFor)

TERMINAL(kFunc)
// TERMINAL(kGo)
TERMINAL(kGoto)
TERMINAL(kIf)
TERMINAL(kImport)

// TERMINAL(kInterface)
// TERMINAL(kMap)
TERMINAL(kPackage)
// TERMINAL(kRange)
// TERMINAL(kReturn)

// TERMINAL(kSelect)
TERMINAL(kStruct)
TERMINAL(kSwitch)
TERMINAL(kType)
TERMINAL(kVar)

/**
 * Identifier
 */
TERMINAL(kIdentifier)

/**
 * basic type
 */
TERMINAL(kIntType)
TERMINAL(kFloatType)
TERMINAL(kStringType)

/**
 * Value
 */
TERMINAL(kIntValue)
TERMINAL(kFloatValue)
TERMINAL(kStringValue)

/**
 * Line Feed
 */
TERMINAL(kLF)

/**
 * one-char operator
 */
TERMINAL(kLeftBrace)
TERMINAL(kRightBrace)
TERMINAL(kLeftParen)
TERMINAL(kRightParen)
TERMINAL(kLeftSquare)
TERMINAL(kRightSquare)

TERMINAL(kDot)
TERMINAL(kComma)
TERMINAL(kColon)
TERMINAL(kSemicolon)

TERMINAL(kAssign)

TERMINAL(kAdd)
TERMINAL(kSub)
TERMINAL(kMul)
TERMINAL(kDiv)
TERMINAL(kMod)

TERMINAL(kBitAnd)
TERMINAL(kBitOr)
TERMINAL(kBitXor)
TERMINAL(kLogicalNegative)

TERMINAL(kLT)
TERMINAL(kGT)

/**
 * multi-char operator
 */
TERMINAL(kLeftShift)  // <<
TERMINAL(kRightShift)  // >>
TERMINAL(kInc) // ++
TERMINAL(kDec) // --
TERMINAL(kLogicalAnd) // &&
TERMINAL(kLogicalOr) // ||

TERMINAL(kLE) // <=
TERMINAL(kGE) // >=
TERMINAL(kEQ) // ==
TERMINAL(kNE) // !=

TERMINAL(kLeftAssign) // <<=
TERMINAL(kRightAssign) // >>=
TERMINAL(kAddAssign)  // +=
TERMINAL(kSubAssign) // -=
TERMINAL(kMulAssign) // *=
TERMINAL(kDivAssign) // /=
TERMINAL(kModAssign) // %=
TERMINAL(kXorAssign) // ^=
TERMINAL(kAndAssign) // &=
TERMINAL(kOrAssign)  // |=

} // end of namespace golike_grammar
