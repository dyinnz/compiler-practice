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

namespace finite_automaton {

// pre-declaration
class NFAEdge;

class Node;

class NFANode;

class DFANode;

class NFAComponent;

class NFA;

class NumberSet;

class DFAConverter;

class DFA;

class DFAOptimizer;


/*----------------------------------------------------------------------------*/

/**
 * unidirectional edge connected with NFA nodes
 */
class NFAEdge {
public:
  typedef std::bitset<CHAR_MAX + 1> CharMasks;

  static NFAEdge *CreateEpsilon() {
    return new NFAEdge;
  }

  static NFAEdge *CreateFromChar(char c);

  static NFAEdge *CreateFromString(const std::string &s);

  static NFAEdge *Merge(NFAEdge *lhs, NFAEdge *rhs);

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

private:
  void set(char c) {
    char_masks_.set(c);
  }

private:
  CharMasks char_masks_;
  NFANode *next_node_{nullptr};
};

std::string to_string(const NFAEdge &edge);


/*----------------------------------------------------------------------------*/

/**
 * common abstract super class of NFANode and DFANode
 */
class Node {
public:
  enum State {
    kStart, kEnd, kStartEnd, kNormal
  };

  constexpr static int kUnsetNumber{-1};

public:
  Node(State state) : state_(state) {}

  virtual ~Node() = 0;

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
  int number_{kUnsetNumber};
};


/*----------------------------------------------------------------------------*/

/**
 *  only for NFA diagram
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
 * only for DFA diagram
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
 * non-deterministic finite automaton component, only contains nodes
 */
class NFAComponent {
public:
  static NFAComponent *CreateFromEdge(NFAEdge *e);

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
 * Auxiliary functions for composing
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


/*----------------------------------------------------------------------------*/

/**
 * non-deterministic finite automaton, could match or search a string.
 */
class NFA {
public:
  NFA(NFANode *start,
      NFANode *end,
      const std::unordered_set<NFANode *> &nodes_manager)
      : start_(start),
        end_(end),
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

  const NFANode *start() const {
    return start_;
  }

  const NFANode *end() const {
    return end_;
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
  NFANode *end_{nullptr};
  std::vector<NFANode *> nodes_;
};


// for debug
void PrintNFARecur(const NFANode *u, std::vector<bool> &visit);

inline void PrintNFA(const NFANode *start, int max_number) {
  std::vector<bool> visit(max_number);
  PrintNFARecur(start, visit);
}


/*----------------------------------------------------------------------------*/

/**
 * a set contains number, support hashing and sharing with others
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

std::string to_string(const NumberSet &num_set);


/*----------------------------------------------------------------------------*/

/**
 * a helper class, convert NFA to DFA
 */
class DFAConverter {
public:
  static DFA *ConvertFromNFA(NFA *nfa);

private:
  DFAConverter(NFA *nfa) : nfa_(nfa) {}

  const NFANode *GetNFANode(int number) const {
    return nfa_->GetNode(number);
  }

  void ConversionPreamble();

  const NumberSet &EpsilonClosure(const NFANode *u);

  NFAEdge::CharMasks GetEdgeCharMasks(const NumberSet &num_set);

  NumberSet GetAdjacentSet(const NumberSet &curr_set, char c);

  DFANode *ConstructDFADiagram();

  std::vector<DFANode *> CollectEndNodes();

  std::vector<DFANode *> CollectAllNodes();

  DFA *Convert();

private:
  std::unordered_map<NumberSet, DFANode *, NumberSet::Hasher> set_to_dfa_node_;
  std::vector<NumberSet> e_closures_;
  NFA *nfa_;
};


/*----------------------------------------------------------------------------*/

/**
 *  deterministric finite automaton
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

  const char *Match(const char *beg, const char *end) const;

  const char *Search(const char *begin, const char *end) const;

private:
  void NumberNode();

private:
  DFANode *start_{nullptr};
  std::vector<DFANode *> ends_;
  std::vector<DFANode *> nodes_;
};


// for debug
void PrintDFARecur(const DFANode *u, std::vector<bool> &visit);

inline void PrintDFA(const DFANode *start, int max_number) {
  std::vector<bool> visit(max_number);
  PrintDFARecur(start, visit);
}


/*----------------------------------------------------------------------------*/

/**
 * a help class, minimize the DFA
 */
class DFAOptimizer {
public:
  static DFA *Minimize(DFA *normal);

private:
  DFAOptimizer(DFA *normal) : normal_(normal), num_to_set_(normal->size()) {}

  const DFANode *GetNormalNode(int number) {
    return normal_->GetNode(number);
  }

  void InitPartition();

  void BuildPartitionMap();

  std::unordered_set<char> GetSetEdgeChars(const NumberSet &s);

  bool PartSetByChar(std::list<NumberSet> &parted_curr_set,
                     const NumberSet &curr_set, char c);

  void TryPartEachSet();

  DFA *ConstructFromSets();

  DFA *Minimize();

private:
  DFA *normal_{nullptr};
  std::list<NumberSet> partition_;
  std::vector<NumberSet *> num_to_set_;
};


} // end of namespace transition_map
