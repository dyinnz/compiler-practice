// // Created by coder on 16-9-5.
//

#include "tokenizer.h"
#include "simplelogger.h"

using std::vector;
using std::string;
using std::move;
using std::pair;

extern simple_logger::BaseLogger logger;

void Tokenizer::BuildTokenizer(const vector<pair<string, Symbol>> &pattern) {
  RegexParser re_parser;
  NFAComponent *result_comp = nullptr;

  ResetPriority();

  for (auto &p : pattern) {
    const string &s = p.first;
    auto symbol = p.second;

    NFAComponent *comp = re_parser.ParseToNFAComponent(s);
    if (!comp) {
      logger.error("{}(): nullptr NFAComponent pointer", __func__);
      return;
    }

    int priority = NextPriority();
    priority_to_symbol.push_back(symbol);
    comp->end()->set_priority(priority);

    if (result_comp) {
      result_comp = re_parser.GetNFAManager().UnionWithMultiEnd(result_comp,
                                                                comp);
    } else {
      result_comp = comp;
    }
  }

  auto token_nfa = re_parser.GetNFAManager().BuildNFA(result_comp);

  auto normal_dfa = ConvertNFAToDFA(token_nfa);

  token_dfa_ = normal_dfa;

  logger.log("finish build");
}

const char *Tokenizer::SkipSpace(const char *p) {
  while (p != end_ && isspace(*p)) {
    if ('\n' == *p) {
      curr_row_ += 1;
      curr_row_pos_ = p + 1;
    }
    p += 1;
  }
  return p;
}

Token Tokenizer::GetNextToken(const char *&p) {
  assert(p < end_);

  Token longest_token = kErrorToken;

  const DFANode *curr_node = token_dfa_->start();

  const char *s = p;
  while (s != end_) {
    const DFANode *next_node = curr_node->GetNextNode(*s);

    if (next_node) {
      curr_node = next_node;
      s += 1;
      // logger.debug("{}", to_string(*curr_node));

      if (curr_node->IsEnd()) {
        int priority = curr_node->priority();
        longest_token.symbol = priority_to_symbol[priority];
      }

    } else {
      break;
    }
  }

  longest_token.str = std::string(p, s);
  longest_token.row = curr_row_;
  longest_token.column = p - curr_row_pos_;
  p = s;

  logger.debug("{}(): {}", __func__, to_string(longest_token));
  return longest_token;
}

bool Tokenizer::LexicalAnalyze(const string &s, vector<Token> &tokens) {
  return LexicalAnalyze(s.c_str(), s.c_str() + s.length(), tokens);
}

bool Tokenizer::LexicalAnalyze(const char *beg,
                               const char *end,
                               vector<Token> &tokens) {
  beg_ = beg;
  end_ = end;

  curr_row_ = 0;
  curr_row_pos_ = beg_;

  curr_ = beg;
  while (true) {
    curr_ = SkipSpace(curr_);
    if (curr_ >= end_) break;

    Token token = GetNextToken(curr_);
    if (token == kErrorToken) {
      return false;
    } else {
      tokens.push_back(move(token));
    }
  }

  return true;
}
