/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <iostream>
#include "simplelogger.h"
#include "nfa_graph.h"

extern simple_logger::BaseLogger logger;

namespace nfa_graph {

/*----------------------------------------------------------------------------*/

/**
 * class Edge
 */
Edge *Edge::createEdgeFromString(const std::string &s) {
  if (s.empty()) {
    return new EpsilonEdge;
  } else if (1 == s.length()) {
    return new CharEdge(s[0]);
  }
  return nullptr;
}

/*----------------------------------------------------------------------------*/

/**
 *  class NFANode
 */
Edge *NFANode::Iterator::NextEdge() {
  while (_curr != _end) {
    Edge *edge = *_curr++;
    if (edge->Match(_c)) {
      return edge;
    }
  }
  return nullptr;
}

/*----------------------------------------------------------------------------*/

/**
 * class NFA
 */
NFA *NFA::CreateSimpleNFAFromEdge(Edge *e) {
  return new NFA(new NFANode(NFANode::kStart), e, new NFANode(NFANode::kEnd));
}


NFA *NFA::CreateSimpleNFAFromString(const std::string &s) {
  return CreateSimpleNFAFromEdge(Edge::createEdgeFromString(s));
}


NFANode *NFA::ReplaceStart(NFANode *start) {
  assert(NFANode::kStart == start->type());
  NFANode *old_start = _start;
  old_start->ChangeNormal();
  _start = start;
  _nodes.insert(_start);
  return old_start;
}


NFANode *NFA::ReplaceEnd(NFANode *end) {
  assert(NFANode::kEnd == end->type());
  NFANode *old_end = _end;
  old_end->ChangeNormal();
  _end = end;
  _nodes.insert(_end);
  return old_end;
}


NFANode *NFA::RemoveStart() {
  NFANode *old_start = _start;
  _start = nullptr;
  _nodes.erase(old_start);
  return old_start;
}


NFANode *NFA::RemoveEnd() {
  NFANode *old_end = _end;
  _end = nullptr;
  _nodes.erase(old_end);
  return old_end;
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
  // TODO
  return nullptr;
}

/*----------------------------------------------------------------------------*/

/**
 * Auxiliary functions
 */
NFA *DoConcatenate(NFA *lhs, NFA *rhs) {
  // merge the end of lhs and the start of rhs
  NFANode *rhs_start = rhs->RemoveStart();
  lhs->end()->FetchEdges(rhs_start);
  delete rhs_start;

  lhs->ReplaceEnd(rhs->RemoveEnd());

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFA *DoLogicalOr(NFA *lhs, NFA *rhs) {
  NFANode *rhs_start = rhs->RemoveStart();
  lhs->start()->FetchEdges(rhs_start);
  delete rhs_start;

  NFANode *new_end = new NFANode(NFANode::kEnd);
  lhs->end()->AddEdge(new EpsilonEdge, new_end);
  rhs->end()->AddEdge(new EpsilonEdge, new_end);
  lhs->ReplaceEnd(new_end);
  rhs->end()->ChangeNormal();

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFA *KleenStar(NFA *nfa) {
  NFANode *old_start = nfa->ReplaceStart(new NFANode(NFANode::kStart));
  NFANode *old_end = nfa->ReplaceEnd(new NFANode(NFANode::kEnd));

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
  NFANode *old_start = nfa->ReplaceStart(new NFANode(NFANode::kStart));
  NFANode *old_end = nfa->ReplaceEnd(new NFANode(NFANode::kEnd));

  nfa->start()->AddEdge(new EpsilonEdge, old_start);
  old_end->AddEdge(new EpsilonEdge, nfa->end());

  old_end->AddEdge(new EpsilonEdge, old_start);
  return nfa;
}


} // end of namespace nfa_graph
