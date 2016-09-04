//
// Created by coder on 16-9-3.
//

#include "simplelogger.h"
#include "re_parser.h"

using std::string;

extern simple_logger::BaseLogger logger;

namespace regular_expression {

/**
 * class REParser
 */
DFA *REParser::Parse(const char *beg, const char *end) {
  REParser parser(beg, end);

  NFA *nfa = parser.ParseUnion(beg)->BuildNFA();
  PrintNFA(nfa->start(), nfa->size());

  DFA *normal = DFA::ConvertFromNFA(nfa);
  PrintDFA(normal->start(), normal->size());

  DFA *minimum = DFAOptimizer::Minimize(normal);
  PrintDFA(minimum->start(), minimum->size());

  return minimum;
}


NFAComponent *REParser::ParseUnion(const char *&p) {
  if (p >= end_) return nullptr;

  NFAComponent *result{nullptr};

  while (true) {
    NFAComponent *current = ParseConcatenate(p);

    if (result && current) {
      result = Union(current, result);

    } else if (!result && current) {
      result = current;

    } else {
      delete result;
      return nullptr;
    }

    // check what the next char is
    if ('|' == *p) {
      p += 1;

    } else if (end_ == p || ')' == *p) {
      break;

    } else {
      logger.error("{}(): unexpected end.", __func__);
      delete result;
      return nullptr;
    }
  }

  return result;
}


NFAComponent *REParser::ParseConcatenate(const char *&p) {
  assert(p < end_);

  NFAComponent *result{nullptr};

  while (true) {
    NFAComponent *current = ParseBasic(p);

    if (result && current) {
      result = Concatenate(result, current);

    } else if (!result && current) {
      result = current;

    } else {
      return nullptr;
    }

    if (end_ == p || ')' == *p || '|' == *p) {
      break;
    }
  }

  return result;
}

NFAComponent *REParser::ParseBasic(const char *&p) {
  assert(p < end_);

  NFAComponent *result{nullptr};

  switch (*p) {
    case '(':
      result = ParseGroup(p);
      break;

    case '[':
      result = ParseSet(p);
      break;

    case '.': {
      auto edge = new NFAEdge;
      edge->SetAll();
      result = NFAComponent::CreateFromEdge(edge);
      p += 1;
    }
      break;

    case '\\':
      result = ParseEscape(p);
      break;

    default:
      // char
      result = NFAComponent::CreateFromChar(*p);
      p += 1;
      break;
  }

  switch (*p) {
    case '*':
      result = KleenStar(result);
      p += 1;
      break;

    case '+':
      result = LeastOne(result);
      p += 1;
      break;

    case '?':
      result = Optional(result);
      p += 1;

    default:
      break;
  }

  return result;
}


NFAComponent *REParser::ParseGroup(const char *&p) {
  assert(p < end_);
  assert('(' == *p);
  p += 1;

  NFAComponent *result = ParseUnion(p);

  if (')' != *p) {
    logger.debug("{}(): {}", __func__, p);
    logger.error("{}(): right parenthesis dismatch", __func__);
    delete result;
    return nullptr;
  }

  p += 1;
  return result;
}


NFAComponent *REParser::ParseString(const char *&p) {
  // TODO
  assert(false);
  return nullptr;
}


NFAComponent *REParser::ParseEscape(const char *&p) {
  assert(p < end_);
  assert(*p == '\\');
  p += 1;

  NFAComponent *result = 0;
  result = NFAComponent::CreateFromChar(*p);

  switch (*p) {
    case '\\':
    case '.':
    case '*':
    case '+':
    case '?':
    case '(':
    case ')':
    case '[':
    case ']':
    case '|':
      result = NFAComponent::CreateFromChar(*p);
      break;

    case 'd':
      result = NFAComponent::CreateFromRange('0', '9' + 1);
      break;

    case 'D': {
      auto edge = NFAEdge::CreateFromRange('0', '9' + 1);
      edge->flip();
      result = NFAComponent::CreateFromEdge(edge);
    }
      break;

    case 's':
      result = NFAComponent::CreateFromString(" \f\n\r\t\v");
      break;

    case 'S': {
      auto edge = NFAEdge::CreateFromString(" \f\n\r\t\v");
      edge->flip();
      result = NFAComponent::CreateFromEdge(edge);
    }
      break;

    case 'w': {
      auto edge = new NFAEdge;
      edge->SetRange('a', 'z' + 1);
      edge->SetRange('A', 'Z' + 1);
      edge->SetRange('0', '9' + 1);
      edge->set('_');
      result = NFAComponent::CreateFromEdge(edge);
    }
      break;

    case 'W': {
      auto edge = new NFAEdge;
      edge->SetRange('a', 'z' + 1);
      edge->SetRange('A', 'Z' + 1);
      edge->SetRange('0', '9' + 1);
      edge->set('_');
      edge->flip();
      result = NFAComponent::CreateFromEdge(edge);
    }
      break;

    default:
      logger.error("{}(): unrecognized escape char", __func__);
      delete result;
      return nullptr;
  }

  p += 1;
  return result;
}

NFAComponent *REParser::ParseSet(const char *&p) {
  assert(p < end_);
  assert('[' == *p);
  p += 1;

  bool reverse = '^' == *p;
  if (reverse) {
    p += 1;
  }

  NFAEdge *edge = new NFAEdge;
  for (; p < end_ && ']' != *p; ++p) {
    if ('-' != *p) {
      edge->set(*p);

    } else {
      if (p + 1 < end_ && ']' != *p && *(p - 1) < *(p + 1)) {
        edge->SetRange(*(p - 1), *(p + 1) + 1);

      } else {
        logger.error("{}(): wrong range", __func__);
        delete edge;
        return nullptr;
      }
    }
  }

  if (reverse) {
    edge->flip();
  }

  if (']' != *p) {
    delete edge;
    return nullptr;
  }
  p += 1;
  NFAComponent *result = NFAComponent::CreateFromEdge(edge);
  return result;
}


} // end of namespace regular_expression
