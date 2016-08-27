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
#include <unordered_set>

namespace nfa_graph {

// pre-declaration
class NFANode;

/**
 * Edge
 */
class Edge {
public:
  static Edge *createEdgeFromString(const std::string &s);

  NFANode *NextNode() const {
    return _v;
  }

  void SetNextNode(NFANode *v) {
    _v = v;
  }

  virtual ~Edge() {}

  virtual bool Match(char c) const = 0;

  virtual int Delta() const = 0;

  virtual void Debug() const = 0;

private:
  NFANode *_v{nullptr};

};


class EpsilonEdge : public Edge {
public:
  virtual bool Match(char c) const {
    return true;
  }

  virtual int Delta() const {
    return 0;
  }

  virtual void Debug() const {
    std::cout << "[epsilon]" << std::endl;
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

  virtual void Debug() const {
    std::cout << "[char: " << _c << "]" << std::endl;
  }

private:
  char _c;

};


/**
 *  Transition Diagram Node
 */
class NFANode {
public:
  enum StateType {
    kStart = 1 << 0,
    kEnd = 1 << 1,
    kStartEnd = (1 << 0) + (1 << 1),
    kNormal = 1 << 2,
  };

  NFANode(StateType type) : _type(type) {}

  ~NFANode() {
    for (auto edge : _edges) {
      delete edge;
    }
  }

  StateType type() const {
    return _type;
  }

  void ChangeNormal() {
    _type = kNormal;
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

  void AddEdge(Edge *e, NFANode *node) {
    e->SetNextNode(node);
    _edges.push_back(e);
  }

  void FetchEdges(NFANode *other) {
    _edges.splice(_edges.end(), other->_edges);
  }

private:
  StateType _type;
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
  NFA(NFANode *start, Edge *e, NFANode *end) : _start(start), _end(end) {
    _start->AddEdge(e, _end);
    _nodes.insert(start);
    _nodes.insert(end);
  }

  ~NFA() {
    for (auto node : _nodes) {
      delete node;
    }
  }

  const char *DFS(NFANode *curr, const char *beg, const char *end,
                  bool is_match);

  const char *Match(const char *beg, const char *end);

  const char *Search(const char *begin, const char *end);

  NFANode *start() {
    return _start;
  }

  NFANode *end() {
    return _end;
  }

  NFANode *ReplaceStart(NFANode *start);

  NFANode *ReplaceEnd(NFANode *end);

  NFANode *RemoveStart();

  NFANode *RemoveEnd();

  void FetchNodes(NFA *nfa) {
    _nodes.insert(nfa->_nodes.begin(), nfa->_nodes.end());
    nfa->_nodes.clear();
  }

private:
  NFANode *_start{nullptr};
  NFANode *_end{nullptr};
  std::unordered_set<NFANode *> _nodes;
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


} // end of namespace transition_map
