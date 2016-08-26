//
// Created by coder on 16-8-25.
//

#include <iostream>
#include "simplelogger.h"
#include "nfa_graph.h"

extern simple_logger::BaseLogger logger;

namespace nfa_graph {

Edge *Edge::createEdgeFromString(const std::string &s) {
  if (s.empty()) {
    return new EpsilonEdge;
  } else if (1 == s.length()) {
    return new CharEdge(s[0]);
  }
  return nullptr;
}


Edge *NFANode::Iterator::NextEdge() {
  while (_curr != _end) {
    Edge *edge = *_curr++;
    if (edge->Match(_c)) {
      return edge;
    }
  }
  return nullptr;
}


NFA *NFA::CreateSimpleNFAFromEdge(Edge *e) {
  return new NFA(new NFANode(NFANode::kStart), e, new NFANode(NFANode::kEnd));
}


NFA *NFA::CreateSimpleNFAFromString(const std::string &s) {
  return CreateSimpleNFAFromEdge(Edge::createEdgeFromString(s));
}


const char *NFA::DFS(NFANode *curr, const char *beg, const char *end, bool is_match) {
  if (NFANode::kEnd == curr->type()) {
    return beg;
  }

  logger.log("node type [{}]", static_cast<int>(curr->type()));

  auto iter = curr->AvailableEdges(*beg);
  Edge *edge = nullptr;
  while (nullptr != (edge = iter.NextEdge())) {
    if (beg == end && 0 != edge->Delta()) {
      continue;
    }

    const char *result = DFS(edge->NextNode(), beg + edge->Delta(), end, is_match);

    if ((is_match && result == end) || (!is_match && result != nullptr)) {
      return result;
    }
  }
  return nullptr;
}


const char *NFA::Match(const char *beg, const char *end) {
  const char *result = DFS(_start, beg, end, true);
  if (result != end) {
    return nullptr;
  } else {
    return beg;
  }
}


const char *NFA::Search(const char *begin, const char *end) {
  return nullptr;
}


NFA *KleenStar(NFA *nfa) {
  NFANode *old_start = nfa->start();
  NFANode *old_end = nfa->end();

  old_start->ChangeNormal();
  old_end->ChangeNormal();

  nfa->set_start(new NFANode(NFANode::kStart));
  nfa->set_end(new NFANode(NFANode::kEnd));

  old_end->AddEdge(new EpsilonEdge, old_start);

  nfa->start()->AddEdge(new EpsilonEdge, old_start);
  old_start->AddEdge(new EpsilonEdge, nfa->end());
  return nfa;
}

NFA *Optional(NFA *nfa) {
  nfa->start()->AddEdge(new EpsilonEdge, nfa->end());
  return nfa;
}

NFA *LeastOne(NFA *nfa) {
  NFANode *old_start = nfa->start();
  NFANode *old_end = nfa->end();

  old_start->ChangeNormal();
  old_end->ChangeNormal();

  nfa->set_start(new NFANode(NFANode::kStart));
  nfa->set_end(new NFANode(NFANode::kEnd));

  nfa->start()->AddEdge(new EpsilonEdge, old_start);
  old_end->AddEdge(new EpsilonEdge, nfa->end());

  old_end->AddEdge(new EpsilonEdge, old_start);
  return nfa;
}

}
