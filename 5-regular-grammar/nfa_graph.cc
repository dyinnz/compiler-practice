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
using std::cout;
using std::endl;

extern simple_logger::BaseLogger logger;

namespace nfa_graph {

/*----------------------------------------------------------------------------*/

/**
 * class Edge
 */
Edge *Edge::CreateFromChar(char c) {
  auto edge = new Edge;
  edge->set(c);
  return edge;
}


Edge *Edge::CreateFromString(const std::string &s) {
  if (s.empty()) {
    return CreateEpsilon();

  } else if (1 == s.length()) {
    return CreateFromChar(s[0]);

  } else {
    return nullptr;
  }
}


Edge *Edge::Merge(Edge *lhs, Edge *rhs) {
  auto *merged = new Edge;
  merged->_chars = lhs->_chars | rhs->_chars;
  delete lhs;
  delete rhs;
  return merged;
}


std::string to_string(Edge *edge) {
  string s{"--"};
  for (char c = 0; c < CHAR_MAX; ++c) {
    if (edge->test(c)) s += c;
  }
  s += "--";
  return s;
}

/*----------------------------------------------------------------------------*/
/**
 * Node
 */

void Node::SetType(int type) {
  if (kNormal == _type || kNormal == type) {
    _type = type;
  } else {
    _type |= type;
  }
}

/*----------------------------------------------------------------------------*/

/**
 * class NFA
 */
NFA *NFA::CreateFromEdge(Edge *e) {
  return new NFA(new Node(Node::kStart), e, new Node(Node::kEnd));
}


NFA *NFA::CreateFromString(const string &s) {
  return CreateFromEdge(Edge::CreateFromString(s));
}


Node *NFA::SetNewStart(Node *start) {
  assert(start->IsStart());
  Node *old_start = _start;
  old_start->SetType(Node::kNormal);
  _start = start;
  _nodes.insert(_start);
  return old_start;
}


Node *NFA::SetNewEnd(Node *end) {
  assert(end->IsEnd());
  Node *old_end = _end;
  old_end->SetType(Node::kNormal);
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
NFA::MatchDFS(Node *curr, const char *beg, const char *end) {
  logger.log("node [{}], type [{}]", curr->number(), curr->Type());

  if (curr->IsEnd()) {
    // recurse base, arrive END state
    return beg;
  }

  for (Edge *edge : curr->edges()) {
    const char *match_pos = nullptr;

    if (edge->IsEpsilon()) {
      // epsilon move
      match_pos = MatchDFS(edge->NextNode(), beg, end);

    } else if (edge->test(*beg) && beg < end) {
      // match
      match_pos = MatchDFS(edge->NextNode(), beg + 1, end);

    } else {
      // skip
      continue;
    }

    if (match_pos == end) {
      // get a match
      return match_pos;
    }
  }

  return nullptr;
}


const char *NFA::Match(const char *beg, const char *end) {
  return (MatchDFS(_start, beg, end) == end) ? beg : nullptr;
}


const char *SearchDFS(Node *curr, const char *beg, const char *end) {
  // TODO
  return nullptr;
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
  lhs->end()->AddEdge(Edge::CreateEpsilon(), new_end);
  rhs->end()->AddEdge(Edge::CreateEpsilon(), new_end);
  lhs->SetNewEnd(new_end);
  rhs->end()->SetType(Node::kNormal);

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFA *KleenStar(NFA *nfa) {
  Node *old_start = nfa->SetNewStart(new Node(Node::kStart));
  Node *old_end = nfa->SetNewEnd(new Node(Node::kEnd));

  old_end->AddEdge(Edge::CreateEpsilon(), old_start);

  nfa->start()->AddEdge(Edge::CreateEpsilon(), old_start);
  old_start->AddEdge(Edge::CreateEpsilon(), nfa->end());
  return nfa;
}


NFA *Optional(NFA *nfa) {
  nfa->start()->AddEdge(Edge::CreateEpsilon(), nfa->end());
  return nfa;
}


NFA *LeastOne(NFA *nfa) {
  Node *old_start = nfa->SetNewStart(new Node(Node::kStart));
  Node *old_end = nfa->SetNewEnd(new Node(Node::kEnd));

  nfa->start()->AddEdge(Edge::CreateEpsilon(), old_start);
  old_end->AddEdge(Edge::CreateEpsilon(), nfa->end());

  old_end->AddEdge(Edge::CreateEpsilon(), old_start);
  return nfa;
}

/*----------------------------------------------------------------------------*/
/**
 * preamble of conversion
 */

void RecordNFARecur(vector<Node *> &nodes, Node *u) {
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


void PrintFARecur(Node *u, vector<bool> &visit) {
  visit[u->number()] = true;

  for (Edge *edge : u->edges()) {
    Node *v = edge->NextNode();

    logger.debug("[{}:{}]{}[{}:{}]", u->number(), u->TypeString(),
                 to_string(edge), v->number(), v->TypeString());

    if (!visit[v->number()]) {
      PrintFARecur(v, visit);
    }
  }
}


/*----------------------------------------------------------------------------*/

string to_string(const set<int> &num_set) {
  string str{"{"};
  for (int num : num_set) {
    str += std::to_string(num);
  }
  str += '}';
  return str;
}

size_t DFAConverter::SetHash::operator()(const std::set<int> &s) const {
  size_t value = 0;
  for (int num : s) {
    value |= num % sizeof(size_t);
  }
  return value;
}


pair<Node *, bool> &DFAConverter::FindNodeBySet(const std::set<int> &s) {
  auto iter = _set_to_dfa_node.find(s);
  if (_set_to_dfa_node.end() == iter) {
    iter = _set_to_dfa_node.insert({s, {new Node(Node::kNormal), false}}).first;
  }
  return iter->second;
}


void DFAConverter::ConversionPreamble() {
  _nfa_nodes = RecordNFA(_nfa);
  _e_closures.resize(_nfa_nodes.size());
  _set_to_dfa_node.reserve(_nfa_nodes.size());
}


void DFAConverter::EpsilonClosureRecur(set<int> &s, Node *u) {
  s.insert(u->number());
  // cout << u->number() << " u's edges: " << u->edges().size() << endl;

  for (Edge *edge : u->edges()) {
    // cout << "epsilon? " << edge->IsEpsilon() << endl;
    if (edge->IsEpsilon()) {
      // filter epsilon edge
      Node *v = edge->NextNode();

      if (s.end() == s.find(v->number())) {
        // logger.debug("{}(): closure: new number {}", __func__, v->number());
        s.insert(v->number());
        EpsilonClosureRecur(s, v);
      }
    }
  }
}


const set<int> &DFAConverter::EpsilonClosure(Node *u) {
  set<int> &s = _e_closures[u->number()];
  if (s.empty()) {
    EpsilonClosureRecur(s, u);
  }
  return s;
}


Edge::EdgeChars DFAConverter::GetSetEdgeChars(const set<int> &s) {
  Edge::EdgeChars chars;
  for (int num : s) {
    for (Edge *edge : _nfa_nodes[num]->edges()) {
      chars |= edge->chars();
    }
  }
  return chars;
}


std::set<int>
DFAConverter::GetAdjacentSet(const std::set<int> &curr_set, char c) {
  set<int> adjacent_set;
  for (int num : curr_set) {
    for (Edge *edge : _nfa_nodes[num]->edges()) {
      const set<int> &e_closure = EpsilonClosure(edge->NextNode());
      adjacent_set.insert(e_closure.begin(), e_closure.end());
    }
  }
  return adjacent_set;
}


void DFAConverter::ConstructDFADiagram() {
  auto start_dfa_node = new Node(Node::kStart);
  _dfa->_start = start_dfa_node;

  set<int> start_set = EpsilonClosure(_nfa->start());
  _set_to_dfa_node.insert({start_set, {start_dfa_node, false}});

  std::queue<set<int>> q;
  q.push(start_set);

  while (!q.empty()) {
    set<int> &curr_set = q.front();

    logger.log("current set {}", to_string(curr_set));

    pair<Node *, bool> &curr_node_pair = FindNodeBySet(curr_set);
    curr_node_pair.second = true;

    Edge::EdgeChars chars = GetSetEdgeChars(curr_set);

    for (char c = 0; c < CHAR_MAX; ++c) {
      if (chars.test(c)) {

        set<int> adjacent_set = GetAdjacentSet(curr_set, c);
        pair<Node *, bool> &node_pair = FindNodeBySet(adjacent_set);

        logger.log("adjacent set {}", to_string(adjacent_set));

        if (!node_pair.second) {
          // unhandled dfa node, push to queue
          q.push(adjacent_set);
        }

        curr_node_pair.first->AddEdge(Edge::CreateFromChar(c),
                                      node_pair.first);
      }
    }

    q.pop();
  }
}


void DFAConverter::CollectNodes() {
  for (auto p : _set_to_dfa_node) {
    const set<int> &s = p.first;
    Node *dfa_node = p.second.first;
    dfa_node->set_number(_dfa->_nodes.size());
    _dfa->_nodes.push_back(dfa_node);

    for (int num : s) {
      if (_nfa_nodes[num]->IsEnd()) {
        if (!dfa_node->IsEnd()) {
          _dfa->_ends.push_back(dfa_node);
        }
        dfa_node->SetType(Node::kEnd);
      }
    }
  }
}


DFA *DFAConverter::Convert() {
  _dfa = new DFA;

  ConversionPreamble();

  ConstructDFADiagram();

  CollectNodes();

  return _dfa;
}


DFA *DFAConverter::ConvertNFAToDFA(NFA *nfa) {
  return DFAConverter(nfa).Convert();
}


DFA *DFA::ConvertFromNFA(NFA *nfa) {
  return DFAConverter::ConvertNFAToDFA(nfa);
}

} // end of namespace nfa_graph
