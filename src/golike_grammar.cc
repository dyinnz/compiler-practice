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
 * Value
 */
TERMINAL(kIntValue)
TERMINAL(kFloat32Value)
TERMINAL(kStringValue)

/**
 * basic type
 */
TERMINAL(kIntType)
TERMINAL(kFloat32Type)
TERMINAL(kStringType)

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

Tokenizer BuildGolikeTokenizer() {
  TokenizerBuilder tokenizer_builder;
  tokenizer_builder
      .SetLineComment("//")
      .SetBlockComment("/*", "*/")
      .SetIgnoreSet({kSpaceSymbol})
      .SetPatterns(
          {
              {"[ \v\r\f\t]", kSpaceSymbol},
              {"\n", kLFSymbol},
              {"break", kBreak},
              {"case", kCase},
              {"const", kConst},
              {"default", kDefault},
              {"else", kElse},
              {"for", kFor},
              {"func", kFunc},
              {"goto", kGoto},
              {"if", kIf},
              {"import", kImport},
              {"package", kPackage},
              {"struct", kStruct},
              {"switch", kSwitch},
              {"type", kType},
              {"var", kVar},
              {"{", kLeftBrace},
              {"}", kRightParen},
              {R"(\()", kLeftParen},
              {R"(\))", kRightParen},
              {R"(\[)", kLeftSquare},
              {R"(\])", kRightSquare},
              {R"(\.)", kDot},
              {",", kComma},
              {":", kColon},
              {";", kSemicolon},
              {"=", kAssign},
              {R"(\+)", kAdd},
              {"-", kSub},
              {R"(\*)", kMul},
              {"/", kDiv},
              {"%", kMod},
              {"&", kBitAnd},
              {R"(\|)", kBitOr},
              {"^", kBitOr},
              {"!", kLogicalNegative},
              {"<", kLT},
              {">", kGT},
              {"<<", kLeftShift},
              {">>", kRightShift},
              {R"(\+\+)", kInc},
              {"--", kDec},
              {"&&", kLogicalAnd},
              {R"(\|\|)", kLogicalOr},
              {"<=", kLE},
              {">=", kGE},
              {"==", kEQ},
              {"!=", kNE},
              {"<<=", kLeftAssign},
              {">>=", kRightAssign},
              {R"(\+=)", kAddAssign},
              {"-=", kSubAssign},
              {R"(\*=)", kMulAssign},
              {"/=", kDivAssign},
              {"%=", kModAssign},
              {"^=", kXorAssign},
              {"&=", kAndAssign},
              {R"(\|=)", kOrAssign},
              {R"(\w(\w|\d)*)", kIdentifier},
              {R"(\d+\.\d*|\.\d+)", kFloat32Value},
              {R"(\d+)", kIntValue},
              {R"("[^"]*")", kIntValue},
          });
  assert(!tokenizer_builder.IsError());
  return tokenizer_builder.Build();
}

} // end of namespace golike_grammar
