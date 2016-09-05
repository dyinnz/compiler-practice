/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#pragma once

#include <climits>
#include <cassert>

#include <memory>
#include <string>
#include <bitset>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include "utility.h"

/*----------------------------------------------------------------------------*/

namespace regular_expression {

/**
 * class pre-declaration
 */

class NFAEdge;

class Node;

class NFANode;

class DFANode;

class NFAComponent;

class NFA;

class NumberSet;

class DFA;


/**
 * get the string representation
 */

std::string to_string(const NFAEdge &edge);

std::string to_string(const Node &node);

std::string to_string(const NumberSet &num_set);


/**
 * for debug
 */

void PrintNFA(const NFA *nfa);

void PrintDFA(const DFA *dfa);


/**
 * Auxiliary functions for composing
 */

NFAComponent *Concatenate(NFAComponent *lhs, NFAComponent *rhs);

template<class ...A>
NFAComponent *Concatenate(NFAComponent *first, A... rest);

NFAComponent *Union(NFAComponent *lhs, NFAComponent *rhs);

template<class ...A>
NFAComponent *Union(NFAComponent *first, A... rest);

NFAComponent *KleenStar(NFAComponent *nfa);

NFAComponent *Optional(NFAComponent *nfa);

NFAComponent *LeastOne(NFAComponent *nfa);

NFA *UnionWithMultiEnd(NFAComponent *lhs, NFAComponent *rhs);


/**
 * @param normal    the DFA to be minimized
 * @return          the minimum DFA, may be the same one if the parameter has
 *                  already minimum DFA
 */
DFA *MinimizeDFA(const DFA *normal);

/**
 * @param nfa   the NFA to be converted
 * @return      the DFA convert from NFA
 */
DFA *ConvertNFAToDFA(const NFA *nfa);


/*----------------------------------------------------------------------------*/

/**
 * @brief   unidirectional edge connected with NFA nodes
 */
class NFAEdge {
public:
  typedef std::bitset<CHAR_MAX + 1> CharMasks;

  static NFAEdge *CreateEpsilon() {
    return new NFAEdge;
  }

  static NFAEdge *CreateFromChar(char c);

  static NFAEdge *CreateFromRange(char beg, char end);

  static NFAEdge *CreateFromString(const std::string &s);

public:
  NFANode *next_node() {
    return next_node_;
  }

  void set_next_node(NFANode *v) {
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

  void set() {
    char_masks_.set();
  }

  void set(char c) {
    char_masks_.set(c);
  }

  void flip() {
    char_masks_.flip();
  }

  void SetRange(char beg, char end) {
    for (char c = beg; c < end; ++c) {
      set(c);
    }
  }

private:
  CharMasks char_masks_;
  NFANode *next_node_{nullptr};
};


/*----------------------------------------------------------------------------*/

/**
 * @brief   common abstract super class of NFANode and DFANode
 */
class Node {
public:
  enum State {
    kStart, kEnd, kStartEnd, kNormal
  };

  constexpr static int kUnsetInt{-1};

public:
  Node(State state) : state_(state) {}

  virtual ~Node() = 0;

  int type() const {
    return type_;
  }

  void set_type(int type) {
    type_ = type;
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

  int number() const {
    return number_;
  }

  void set_number(int number) {
    number_ = number;
  }

private:
  State state_;
  int type_{kUnsetInt};
  int number_{kUnsetInt};
};


/*----------------------------------------------------------------------------*/

/**
 *  @brief  only for NFA diagram
 */
class NFANode : public Node {
public:
  NFANode(State state) : Node(state) {}

  virtual ~NFANode() {
    for (auto edge : edges_) {
      delete edge;
    }
  }

  const std::list<NFAEdge *> &edges() const {
    return edges_;
  }

  void AddEdge(NFAEdge *edge, NFANode *node) {
    edge->set_next_node(node);
    edges_.push_back(edge);
  }

  void FetchEdges(NFANode *other) {
    edges_.splice(edges_.end(), other->edges_);
  }

private:
  std::list<NFAEdge *> edges_;
};


/*----------------------------------------------------------------------------*/

/**
 * @brief   only for DFA diagram
 */
class DFANode : public Node {
public:
  DFANode(State state) : Node(state) {}

  virtual ~DFANode() {}

  const std::unordered_map<char, DFANode *> &edges() const {
    return edges_;
  };

  void AddEdge(char c, DFANode *node) {
    edges_[c] = node;
  }

