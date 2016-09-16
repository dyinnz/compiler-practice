//
// Created by coder on 16-9-15.
//

#include "expr_grammar.h"
#include "tokenizer.h"

using std::string;

namespace expr_grammar {

Symbol kExpr{false, kExprID};
Symbol kExprRecur{false, kExprRecurID};
Symbol kTerm{false, kTermID};
Symbol kTermRecur{false, kTermRecurID};
Symbol kFactor{false, kFactorID};

Symbol kAdd{true, '+'};
Symbol kSub{true, '-'};
Symbol kMul{true, '*'};
Symbol kDiv{true, '/'};
Symbol kLeftParen{true, '('};
Symbol kRightParen{true, ')'};
Symbol kNumber{true, kNumberID};
Symbol kName{true, kNameID};

string to_string(const Symbol &symbol) {
  switch (symbol.SymbolID()) {
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

    default:return string(1, static_cast<char>(symbol.SymbolID()));
  }
}

static bool IsTerminal(int id) {
  switch (id) {
    case kExprID:
    case kExprRecurID:
    case kTermID:
    case kTermRecurID:
    case kFactorID:
    case kStartID:
      return false;
    default:
      return true;
  }
}

Tokenizer BuildExprTokenizer() {
  Tokenizer tokenizer;

  tokenizer.BuildTokenizer(
      {{"\\(", '('},
       {"\\)", ')'},
       {"\\+", '+'},
       {"-", '-'},
       {"\\*", '*'},
       {"/", '/'},
       {"\\w+", kNameID},
       {"\\d+", kNumberID}
      });

  return tokenizer;
}

Grammar BuildExprGrammar() {

  GrammarBuilder builder;

  builder.SetSymbolTable({kExpr, kExprRecur, kTerm, kTermRecur, kFactor,
                          kAdd, kSub, kMul, kDiv, kLeftParen, kRightParen,
                          kNumber, kName,
                          kEpsilon, kEof, kStart});

  builder.SetIsTerminal(IsTerminal);

  builder.InsertRule(kStart, {kExpr});

  builder.InsertRule(kExpr, {kTerm, kExprRecur});

  builder.InsertRule(kExprRecur, {kAdd, kTerm, kExprRecur});
  builder.InsertRule(kExprRecur, {kSub, kTerm, kExprRecur});
  builder.InsertRule(kExprRecur, {kEpsilon});

  builder.InsertRule(kTerm, {kFactor, kTermRecur});

  builder.InsertRule(kTermRecur, {kMul, kFactor, kTermRecur});
  builder.InsertRule(kTermRecur, {kDiv, kFactor, kTermRecur});
  builder.InsertRule(kTermRecur, {kEpsilon});

  builder.InsertRule(kFactor, {kLeftParen, kExpr, kRightParen});
  builder.InsertRule(kFactor, {kNumber});
  builder.InsertRule(kFactor, {kName});

  return builder.Build();
}

} // end of namespace expr_grammar
