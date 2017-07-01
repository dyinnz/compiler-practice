//
// Created by coder on 16-9-27.
//

#pragma once

#include "grammar.h"
#include "tokenizer.h"

namespace golike_grammar {

/**
 * 17 keywords, 8 unsupported keywords
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
DECLARE_SYMBOL(kReturn, 30)

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
 * Value
 */
DECLARE_SYMBOL(kIntLit, 41)
DECLARE_SYMBOL(kFloatLit, 42)
DECLARE_SYMBOL(kStringLit, 43)

/**
 * one-char operator
 */
DECLARE_CHAR_SYMBOL(kLeftBrace, '{')
DECLARE_CHAR_SYMBOL(kRightBrace, '}')
DECLARE_CHAR_SYMBOL(kLeftParen, '(')
DECLARE_CHAR_SYMBOL(kRightParen, ')')
DECLARE_CHAR_SYMBOL(kLeftSquare, '[')
DECLARE_CHAR_SYMBOL(kRightSquare, ']')

DECLARE_CHAR_SYMBOL(kDot, '.')
DECLARE_CHAR_SYMBOL(kComma, ',')
DECLARE_CHAR_SYMBOL(kColon, ':')
DECLARE_CHAR_SYMBOL(kSemicolon, ';')
DECLARE_CHAR_SYMBOL(kAssign, '=')

DECLARE_CHAR_SYMBOL(kAdd, '+')
DECLARE_CHAR_SYMBOL(kSub, '-')
DECLARE_CHAR_SYMBOL(kMul, '*')
DECLARE_CHAR_SYMBOL(kDiv, '/')
DECLARE_CHAR_SYMBOL(kMod, '%')

DECLARE_CHAR_SYMBOL(kBitAnd, '&')
DECLARE_CHAR_SYMBOL(kBitOr, '|')
DECLARE_CHAR_SYMBOL(kBitXor, '^')
DECLARE_CHAR_SYMBOL(kLogicalNeg, '!')

DECLARE_CHAR_SYMBOL(kLT, '<')
DECLARE_CHAR_SYMBOL(kGT, '>')

/**
 * multi-char operator
 */
DECLARE_SYMBOL(kBitClear, 60) // &^

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

DECLARE_SYMBOL(kLeftAssign, 80) // <<=
DECLARE_SYMBOL(kRightAssign, 81) // >>=
DECLARE_SYMBOL(kXorAssign, 82) // ^=
DECLARE_SYMBOL(kAndAssign, 83) // &=
DECLARE_SYMBOL(kOrAssign, 84) // |=

DECLARE_SYMBOL(kAddAssign, 85)  // +=
DECLARE_SYMBOL(kSubAssign, 86) // -=
DECLARE_SYMBOL(kMulAssign, 87) // *=
DECLARE_SYMBOL(kDivAssign, 88) // /=
DECLARE_SYMBOL(kModAssign, 89) // %=
DECLARE_SYMBOL(kShortDecl, 90) // :=

/*----------------------------------------------------------------------------*/
// non-terminal

// Source
DECLARE_SYMBOL(kPackageClause, 200)
DECLARE_SYMBOL(kImportDeclRecur, 201)
DECLARE_SYMBOL(kTopDeclRecur, 202)

DECLARE_SYMBOL(kIgnore, 204)

// Type
DECLARE_SYMBOL(kTypeName, 210)
DECLARE_SYMBOL(kLiteral, 211)

// Declaration
DECLARE_SYMBOL(kDeclaration, 220)
DECLARE_SYMBOL(kDeclAssign, 221)

DECLARE_SYMBOL(kFunctionDecl, 222)
DECLARE_SYMBOL(kSignature, 223)
DECLARE_SYMBOL(kSignatureReturn, 224)
DECLARE_SYMBOL(kParameterList, 225)
DECLARE_SYMBOL(kParameterRecur, 226)

// Statement
DECLARE_SYMBOL(kBlock, 240)
DECLARE_SYMBOL(kStatement, 241)
DECLARE_SYMBOL(kStmtRecur, 242)
DECLARE_SYMBOL(kStmtList, 243)

// Statement -- single line
DECLARE_SYMBOL(kReturnStmt, 253)
DECLARE_SYMBOL(kGotoStmt, 254)

DECLARE_SYMBOL(kIncDecStmt, 256)
DECLARE_SYMBOL(kIncDecTail, 257)
DECLARE_SYMBOL(kAssignStmt, 258)
DECLARE_SYMBOL(kShortVarDecl, 269)

// Statement -- multi line
DECLARE_SYMBOL(kIfStmt, 280)
DECLARE_SYMBOL(kIfHead, 281)
DECLARE_SYMBOL(kIfHeadRight, 282)
DECLARE_SYMBOL(kElseClause, 283)
DECLARE_SYMBOL(kElseTail, 284)

DECLARE_SYMBOL(kSwitchStmt, 290)
DECLARE_SYMBOL(kSwitchHead, 291)
DECLARE_SYMBOL(kCaseRecur, 292)

DECLARE_SYMBOL(kForStmt, 293)
DECLARE_SYMBOL(kForHead, 294)
DECLARE_SYMBOL(kForHeadRight, 295)

// about Expression
DECLARE_SYMBOL(kExpr, 300)
DECLARE_SYMBOL(kExprRecur, 301)
DECLARE_SYMBOL(kExprList, 302)
DECLARE_SYMBOL(kExprListRecur, 303)
DECLARE_SYMBOL(kExprListLess, 304)

DECLARE_SYMBOL(kComplexExpr, 305)
DECLARE_SYMBOL(kRestExpr, 306)

DECLARE_SYMBOL(kUnaryExpr, 310)
DECLARE_SYMBOL(kPrimaryExpr, 311)
DECLARE_SYMBOL(kPrimaryExprRecur, 312)
DECLARE_SYMBOL(kOperand, 313)

// common operator
DECLARE_SYMBOL(kUnaryOp, 320)
DECLARE_SYMBOL(kBinaryOp, 321)
DECLARE_SYMBOL(kCommonAssign, 322)

// EndLine
DECLARE_SYMBOL(kEndLine, 45)

/**
 * @brief   build golike-language tokenizer
 * @return  a tokenizer
 */
Tokenizer BuildGolikeTokenizer();

/**
 * @brief   build golike-language grammar
 * @return  a grammar
 */
Grammar BuildGolikeGrammar();

/**
 * @brief   grammar data passed to LL(1) Parser
 */
class GolikeGrammarData {
 public:
  GolikeGrammarData() : ast_(std::make_shared<Ast>()) {}

  std::vector<AstNode *> &node_stack() {
    return node_stack_;
  }

  std::shared_ptr<Ast> ast() {
    return ast_;
  }

 private:
  std::shared_ptr<Ast> ast_;
  std::vector<AstNode *> node_stack_;
};

/**
 * @brief   create golike grammar data
 * @return  golike grammar data
 */
std::shared_ptr<GolikeGrammarData> CreateGolikeGrammarData();

} // end of golike_grammar
