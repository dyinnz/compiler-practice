/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#pragma once

#include <climits>
#include <cassert>

#include <iostream>
#include <memory>
#include <string>
#include <bitset>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include "utility.h"

namespace finite_automaton {

// pre-declaration
class Node;

/**
 * Edge
 */
class Edge {
public:
  typedef std::bitset<CHAR_MAX + 1> CharMasks;

  static Edge *CreateEpsilon() {
    return new Edge;
  }

  static Edge *CreateFromChar(char c);

  static Edge *CreateFromString(const std::string &s);


  static Edge *Merge(Edge *lhs, Edge *rhs);

public:
  Node *next_node() {
    return next_node_;
  }

  void set_next_node(Node *v) {
    next_node_ = v;
  }

  bool IsEpsilon() const {
    return char_masks_.none();
  }

  bool test(char c) const {
    return char_masks_.test(c);
  }

  const CharMasks &char_masks() const {
    return char_masks_;
  }


private:
  void set(char c) {
    char_masks_.set(c);
  }

private:
  CharMasks char_masks_;
  Node *next_node_{nullptr};
};

std::string to_string(const Edge &edge);

/**
 *  Transition Diagram Node
 */
class Node {
public:
  enum State {
    kStart, kEnd, kStartEnd, kNormal
  };

public:
  Node(State state) : state_(state) {}

  ~Node() {
    for (auto edge : edges_) {
      delete edge;
    }
  }

  bool IsStart() const {
    return kStart == state_ || kStartEnd == state_;
  }

  bool IsEnd() const {
    return kEnd == state_ || kStartEnd == state_;
  }

  bool IsNormal() const {
    return kNormal == state_;
  }

  State state() const {
    return state_;
  }

  void AttachState(State state);

  constexpr static int kUnsetNumber{-1};

  int number() const {
    return number_;
  }

  void set_number(int number) {
    number_ = number;
  }

  const std::list<Edge *> &edges() const {
    return edges_;
  }

  void AddEdge(Edge *edge, Node *node) {
    edge->set_next_node(node);
    edges_.push_back(edge);
  }

  void FetchEdges(Node *other) {
    edges_.splice(edges_.end(), other->edges_);
  }

private:
  State state_;
  int number_{kUnsetNumber};
  std::list<Edge *> edges_;
};

std::string to_string(const Node &node);

/**
 * non-deterministic finite automaton
 */

class NFA;

class NFAComponent {
public:
  static NFAComponent *CreateFromEdge(Edge *e);

  static NFAComponent *CreateFromString(const std::string &s);

public:
  NFAComponent(Node *start, Edge *e, Node *end) : start_(start), end_(end) {
    start_->AddEdge(e, end_);
    nodes_manager_.insert(start);
    nodes_manager_.insert(end);
  }

  ~NFAComponent() {
    for (auto node : nodes_manager_) {
      delete node;
    }
  }

  Node *start() {
    return start_;
  }

  Node *end() {
    return end_;
  }

  Node *SetNewStart(Node *start);

  Node *SetNewEnd(Node *end);

  Node *RemoveStart();

  Node *RemoveEnd();

  void FetchNodes(NFAComponent *nfa) {
    nodes_manager_.insert(nfa->nodes_manager_.begin(),
                          nfa->nodes_manager_.end());
    nfa->nodes_manager_.clear();
  }

  NFA *BuildNFA();

private:
  Node *start_{nullptr};
  Node *end_{nullptr};
  std::unordered_set<Node *> nodes_manager_;
};


/**
 * compose functions
 */

NFAComponent *DoConcatenate(NFAComponent *lhs, NFAComponent *rhs);

inline NFAComponent *Concatenate(NFAComponent *lhs, NFAComponent *rhs) {
  return DoConcatenate(lhs, rhs);
}

template<class ...A>
NFAComponent *Concatenate(NFAComponent *first, A... rest) {
  if (0 == sizeof...(rest)) {
    return first;
  } else {
    NFAComponent *rest_result = Concatenate(rest...);
    return Concatenate(first, rest_result);
  }
}


NFAComponent *DoLogicalOr(NFAComponent *lhs, NFAComponent *rhs);

inline NFAComponent *LogicalOr(NFAComponent *lhs, NFAComponent *rhs) {
  return DoLogicalOr(lhs, rhs);
}

template<class ...A>
NFAComponent *LogicalOr(NFAComponent *first, A... rest) {
  if (0 == sizeof...(rest)) {
    return first;
  } else {
    NFAComponent *rest_result = LogicalOr(rest...);
    return LogicalOr(first, rest_result);
  }
}


NFAComponent *KleenStar(NFAComponent *nfa);

NFAComponent *Optional(NFAComponent *nfa);

NFAComponent *LeastOne(NFAComponent *nfa);


/**
 * class NFA
 */
class NFA {
public:
  NFA(Node *start, Node *end, const std::unordered_set<Node *> &nodes_manager)
      : start_(start), end_(end),
        nodes_(nodes_manager.begin(), nodes_manager.end()) {
    NumberNodes();
  }

