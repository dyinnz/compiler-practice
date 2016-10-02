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
NON_TERMINAL(kStatementRecur)
NON_TERMINAL(kStmtList)
NON_TERMINAL(kStmtListRecur)

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
NON_TERMINAL(kCaseClauseRecur)

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

  builder.SetSymbolTable({
          // special
          kStartSymbol, kEofSymbol, kSpaceSymbol, kLFSymbol, kEpsilonSymbol,
          // keywords
          kBreak, kCase, kConst, kContinue, kDefault, kElse, kFor, kFunc, kGoto,
          kIf, kImport, kPackage, kReturn, kStruct, kSwitch, kType, kVar,
          // other
          kIdentifier, kIntLit, kFloatLit, kStringLit,
          // one-char  operator
          kLeftBrace, kRightBrace, kLeftParen, kRightParen, kLeftSquare,
          kRightSquare, kDot, kComma, kColon, kSemicolon, kAssign,
          kAdd, kSub, kMul, kDiv, kMod,
          kBitAnd, kBitOr, kBitXor, kLogicalNegative, kLT, kGT,
          // multi-char operator
          kLeftShift, kRightShift, kInc, kDec, kLogicalAnd, kLogicalOr,
          kLE, kGE, kEQ, kNE,
          kLeftAssign, kRightAssign, kXorAssign, kAndAssign, kOrAssign,
          kAddAssign, kSubAssign, kMulAssign, kDivAssign, kModAssign,
          kShortDecl,

          // source
          kPackageClause, kImportDeclRecur, kTopDeclRecur,
          kIgnore,
          // type & literal
          kTypeName, kLiteral,
          // declaration
          kDeclaration, kDeclAssign, kFunctionDecl,
          kSignature, kSignatureReturn, kParameterList, kParameterRecur,
          // statement
          kBlock, kStatement, kStatementRecur,
          // statement -- single line
          kSimpleStmt, kLabeledStmt, kReturnStmt, kGotoStmt,
          kEmptyStmt, kIncDecStmt, kAssignStmt, kShortVarDecl,
          // statement -- multi line
          kIfStmt, kIfHead, kIfHeadRight, kElseClause, kElseTail,
          kSwitchStmt, kSwitchHead, kCaseClauseRecur,
          kForStmt,kForHead, kForHeadRight,
          // expression
          kExpr, kExprRecur, kExprList, kExprListRecur, kExprListLess,
          kUnaryExpr, kPrimaryExpr, kPrimaryExprRecur, kOperand,
          // common operator & End Line
          kUnaryOp, kBinaryOp, kEndLine,
      });

  builder.SetTokenFeeder(TokenFeeder);

  // Source
  builder.InsertRule(
      kStartSymbol,
      {kPackageClause, kIgnore, kImportDeclRecur, kTopDeclRecur},
      EmptyFunction);

  builder.InsertRule(kEndLine, {kLFSymbol}, EmptyFunction);
  builder.InsertRule(kEndLine, {kSemicolon}, EmptyFunction);

  builder.InsertRule(kIgnore, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kIgnore, {kEndLine, kIgnore},
                     EmptyFunction);

  builder.InsertRule(kPackageClause, {kPackage, kIdentifier, kEndLine},
                     EmptyFunction);

  // Type Name
  builder.InsertRule(kTypeName, {kIdentifier},
                     EmptyFunction);
  builder.InsertRule(kTypeName, {kFunc, kSignature},
                     EmptyFunction);

  // Declaration
  builder.InsertRule(kImportDeclRecur, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kImportDeclRecur,
                     {kImport, kStringLit, kEndLine, kIgnore, kImportDeclRecur},
                     EmptyFunction);

  builder.InsertRule(kTopDeclRecur, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kTopDeclRecur, {kDeclaration, kIgnore, kTopDeclRecur},
                     EmptyFunction);
  builder.InsertRule(kTopDeclRecur, {kFunctionDecl, kIgnore, kTopDeclRecur},
                     EmptyFunction);

  builder.InsertRule(kDeclaration,
                     {kVar, kIdentifier, kTypeName, kDeclAssign, kEndLine},
                     EmptyFunction);
  builder.InsertRule(kDeclAssign, {kEpsilonSymbol},
                     EmptyFunction);
  builder.InsertRule(kDeclAssign, {kAssign, kExprList},
                     EmptyFunction);

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

  // Statement

  builder.InsertRule(kBlock, {kLeftBrace, kStatementRecur, kRightBrace},
                     EmptyFunction);

  builder.InsertRule(kLiteral, {kIntLit},
                     EmptyFunction);
  builder.InsertRule(kLiteral, {kFloatLit},
                     EmptyFunction);
  builder.InsertRule(kLiteral, {kStringLit},
                     EmptyFunction);

  builder.InsertRule(kStmtList, {kStatement, kStmtListRecur}, EmptyFunction);
  builder.InsertRule(kStmtListRecur, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kStmtListRecur,
                     {kStatement, kStmtListRecur},
                     EmptyFunction);

  builder.InsertRule(kStatementRecur, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kStatementRecur,
                     {kStatement, kStatementRecur},
                     EmptyFunction);

  builder.InsertRule(kStatement, {kEndLine}, EmptyFunction);
  builder.InsertRule(kStatement, {kSimpleStmt}, EmptyFunction);
  builder.InsertRule(kStatement, {kDeclaration}, EmptyFunction);
  builder.InsertRule(kStatement, {kReturnStmt}, EmptyFunction);
  builder.InsertRule(kStatement, {kBreak, kEndLine}, EmptyFunction);
  builder.InsertRule(kStatement, {kContinue, kEndLine}, EmptyFunction);
  builder.InsertRule(kStatement, {kGotoStmt}, EmptyFunction);
  builder.InsertRule(kStatement, {kIfStmt}, EmptyFunction);
  builder.InsertRule(kStatement, {kSwitchStmt}, EmptyFunction);
  builder.InsertRule(kStatement, {kForStmt}, EmptyFunction);

  builder.InsertRule(kSimpleStmt, {kExpr, kEndLine}, EmptyFunction);
  builder.InsertRule(kReturnStmt,
                     {kReturn, kExprListLess, kEndLine},
                     EmptyFunction);
  builder.InsertRule(kGotoStmt, {kGoto, kIdentifier, kEndLine}, EmptyFunction);

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
                                   kLeftBrace, kIgnore, kCaseClauseRecur,
                                   kRightBrace},
                     EmptyFunction);
  builder.InsertRule(kSwitchHead, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kSwitchHead, {kIfHead}, EmptyFunction);
  builder.InsertRule(kCaseClauseRecur, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kCaseClauseRecur,
                     {kCase, kExprList, kColon, kStmtList, kCaseClauseRecur},
                     EmptyFunction);
  builder.InsertRule(kCaseClauseRecur,
                     {kDefault, kColon, kStmtList},
                     EmptyFunction);

  // For
  builder.InsertRule(kForStmt, {kFor, kForHead, kBlock}, EmptyFunction);
  builder.InsertRule(kForHead, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kForHead, {kExpr, kForHeadRight}, EmptyFunction);
  builder.InsertRule(kForHeadRight, {kEpsilonSymbol}, EmptyFunction);
  // TODO
  builder.InsertRule(kForHeadRight, {kSemicolon, kExpr, kSemicolon, kExpr}, EmptyFunction);

  // Expression
  builder.InsertRule(kExprListLess, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kExprListLess, {kExprList}, EmptyFunction);
  builder.InsertRule(kExpr, {kLiteral}, EmptyFunction);
  builder.InsertRule(kExprList, {kExpr, kExprListRecur}, EmptyFunction);
  builder.InsertRule(kExprListRecur, {kEpsilonSymbol}, EmptyFunction);
  builder.InsertRule(kExprListRecur,
                     {kComma, kExpr, kExprListRecur},
                     EmptyFunction);

  return builder.Build();
}

std::shared_ptr<GolikeGrammarData> CreateGolikeGrammarData() {
  return std::make_shared<GolikeGrammarData>();
}

} // end of namespace golike_grammar
