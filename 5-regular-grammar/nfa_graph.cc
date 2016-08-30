/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <iostream>
#include <queue>
#include "simplelogger.h"
#include "nfa_graph.h"

using std::vector;
using std::string;
using std::set;
using std::pair;

extern simple_logger::BaseLogger logger;

namespace nfa_graph {

/*----------------------------------------------------------------------------*/

/**
 * class Edge
 */
Edge *Edge::createEdgeFromString(const string &s) {
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
Edge *Node::Iterator::NextEdge() {
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
  return new NFA(new Node(Node::kStart), e, new Node(Node::kEnd));
}


NFA *NFA::CreateSimpleNFAFromString(const string &s) {
  return CreateSimpleNFAFromEdge(Edge::createEdgeFromString(s));
}


Node *NFA::SetNewStart(Node *start) {
  assert(Node::kStart == start->type());
  Node *old_start = _start;
  old_start->ChangeNormal();
  _start = start;
  _nodes.insert(_start);
  return old_start;
}


Node *NFA::SetNewEnd(Node *end) {
  assert(Node::kEnd == end->type());
  Node *old_end = _end;
  old_end->ChangeNormal();
  _end = end;
  _nodes.insert(_end);
  return old_end;
}


Node *NFA::RemoveStart() {
  Node *old_start = _start;
  _start = nullptr;
  _nodes.erase(old_start);
  return old_start;
}


Node *NFA::RemoveEnd() {
  Node *old_end = _end;
  _end = nullptr;
  _nodes.erase(old_end);
  return old_end;
}


const char *
NFA::DFS(Node *curr, const char *beg, const char *end, bool is_match) {
  if (Node::kEnd == curr->type()) {
    return beg;
  }

  logger.log("node type [{}]", static_cast<int>(curr->type()));

  auto iter = curr->AvailableEdges(*beg);
  Edge *edge = nullptr;
  while (nullptr != (edge = iter.NextEdge())) {
    if (beg == end && 0 != edge->Delta()) {
      continue;
    }

    const char *result = DFS(edge->NextNode(), beg + edge->Delta(), end,
                             is_match);

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
  Node *rhs_start = rhs->RemoveStart();
  lhs->end()->FetchEdges(rhs_start);
  delete rhs_start;

  lhs->SetNewEnd(rhs->RemoveEnd());

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFA *DoLogicalOr(NFA *lhs, NFA *rhs) {
  Node *rhs_start = rhs->RemoveStart();
  lhs->start()->FetchEdges(rhs_start);
  delete rhs_start;

  auto *new_end = new Node(Node::kEnd);
  lhs->end()->AddEdge(new EpsilonEdge, new_end);
  rhs->end()->AddEdge(new EpsilonEdge, new_end);
  lhs->SetNewEnd(new_end);
  rhs->end()->ChangeNormal();

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFA *KleenStar(NFA *nfa) {
  Node *old_start = nfa->SetNewStart(new Node(Node::kStart));
  Node *old_end = nfa->SetNewEnd(new Node(Node::kEnd));

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
  Node *old_start = nfa->SetNewStart(new Node(Node::kStart));
  Node *old_end = nfa->SetNewEnd(new Node(Node::kEnd));

  nfa->start()->AddEdge(new EpsilonEdge, old_start);
  old_end->AddEdge(new EpsilonEdge, nfa->end());

  old_end->AddEdge(new EpsilonEdge, old_start);
  return nfa;
}


void RecordNFARecur(vector<Node*> &nodes, Node *u) {
  // the number of the first u must be 0
  u->set_number(nodes.size());
  nodes.push_back(u);

  for (Edge *edge : u->edges()) {
    Node *v = edge->NextNode();

    if (Node::kUnsetNumber == v->number()) {
      RecordNFARecur(nodes, v);
    }
  }
}

void PrintNFARecur(Node *u, vector<bool> &visit) {
  visit[u->number()] = true;

  for (Edge *edge : u->edges()) {
    Node *v = edge->NextNode();

    logger.debug("{}{}{}", u->number(), edge->ToString(), v->number());

    if (!visit[v->number()]) {
      PrintNFARecur(v, visit);
    }
  }
}


size_t DFAConverter::SetHash::operator() (const std::set<int> &s) const {
  size_t value = 0;
  for (int num : s) {
    value |= num % sizeof(size_t);
  }
  return value;
}

void DFAConverter::EpsilonClosure(set<int> &s, Node *u) {
  s.insert(u->number());
  for (Edge *edge : u->edges()) {
    // filter epsilon edge
    if (0 != edge->Delta()) continue;

    Node *v = edge->NextNode();
    if (s.end() == s.find(v->number())) {
      EpsilonClosure(s, v);
    }
  }
}

set<int> DFAConverter::EpsilonClosure(Node *u) {
  set<int> s;
  EpsilonClosure(s, u);
  return s;
}

void DFAConverter::Expand(set<int> &s, Node *dfa_u) {
  for (int node_number : s) {
    for (Edge *edge : _nodes[node_number]->edges()) {
      if (0 == edge->Delta()) continue;

      set<int> adjacent_set = EpsilonClosure(edge->NextNode());
      auto result = _set_visit.find(adjacent_set);

      Node *dfa_v = nullptr;
      if (_set_visit.end() == result) {
        dfa_v = new Node(Node::kNormal);
        _set_visit.insert({adjacent_set, dfa_v});
        //

      } else {
        dfa_v = result->second;
      }
      dfa_u->AddEdge(edge->Clone(), dfa_v);
    }
  }
}

DFA* DFAConverter::Convert() {
  _nodes = RecordNFA(_nfa);
  _dfa = new DFA;

  set<int> start_set = EpsilonClosure(_nfa->start());
  _set_visit.insert({start_set, new Node(Node::kStart)});

  std::queue<set<int>> q;

  while (!q.empty()) {
    // set<int> &curr_set = *q.front(); q.pop();
    // Expand(curr_set);
  }

  return nullptr;
}


DFA* DFAConverter::ConvertNFAToDFA(NFA *nfa) {
  return DFAConverter(nfa).Convert();
}


} // end of namespace nfa_graph