  ~NFA() {
    delete start_;
    delete end_;
    for (auto node : nodes_) {
      delete node;
    }
  }

  const char *Match(const char *beg, const char *end) const;

  const char *Search(const char *begin, const char *end) const;

  const Node *start() const {
    return start_;
  }

  const Node *end() const {
    return end_;
  }

  size_t size() const {
    return nodes_.size();
  }

  const Node *GetNode(size_t number) const {
    return nodes_[number];
  }

private:
  void NumberNodes();

  const char *MatchDFS(Node *curr, const char *beg, const char *end) const;

  const char *SearchDFS(Node *curr, const char *beg, const char *end) const;

private:
  Node *start_{nullptr};
  Node *end_{nullptr};
  std::vector<Node *> nodes_;
};


/**
 *  deterministric finite automaton
 */

void PrintFARecur(const Node *u, std::vector<bool> &visit);

inline void PrintFA(const Node *start, int max_number) {
  std::vector<bool> visit(max_number);
  PrintFARecur(start, visit);
}


/**
 * class NumberSet
 */
class NumberSet {
public:
  struct Hasher {
    size_t operator()(const NumberSet &num_set) const;
  };

  NumberSet() : pset_(std::make_shared<std::set<int>>()) {}

  const std::set<int> &set() const {
    return *pset_;
  }

  std::set<int>::const_iterator begin() const {
    return pset_->begin();
  }

  std::set<int>::const_iterator end() const {
    return pset_->end();
  }

  bool empty() const {
    return pset_->empty();
  }

  bool contains(int num) const {
    return pset_->end() != pset_->find(num);
  }

  bool insert(int num) {
    return pset_->insert(num).second;
  }

  void insert(const NumberSet &num_set) {
    pset_->insert(num_set.begin(), num_set.end());
  }

private:
  std::shared_ptr<std::set<int>> pset_;
};

inline bool operator==(const NumberSet &lhs, const NumberSet &rhs) {
  return lhs.set() == rhs.set();
}

std::string to_string(const NumberSet &num_set);

/**
 * DFA converter
 */
class DFA;

class DFAConverter {
public:
  static DFA *ConvertFromNFA(NFA *nfa);

private:
  DFAConverter(NFA *nfa) : nfa_(nfa) {}

  struct SetHash {
    size_t operator()(const std::set<int> &s) const;
  };

  const Node *GetNode(int number) const {
    return nfa_->GetNode(number);
  }

  void ConversionPreamble();

  const NumberSet &EpsilonClosure(const Node *u);

  Edge::CharMasks GetEdgeCharMasks(const NumberSet &num_set);

  NumberSet GetAdjacentSet(const NumberSet &curr_set, char c);

  void ConstructDFADiagram();

  void CollectNodes();

  DFA *Convert();

private:
  std::unordered_map<NumberSet, Node *, NumberSet::Hasher> set_to_dfa_node_;
  std::vector<NumberSet> e_closures_;
  NFA *nfa_;
  DFA *dfa_;
};


/**
 * class DFA
 */
class DFA {
public:
  static DFA *ConvertFromNFA(NFA *nfa);

  friend class DFAConverter;

  friend class DFAOptimizer;

public:
  size_t size() {
    return nodes_.size();
  }

  Node *start() {
    return start_;
  }

  const char *Match(const char *beg, const char *end);

  const char *Search(const char *begin, const char *end);

private:
  Node *start_{nullptr};
  std::vector<Node *> nodes_;
  std::vector<Node *> ends_;
};


/**
 * class DFAOptimizer
 */
class DFAOptimizer {
public:
  static DFA *Minimize(DFA *normal);

private:
  DFAOptimizer(DFA *normal) : normal_(normal), normal_nodes_(normal_->nodes_) {
    partition_map_.resize(normal_nodes_.size());
  }

  void InitPartition();

  void BuildPartitionMap();

  Edge::CharMasks GetSetCharMasks(const std::set<int> &s);

  bool PartSetByChar(std::list<std::set<int>> &parted_curr_set,
                     const std::set<int> &curr_set, char c);

  void TryPartEachSet();

  void ConstructFromSets();

  DFA *Minimize();

private:
  DFA *minimum_{nullptr};
  DFA *normal_{nullptr};
  std::vector<Node *> normal_nodes_;
  std::list<std::set<int>> partition_;
  std::vector<std::set<int> *> partition_map_;
};


} // end of namespace transition_map
