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

namespace nfa_graph {

// pre-declaration
class Node;

/**
 * Edge
 */
class Edge {
public:
  static Edge *createEdgeFromString(const std::string &s);

  Node *NextNode() const {
    return _v;
  }

  void SetNextNode(Node *v) {
    _v = v;
  }

  virtual ~Edge() {}

  virtual bool Match(char c) const = 0;

  virtual int Delta() const = 0;

  virtual std::string ToString() const = 0;

  virtual Edge* Clone() const = 0;

private:
  Node *_v{nullptr};
};


class EpsilonEdge : public Edge {
public:
  virtual bool Match(char c) const {
    return true;
  }

  virtual int Delta() const {
    return 0;
  }

  virtual std::string ToString() const {
    return "-Epsilon-";
  }

  virtual Edge* Clone() const {
    return new EpsilonEdge;
  }
};


class CharEdge : public Edge {
public:
  CharEdge(char c) : _c(c) {}

  virtual bool Match(char c) const {
    return c == _c;
  }

  virtual int Delta() const {
    return 1;
  }

  virtual std::string ToString() const {
    return std::string("-") + _c + "-";
  }

  virtual Edge *Clone() const {
    return new CharEdge(_c);
  }

private:
  char _c;

};


/**
 *  Transition Diagram Node
 */
class Node {
public:
  enum StateType {
    kStart = 1 << 0,
    kEnd = 1 << 1,
    kStartEnd = (1 << 0) + (1 << 1),
    kNormal = 1 << 2,
  };

  Node(StateType type) : _type(type) {}

  ~Node() {
    for (auto edge : _edges) {
      delete edge;
    }
  }

  StateType type() const {
    return _type;
  }

  constexpr static int kUnsetNumber {-1};

  int number() const {
    return _number;
  }

  void set_number(int number) {
    _number = number;
  }

  void ChangeNormal() {
    _type = kNormal;
  }

  std::list<Edge*> edges() {
    return _edges;
  }

  class Iterator {
  private:
    char _c;
    std::list<Edge *>::const_iterator _curr;
    std::list<Edge *>::const_iterator _end;

  public:
    Iterator(char c,
             std::list<Edge *>::const_iterator curr,
             std::list<Edge *>::const_iterator end)
        : _c(c), _curr(curr), _end(end) {}

    Edge *NextEdge();
  };

  Iterator AvailableEdges(char c) const {
    return Iterator(c, _edges.begin(), _edges.end());
  }

  void AddEdge(Edge *e, Node *node) {
    e->SetNextNode(node);
    _edges.push_back(e);
  }

  void FetchEdges(Node *other) {
    _edges.splice(_edges.end(), other->_edges);
  }

private:
  StateType _type;
  int _number{kUnsetNumber};
  std::list<Edge *> _edges;
};


/**
 * non-deterministic finite automaton
 */
class NFA {
public:
  static NFA *CreateSimpleNFAFromEdge(Edge *e);

  static NFA *CreateSimpleNFAFromString(const std::string &s);

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

  const char *DFS(Node *curr, const char *beg, const char *end,
                  bool is_match);

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

void RecordNFARecur(std::vector<Node*> &nodes, Node *u);

inline std::vector<Node*> RecordNFA(NFA *nfa) {
  std::vector<Node*> nodes;
  RecordNFARecur(nodes, nfa->start());
  return nodes;
}


void PrintNFARecur(Node *u, std::vector<bool> &visit);
inline void PrintNFA(NFA *nfa, int max_number) {
  std::vector<bool> visit(max_number);
  PrintNFARecur(nfa->start(), visit);
}

class DFA;

class DFAConverter {
public:
  static DFA* ConvertNFAToDFA(NFA *nfa);

private:
  DFAConverter(NFA *nfa) : _nfa(nfa) {}

  void Expand(std::set<int> &s, Node *dfa_u);
  DFA* Convert();
  void EpsilonClosure(std::set<int> &s, Node *u);
  std::set<int> EpsilonClosure(Node *u);

  struct SetHash {
    size_t operator() (const std::set<int> &s) const;
  };

  std::unordered_map<std::set<int>, Node*, SetHash> _set_visit;
  std::vector<Node*> _nodes;
  NFA *_nfa;
  DFA *_dfa;
};

class DFA {
public:
  static DFA* ConvertFromNFA(NFA *nfa) {
    return DFAConverter::ConvertNFAToDFA(nfa);
  }

private:
  Node *_start {nullptr};
  std::vector<Node*> _ends {nullptr};
  std::unordered_set<Node*> _nodes;
};



} // end of namespace transition_map