  const DFANode *GetNextNode(char c) const {
    auto iter = edges_.find(c);
    if (edges_.end() == iter) {
      return nullptr;
    } else {
      return iter->second;
    }
  }

private:
  std::unordered_map<char, DFANode *> edges_;
};


/*----------------------------------------------------------------------------*/

/**
 * @brief   non-deterministic finite automaton component, only contains nodes
 */
class NFAComponent {
public:
  static NFAComponent *CreateFromEdge(NFAEdge *e);

  static NFAComponent *CreateFromChar(char c);

  static NFAComponent *CreateFromRange(char beg, char end);

  static NFAComponent *CreateFromString(const std::string &s);

public:
  NFAComponent(NFANode *start, NFAEdge *e, NFANode *end)
      : start_(start), end_(end) {
    start_->AddEdge(e, end_);
    nodes_manager_.insert(start);
    nodes_manager_.insert(end);
  }

  ~NFAComponent() {
    for (auto node : nodes_manager_) {
      delete node;
    }
  }

  NFANode *start() {
    return start_;
  }

  NFANode *end() {
    return end_;
  }

  NFANode *SetNewStart(NFANode *start);

  NFANode *SetNewEnd(NFANode *end);

  NFANode *RemoveStart();

  NFANode *RemoveEnd();

  void FetchNodes(NFAComponent *nfa) {
    nodes_manager_.insert(nfa->nodes_manager_.begin(),
                          nfa->nodes_manager_.end());
    nfa->nodes_manager_.clear();
  }

  NFA *BuildNFA();

private:
  NFANode *start_{nullptr};
  NFANode *end_{nullptr};
  std::unordered_set<NFANode *> nodes_manager_;
};


/*----------------------------------------------------------------------------*/

/**
 * @brief   non-deterministic finite automaton, could match or search a string.
 */
class NFA {
public:
  NFA(NFANode *start, const std::unordered_set<NFANode *> &nodes_manager)
      : start_(start), nodes_(nodes_manager.begin(), nodes_manager.end()) {
    NumberNodes();
  }

  ~NFA() {
    start_ = nullptr;
    for (auto node : nodes_) {
      delete node;
    }
  }

  bool Match(const char *beg, const char *end) const;

  const char *Search(const char *begin, const char *end) const;

  const NFANode *start() const {
    return start_;
  }

  size_t size() const {
    return nodes_.size();
  }

  const NFANode *GetNode(size_t number) const {
    return nodes_[number];
  }

private:
  void NumberNodes();

  const char *MatchDFS(NFANode *curr, const char *beg, const char *end) const;

  const char *SearchDFS(NFANode *curr, const char *beg, const char *end) const;

private:
  NFANode *start_{nullptr};
  std::vector<NFANode *> nodes_;
};


/*----------------------------------------------------------------------------*/

/**
 * @brief   a set contains number, support hashing and sharing with others
 */
class NumberSet {
public:
  struct Hasher {
    size_t operator()(const NumberSet &num_set) const;
  };

public:
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

inline bool operator!=(const NumberSet &lhs, const NumberSet &rhs) {
  return !(lhs == rhs);
}


/*----------------------------------------------------------------------------*/

/**
 * @brief   deterministric finite automaton
 */
class DFA {
public:
  static DFA *ConvertFromNFA(NFA *nfa);

public:
  DFA(DFANode *start,
      std::vector<DFANode *> &&ends,
      std::vector<DFANode *> &&nodes)
      : start_(start), ends_(std::move(ends)), nodes_(std::move(nodes)) {
    NumberNode();
  }

  size_t size() const {
    return nodes_.size();
  }

  const DFANode *start() const {
    return start_;
  }

  const DFANode *GetNode(int number) const {
    return nodes_[number];
  }

  bool Match(const char *beg, const char *end) const;

  bool Match(const std::string &s) const;

  const char *Search(const char *begin, const char *end) const;

  size_t Search(const std::string &s) const;

private:
  void NumberNode();

private:
  DFANode *start_{nullptr};
  std::vector<DFANode *> ends_;
  std::vector<DFANode *> nodes_;
};


/*----------------------------------------------------------------------------*/

template<class ...A>
NFAComponent *Concatenate(NFAComponent *first, A... rest) {
  if (0 == sizeof...(rest)) {
    return first;
  } else {
    NFAComponent *rest_result = Concatenate(rest...);
    return Concatenate(first, rest_result);
  }
}

template<class ...A>
NFAComponent *Union(NFAComponent *first, A... rest) {
  if (0 == sizeof...(rest)) {
    return first;
  } else {
    NFAComponent *rest_result = Union(rest...);
    return Union(first, rest_result);
  }
}


} // end of namespace transition_map
