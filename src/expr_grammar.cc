//
// Created by coder on 16-9-15.
//

#include "expr_grammar.h"
#include "simplelogger.h"

using std::string;
using std::move;

extern simple_logger::BaseLogger logger;

namespace expr_grammar {

NON_TERMINAL(kExpr)
NON_TERMINAL(kExprRecur)
NON_TERMINAL(kTerm)
NON_TERMINAL(kTermRecur)
NON_TERMINAL(kFactor)

TERMINAL(kAdd)
TERMINAL(kSub)
TERMINAL(kMul)
TERMINAL(kDiv)
TERMINAL(kLeftParen)
TERMINAL(kRightParen)
TERMINAL(kNumber)
TERMINAL(kName)

string to_string(const Symbol &symbol) {
  switch (symbol.ID()) {
    case kExprID:return "Expr";

    case kExprRecurID:return "ExprRecur";

    case kTermID:return "Term";

    case kTermRecurID:return "TermRecur";

    case kFactorID:return "Factor";

    case kNumberID:return "Number";

    case kNameID:return "Name";

    case kEpsilonID:return "Epsilon";

    case kEofID:return "EOF";

    case kStartID:return "Start";

    default:return string(1, static_cast<char>(symbol.ID()));
  }
}

Tokenizer BuildExprTokenizer() {
  TokenizerBuilder tokenizer_builder;
  tokenizer_builder.SetPatterns(
      {{R"(\()", kLeftParen},
       {R"(\))", kRightParen},
       {R"(\+)", kAdd},
       {R"(-)", kSub},
       {R"(/)", kDiv},
       {R"(\*)", kMul},
       {R"(\d+)", kNumber},
       {R"([A-Za-z_]\w*)", kName},
       {"[ \r\v\t\f]", kSpaceSymbol},
      });

  return tokenizer_builder.Build();
}

std::shared_ptr<ExprGrammarData> CreateGrammarData() {
  return std::make_shared<ExprGrammarData>();
}

void TokenFeeder(void *grammar_data, Token &&token) {
  ExprGrammarData *expr_data = static_cast<ExprGrammarData *>(grammar_data);

  auto ast_node = expr_data->ast()->CreateNode(move(token));
  expr_data->node_record().push_back(ast_node);

  logger.debug("{}() node record size {} feed {}",
               __func__,
               expr_data->node_record().size(),
               to_string(token));
}

void Operand_Recur(void *grammar_data) {
  ExprGrammarData *expr_data = static_cast<ExprGrammarData *>(grammar_data);
  logger.debug("{}() node record size {}",
               __func__,
               expr_data->node_record().size());

  auto &node_record = expr_data->node_record();

  auto recur_node = node_record.back();
  node_record.pop_back();

  auto operand_node = node_record.back();
  if (recur_node->symbol() != kEpsilonSymbol) {
    recur_node->push_child_front(operand_node);
    node_record.back() = recur_node;
  }
}

void Recur_Op_Recur(void *grammar_data) {
  ExprGrammarData *expr_data = static_cast<ExprGrammarData *>(grammar_data);
  logger.debug("{}() node record size {}",
               __func__,
               expr_data->node_record().size());

  auto &node_record = expr_data->node_record();

  auto recur_node = node_record.back();
  node_record.pop_back();
  auto operand_node = node_record.back();
  node_record.pop_back();

  auto &operator_node = node_record.back();
  if (recur_node->symbol() == kEpsilonSymbol) {
    operator_node->push_child_back(operand_node);

  } else {
    recur_node->push_child_front(operand_node);
    operator_node->push_child_back(recur_node);
  }
}

void Operand_Epsilon(void *grammar_data) {
  ExprGrammarData *expr_data = static_cast<ExprGrammarData *>(grammar_data);
  logger.debug("{}() node record size {}",
               __func__,
               expr_data->node_record().size());

  auto epsilon_node = expr_data->ast()->CreateNode(kEpsilonSymbol);
  expr_data->node_record().push_back(epsilon_node);
}

void Factor_Left_Expr_Right(void *grammar_data) {
  ExprGrammarData *expr_data = static_cast<ExprGrammarData *>(grammar_data);
  logger.debug("{}() node record size {}",
               __func__,
               expr_data->node_record().size());

  auto &node_record = expr_data->node_record();

  assert(node_record.back()->symbol() == kRightParen);
  node_record.pop_back();

  auto *expr_node = node_record.back();
  node_record.pop_back();

  // assert(node_stack.back()->symbol() == kLeftParen);
  // node_stack.back() = expr_node;

  // TODO
  while (node_record.back()->symbol() != kLeftParen) {
    node_record.pop_back();
  }
  node_record.back() = expr_node;
}

void EmptyFunction(void *grammar_data) {
  ExprGrammarData *expr_data = static_cast<ExprGrammarData *>(grammar_data);
  // do nothing
  logger.debug("{}() token of top node: {}",
               __func__,
               expr_data->node_record().back()->str());
}

Grammar BuildExprGrammar() {

  GrammarBuilder builder;

  builder.SetSymbolTable({kExpr, kExprRecur, kTerm, kTermRecur, kFactor,
                          kAdd, kSub, kMul, kDiv, kLeftParen, kRightParen,
                          kNumber, kName,
                          kEpsilonSymbol, kEofSymbol, kStartSymbol});

  builder.SetTokenFeeder(TokenFeeder);

  builder.InsertRule(kStartSymbol, {kExpr}, EmptyFunction); // 0

  builder.InsertRule(kExpr, {kTerm, kExprRecur}, Operand_Recur); // 1

  builder.InsertRule(kExprRecur,
                     {kAdd, kTerm, kExprRecur},
                     Recur_Op_Recur); // 2
  builder.InsertRule(kExprRecur,
                     {kSub, kTerm, kExprRecur},
                     Recur_Op_Recur); // 3
  builder.InsertRule(kExprRecur, {kEpsilonSymbol}, Operand_Epsilon); // 4

  builder.InsertRule(kTerm, {kFactor, kTermRecur}, Operand_Recur); // 5

  builder.InsertRule(kTermRecur,
                     {kMul, kFactor, kTermRecur},
                     Recur_Op_Recur); // 6
  builder.InsertRule(kTermRecur,
                     {kDiv, kFactor, kTermRecur},
                     Recur_Op_Recur); // 7
  builder.InsertRule(kTermRecur, {kEpsilonSymbol}, Operand_Epsilon); // 8

  builder.InsertRule(kFactor,
                     {kLeftParen, kExpr, kRightParen},
                     Factor_Left_Expr_Right); // 9
  builder.InsertRule(kFactor, {kNumber}, EmptyFunction); // 10
  builder.InsertRule(kFactor, {kName}, EmptyFunction); // 11

  return builder.Build();
}

} // end of namespace expr_grammar
