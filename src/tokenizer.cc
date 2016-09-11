// // Created by coder on 16-9-5.
//

#include "tokenizer.h"
#include "simplelogger.h"

using std::vector;
using std::string;
using std::move;

extern simple_logger::BaseLogger logger;

void Tokenizer::BuildTokenizer(const std::vector<std::string> &rules) {
  RegexParser re_parser;
  NFAComponent *result_comp = nullptr;
  ResetLabel();

  for (auto &s : rules) {
    NFAComponent *comp = re_parser.ParseToNFAComponent(s);
    if (!comp) {
      logger.error("{}(): nullptr NFAComponent pointer", __func__);
      return;
    }

    comp->end()->set_type(NextLabel());

    if (result_comp) {
      result_comp = re_parser.GetNFAManager().UnionWithMultiEnd(result_comp,
                                                                comp);
    } else {
      result_comp = comp;
    }
  }

  auto token_nfa = re_parser.GetNFAManager().BuildNFA(result_comp);
  PrintNFA(token_nfa);

  auto normal_dfa = ConvertNFAToDFA(token_nfa);
  PrintDFA(normal_dfa.get());

  token_dfa_ = normal_dfa;

  logger.log("finish build");
}

const char *Tokenizer::SkipSpace(const char *p) {
  while (p != end_ && isspace(*p)) {
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
      logger.debug("{}", to_string(*curr_node));

      if (curr_node->IsEnd()) {
        longest_token.type = curr_node->type();
      }

    } else {
      break;
    }
  }

  longest_token.str= std::string(p, s);
  p = s;

  logger.debug("{}:{}", longest_token.type, longest_token.str);
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

  tokens.push_back(kEndToken);
  return true;
}
