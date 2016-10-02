//
// Created by coder on 16-9-27.
//

#include "golike_grammar.h"
#include "simplelogger.h"

using std::move;

namespace golike_grammar {

/*----------------------------------------------------------------------------*/
// terminal

/**
 * 17 keywords, 8 unsupported keywords
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
TERMINAL(kReturn)

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
TERMINAL(kIntLit)
TERMINAL(kFloatLit)
TERMINAL(kStringLit)


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
TERMINAL(kShortDecl)  // |=

/*----------------------------------------------------------------------------*/
// non-terminal

// Source
NON_TERMINAL(kPackageClause)
NON_TERMINAL(kImportDeclRecur)
NON_TERMINAL(kTopDeclRecur)

NON_TERMINAL(kIgnore)

// Type
NON_TERMINAL(kTypeName)

// Literal
NON_TERMINAL(kLiteral)

// End Line
NON_TERMINAL(kEndLine)

// Declaration
NON_TERMINAL(kDeclaration)
NON_TERMINAL(kDeclAssign)

NON_TERMINAL(kFunctionDecl)
NON_TERMINAL(kSignature)
NON_TERMINAL(kSignatureReturn)
NON_TERMINAL(kParameterList)
NON_TERMINAL(kParameterRecur)

// Statement
NON_TERMINAL(kBlock)
NON_TERMINAL(kStatement)
NON_TERMINAL(kStmtRecur)
NON_TERMINAL(kStmtList)

// Statement -- single line
NON_TERMINAL(kSimpleStmt)
NON_TERMINAL(kLabeledStmt)
NON_TERMINAL(kReturnStmt)
NON_TERMINAL(kGotoStmt)

NON_TERMINAL(kEmptyStmt)
NON_TERMINAL(kIncDecStmt)
NON_TERMINAL(kIncDecTail)
NON_TERMINAL(kAssignStmt)
NON_TERMINAL(kShortVarDecl)

// Statement -- multi line
NON_TERMINAL(kIfStmt)
NON_TERMINAL(kIfHead)
NON_TERMINAL(kIfHeadRight)
NON_TERMINAL(kElseClause)
NON_TERMINAL(kElseTail)

NON_TERMINAL(kSwitchStmt)
NON_TERMINAL(kSwitchHead)
NON_TERMINAL(kCaseRecur)

NON_TERMINAL(kForStmt)
NON_TERMINAL(kForHead)
NON_TERMINAL(kForHeadRight)

// about Expression
NON_TERMINAL(kExpr)
NON_TERMINAL(kExprRecur)
NON_TERMINAL(kExprList)
NON_TERMINAL(kExprListRecur)
NON_TERMINAL(kExprListLess)

NON_TERMINAL(kUnaryExpr)
NON_TERMINAL(kPrimaryExpr)
NON_TERMINAL(kPrimaryExprRecur)
NON_TERMINAL(kOperand)

// common operator
NON_TERMINAL(kUnaryOp)
NON_TERMINAL(kBinaryOp)

/*----------------------------------------------------------------------------*/

Tokenizer BuildGolikeTokenizer() {
  TokenizerBuilder tokenizer_builder;
  tokenizer_builder
      .SetLineComment("//")
      .SetBlockComment("/*", "*/")
      .SetIgnoreSet({kSpaceSymbol})
      .SetPatterns(
          {
              // space
              {"[ \v\r\f\t]", kSpaceSymbol},
              {"\n", kLFSymbol},
              // keywords
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
              {"return", kReturn},
              {"struct", kStruct},
              {"switch", kSwitch},
              {"type", kType},
              {"var", kVar},
              // multi-char operator
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
              // words
              {R"(\d+)", kIntLit},
              {R"(\d+\.\d*|\.\d+)", kFloatLit},
              {R"("[^"]*")", kStringLit},
              {R"(\w(\w|\d)*)", kIdentifier},
              // one-char operator
              {"{", kLeftBrace},
              {"}", kRightBrace},
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
          });
  assert(!tokenizer_builder.IsError());
  return tokenizer_builder.Build();
}

/*----------------------------------------------------------------------------*/
// grammar

static void TokenFeeder(void *grammar_data, Token &&token) {
  auto golike_data = static_cast<GolikeGrammarData *>(grammar_data);
  logger.debug("{}(): {}", __func__, to_string(token));

  auto ast_node = golike_data->ast()->CreateNode(move(token));
  golike_data->node_record().push_back(ast_node);
}

static void EmptyFunction(void *grammar_data) {
  // do notghing
}

Grammar BuildGolikeGrammar() {
  GrammarBuilder builder;

  builder.SetSymbolTable(
      {
          // Terminal
          // special
          kStartSymbol, kEofSymbol, kSpaceSymbol, kLFSymbol, kEpsilonSymbol,
          // keywords
          kBreak, kCase, kConst, kContinue, kDefault, kElse, kFor, kFunc, kGoto,
          kIf, kImport, kPackage, kReturn, kStruct, kSwitch, kType, kVar,
          // other
          kIdentifier, kIntLit, kFloatLit, kStringLit,
          // one-char  operator
          kLeftBrace, kRightBrace, kLeftParen, kRightParen,
          kLeftSquare, kRightSquare, kDot, kComma, kColon, kSemicolon, kAssign,
          kAdd, kSub, kMul, kDiv, kMod,
          kBitAnd, kBitOr, kBitXor, kLogicalNegative, kLT, kGT,
          // multi-char operator
          kLeftShift, kRightShift, kInc, kDec, kLogicalAnd, kLogicalOr,
          kLE, kGE, kEQ, kNE,
          kLeftAssign, kRightAssign, kXorAssign, kAndAssign, kOrAssign,
          kAddAssign, kSubAssign, kMulAssign, kDivAssign, kModAssign,
          kShortDecl,

          // Non-Terminal
          // source
          kPackageClause, kImportDeclRecur, kTopDeclRecur,
          kIgnore,
          // type & literal
          kTypeName, kLiteral,
          // declaration
          kDeclaration, kDeclAssign, kFunctionDecl,
          kSignature, kSignatureReturn, kParameterList, kParameterRecur,
          // statement
          kBlock, kStatement, kStmtRecur, kStmtList,
          // statement -- single line
          kReturnStmt, kGotoStmt, kIncDecStmt, kIncDecTail,
          kAssignStmt, kShortVarDecl,
          // statement -- multi line
          kIfStmt, kIfHead, kIfHeadRight, kElseClause, kElseTail,
          kSwitchStmt, kSwitchHead, kCaseRecur,
          kForStmt, kForHead, kForHeadRight,
          // expression
          kExpr, kExprRecur, kExprList, kExprListRecur, kExprListLess,
          kUnaryExpr, kPrimaryExpr, kPrimaryExprRecur, kOperand,
          // common operator & End Line
          kUnaryOp, kBinaryOp, kEndLine,
      });

  builder.SetTokenFeeder(TokenFeeder);

  // End Line & Ignore
  builder.InsertRule(kEndLine, {kLFSymbol}, EmptyFunction);
  builder.InsertRule(kEndLine, {kSemicolon}, EmptyFunction);

  builder.InsertRule(kIgnore, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kIgnore, {kEndLine, kIgnore},
                     EmptyFunction);

  // Type Name
  builder.InsertRule(kTypeName, {kIdentifier},
                     EmptyFunction);
  builder.InsertRule(kTypeName, {kFunc, kSignature},
                     EmptyFunction);

  // Literal
  builder.InsertRule(kLiteral, {kIntLit},
                     EmptyFunction);
  builder.InsertRule(kLiteral, {kFloatLit},
                     EmptyFunction);
  builder.InsertRule(kLiteral, {kStringLit},
                     EmptyFunction);

  // Source
  builder.InsertRule(
      kStartSymbol,
      {kPackageClause, kIgnore, kImportDeclRecur, kTopDeclRecur},
      EmptyFunction);
  // Package
  builder.InsertRule(kPackageClause, {kPackage, kIdentifier, kEndLine},
                     EmptyFunction);

  // Import
  builder.InsertRule(kImportDeclRecur, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kImportDeclRecur,
                     {kImport, kStringLit, kEndLine, kIgnore, kImportDeclRecur},
                     EmptyFunction);

  // Top Declaration
  builder.InsertRule(kTopDeclRecur, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kTopDeclRecur, {kDeclaration, kIgnore, kTopDeclRecur},
                     EmptyFunction);
  builder.InsertRule(kTopDeclRecur, {kFunctionDecl, kIgnore, kTopDeclRecur},
                     EmptyFunction);

  // Top - Normal Declaration
  builder.InsertRule(kDeclaration,
                     {kVar, kIdentifier, kTypeName, kDeclAssign, kEndLine},
                     EmptyFunction);
  builder.InsertRule(kDeclAssign, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kDeclAssign, {kAssign, kExprList},
                     EmptyFunction);

  // Top - Function Declaration
  builder.InsertRule(kFunctionDecl, {kFunc, kIdentifier, kSignature, kBlock},
                     EmptyFunction);
  builder.InsertRule(
      kSignature, {kLeftParen, kParameterList, kRightParen, kSignatureReturn},
      EmptyFunction);
  builder.InsertRule(kParameterList, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kParameterList, {kIdentifier, kTypeName, kParameterRecur},
                     EmptyFunction);
  builder.InsertRule(kParameterRecur, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kParameterRecur,
                     {kComma, kIdentifier, kTypeName, kParameterRecur},
                     EmptyFunction);
  builder.InsertRule(kSignatureReturn, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kSignatureReturn, {kTypeName},
                     EmptyFunction);

  // Block
  builder.InsertRule(kBlock, {kLeftBrace, kStmtRecur, kRightBrace},
                     EmptyFunction);

  // Statement List, >= 1
  builder.InsertRule(kStmtList,
                     {kIgnore, kStatement, kStmtRecur},
                     EmptyFunction);
  // Statement List, >= 0
  builder.InsertRule(kStmtRecur, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kStmtRecur, {kEndLine, kStmtRecur}, EmptyFunction);
  builder.InsertRule(kStmtRecur, {kStatement, kStmtRecur},
                     EmptyFunction);

  // Statement -> Declaration
  builder.InsertRule(kStatement, {kDeclaration}, EmptyFunction);
  // Line statement
  builder.InsertRule(kStatement, {kBreak, kEndLine}, EmptyFunction);
  builder.InsertRule(kStatement, {kContinue, kEndLine}, EmptyFunction);
  builder.InsertRule(kStatement, {kGoto, kIdentifier, kEndLine}, EmptyFunction);
  builder.InsertRule(kStatement, {kReturn, kExprListLess, kEndLine},
                     EmptyFunction);
  builder.InsertRule(kStatement, {kExpr, kEndLine}, EmptyFunction);
  // Block statement
  builder.InsertRule(kStatement, {kBlock}, EmptyFunction);
  builder.InsertRule(kStatement, {kIfStmt}, EmptyFunction);
  builder.InsertRule(kStatement, {kSwitchStmt}, EmptyFunction);
  builder.InsertRule(kStatement, {kForStmt}, EmptyFunction);

  // If
  builder.InsertRule(kIfStmt, {kIf, kIfHead, kBlock, kElseClause},
                     EmptyFunction);

  builder.InsertRule(kIfHead, {kExpr, kIfHeadRight}, EmptyFunction);
  builder.InsertRule(kIfHeadRight, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kIfHeadRight, {kSemicolon, kExpr}, EmptyFunction);

  builder.InsertRule(kElseClause, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kElseClause, {kElse, kElseTail}, EmptyFunction);
  builder.InsertRule(kElseTail, {kBlock}, EmptyFunction);
  builder.InsertRule(kElseTail, {kIfStmt}, EmptyFunction);

  // Switch
  builder.InsertRule(kSwitchStmt, {kSwitch, kSwitchHead,
                                   kLeftBrace, kIgnore, kCaseRecur,
                                   kRightBrace},
                     EmptyFunction);
  builder.InsertRule(kSwitchHead, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kSwitchHead, {kIfHead}, EmptyFunction);
  builder.InsertRule(kCaseRecur, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kCaseRecur,
                     {kCase, kExprList, kColon, kStmtList, kCaseRecur},
                     EmptyFunction);
  builder.InsertRule(kCaseRecur,
                     {kDefault, kColon, kStmtList},
                     EmptyFunction);

  // For
  builder.InsertRule(kForStmt, {kFor, kForHead, kBlock}, EmptyFunction);
  builder.InsertRule(kForHead, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kForHead, {kExpr, kForHeadRight}, EmptyFunction);
  builder.InsertRule(kForHeadRight, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kForHeadRight,
                     {kSemicolon, kExpr, kSemicolon, kExpr},
                     EmptyFunction);

  // Expression List & Recur
  builder.InsertRule(kExprListLess, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kExprListLess, {kExprList}, EmptyFunction);
  builder.InsertRule(kExprList, {kExpr, kExprListRecur}, EmptyFunction);
  builder.InsertRule(kExprListRecur, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kExprListRecur,
                     {kComma, kExpr, kExprListRecur},
                     EmptyFunction);
  // Expression
  builder.InsertRule(kExpr, {kLiteral}, EmptyFunction);

  return builder.Build();
}

std::shared_ptr<GolikeGrammarData> CreateGolikeGrammarData() {
  return std::make_shared<GolikeGrammarData>();
}

} // end of namespace golike_grammar
