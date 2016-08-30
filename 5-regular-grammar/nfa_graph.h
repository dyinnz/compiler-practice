/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <functional>
#include <climits>

#include "utility.h"

namespace nfa_graph {

// pre-declaration
class Node;

/**
 * Edge
 */
class Edge {
public:
  typedef std::bitset<CHAR_MAX + 1> EdgeChars;

  static Edge *CreateEpsilon() {
    return new Edge;
  }

  static Edge *CreateFromChar(char c);

  static Edge *CreateFromString(const std::string &s);


  static Edge *Merge(Edge *lhs, Edge *rhs);

  friend std::string to_string(Edge *edge);

public:
  Node *NextNode() const {
    return _v;
  }

  void SetNextNode(Node *v) {
    _v = v;
  }

  bool IsEpsilon() const {
    return _chars.none();
  }

  bool test(char c) const {
    return _chars.test(c);
  }

  EdgeChars chars() {
    return _chars;
  }


private:
  void set(char c) {
    _chars.set(c);
  }

private:
  EdgeChars _chars;
  Node *_v{nullptr};
};


/**
 *  Transition Diagram Node
 */
class Node {
public:
  constexpr static int kStart = 1;
  constexpr static int kEnd = 2;
  constexpr static int kStartEnd = 3;
  constexpr static int kNormal = 4;

  Node(int type) : _type(type) {}

  ~Node() {
    for (auto edge : _edges) {
      delete edge;
    }
  }

  bool IsStart() {
    return _type & 1;
  }

  bool IsEnd() {
    return _type & 2;
  }

  bool IsNormal() {
    return _type & 4;
  }

  int Type() {
    return _type;
  }

  std::string TypeString() {
    if (kStart == _type) {
      return "start";
    } else if (kEnd == _type) {
      return "end";
    } else if (kStartEnd == _type) {
      return "start/end";
    } else {
      return "normal";
    }
  }

  void SetType(int type);

  constexpr static int kUnsetNumber{-1};

  int number() const {
    return _number;
  }

  void set_number(int number) {
    _number = number;
  }

  std::list<Edge *> edges() {
    return _edges;
  }

  void AddEdge(Edge *edge, Node *node) {
    edge->SetNextNode(node);
    _edges.push_back(edge);
  }

  void FetchEdges(Node *other) {
    _edges.splice(_edges.end(), other->_edges);
  }

private:
  int _type;
  int _number{kUnsetNumber};
  std::list<Edge *> _edges;
};


/**
 * non-deterministic finite automaton
 */
class NFA {
public:
  static NFA *CreateFromEdge(Edge *e);

  static NFA *CreateFromString(const std::string &s);

public:
  NFA(Node *start, Edge *e, Node *end) : _start(start), _end(end) {
    _start->AddEdge(e, _end);
    _nodes.insert(start);
    _nodes.insert(end);
  }

  ~NFA() {
    for (auto node : _nodes) {
      delete node;
    }
  }

  const char *Match(const char *beg, const char *end);

  const char *Search(const char *begin, const char *end);

  Node *start() {
    return _start;
  }

  Node *end() {
    return _end;
  }

  Node *SetNewStart(Node *start);

  Node *SetNewEnd(Node *end);

  Node *RemoveStart();

  Node *RemoveEnd();

  void FetchNodes(NFA *nfa) {
    _nodes.insert(nfa->_nodes.begin(), nfa->_nodes.end());
    nfa->_nodes.clear();
  }

private:
  const char *MatchDFS(Node *curr, const char *beg, const char *end);

  const char *SearchDFS(Node *curr, const char *beg, const char *end);

private:
  Node *_start{nullptr};
  Node *_end{nullptr};
  std::unordered_set<Node *> _nodes;
};


/**
 * compose functions
 */

NFA *DoConcatenate(NFA *lhs, NFA *rhs);

inline NFA *Concatenate(NFA *lhs, NFA *rhs) {
  return DoConcatenate(lhs, rhs);
}

template<class ...A>
NFA *Concatenate(NFA *first, A... rest) {
  if (0 == sizeof...(rest)) {
    return first;
  } else {
    NFA *rest_result = Concatenate(rest...);
    return Concatenate(first, rest_result);
  }
}


NFA *DoLogicalOr(NFA *lhs, NFA *rhs);

inline NFA *LogicalOr(NFA *lhs, NFA *rhs) {
  return DoLogicalOr(lhs, rhs);
}

template<class ...A>
NFA *LogicalOr(NFA *first, A... rest) {
  if (0 == sizeof...(rest)) {
    return first;
  } else {
    NFA *rest_result = LogicalOr(rest...);
    return LogicalOr(first, rest_result);
  }
}


NFA *KleenStar(NFA *nfa);

NFA *Optional(NFA *nfa);

NFA *LeastOne(NFA *nfa);


/**
 *  deterministric finite automaton
 */

void RecordNFARecur(std::vector<Node *> &nodes, Node *u);

inline std::vector<Node *> RecordNFA(NFA *nfa) {
  std::vector<Node *> nodes;
  RecordNFARecur(nodes, nfa->start());
  return nodes;
}


void PrintFARecur(Node *u, std::vector<bool> &visit);

inline void PrintFA(Node *start, int max_number) {
  std::vector<bool> visit(max_number);
  PrintFARecur(start, visit);
}

/**
 * DFA converter
 */
class DFA;

class DFAConverter {
public:
  static DFA *ConvertFromNFA(NFA *nfa);

private:
  DFAConverter(NFA *nfa) : _nfa(nfa) {}

  struct SetHash {
    size_t operator()(const std::set<int> &s) const;
  };

  std::pair<Node *, bool> &FindNodeBySet(const std::set<int> &s);

  void ConversionPreamble();

  void EpsilonClosureRecur(std::set<int> &s, Node *u);

  const std::set<int> &EpsilonClosure(Node *u);

  Edge::EdgeChars GetSetEdgeChars(const std::set<int> &s);

  std::set<int> GetAdjacentSet(const std::set<int> &curr_set, char c);

  void ConstructDFADiagram();

  void CollectNodes();

  DFA *Convert();

private:
  std::unordered_map<std::set<int>, Node *, SetHash> _set_to_dfa_node;
  std::vector<std::set<int>> _e_closures;
  std::vector<Node *> _nfa_nodes;
  NFA *_nfa;
  DFA *_dfa;
};


/**
 * class DFA
 */
class DFA {
public:
  static DFA *ConvertFromNFA(NFA *nfa);

  friend class DFAConverter;

public:
  size_t size() {
    return _nodes.size();
  }

  Node *start() {
    return _start;
  }

  const char *Match(const char *beg, const char *end);

  const char *Search(const char *begin, const char *end);

private:
  Node *_start{nullptr};
  std::vector<Node *> _nodes;
  std::vector<Node *> _ends;
};


} // end of namespace transition_map
