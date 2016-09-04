//
// Created by coder on 16-9-3.
//

#pragma once

#include "finite_automaton.h"

namespace regular_expression {

/**
 * Regular Expression Parser
 */

class REParser {
public:
  static DFA *Parse(const char *beg, const char *end);

private:
  REParser(const char *beg, const char *end) : beg_(beg), end_(end) {
  }

  NFAComponent *ParseUnion(const char *&p);

  NFAComponent *ParseConcatenate(const char *&p);

  NFAComponent *ParseBasic(const char *&p);

  NFAComponent *ParseGroup(const char *&p);

  NFAComponent *ParseSet(const char *&p);

  NFAComponent *ParseString(const char *&p);

  NFAComponent *ParseEscape(const char *&p);

private:
  const char *beg_{nullptr};
  const char *end_{nullptr};
};


} // end of namespace regular_expression

