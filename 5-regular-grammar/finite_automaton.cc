/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <iostream>
#include <algorithm>
#include <queue>

#include "simplelogger.h"
#include "finite_automaton.h"

using std::vector;
using std::string;
using std::list;
using std::set;
using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::move;
using std::cout;
using std::endl;

extern simple_logger::BaseLogger logger;

/*----------------------------------------------------------------------------*/

namespace finite_automaton {

/**
 * class NFAEdge
 */

NFAEdge *NFAEdge::CreateFromChar(char c) {
  auto edge = new NFAEdge;
  edge->set(c);
  return edge;
}


NFAEdge *NFAEdge::CreateFromString(const std::string &s) {
  if (s.empty()) {
    return CreateEpsilon();

  } else if (1 == s.length()) {
    return CreateFromChar(s[0]);

  } else {
    return nullptr;
  }
}


NFAEdge *NFAEdge::Merge(NFAEdge *lhs, NFAEdge *rhs) {
  lhs->char_masks_ |= rhs->char_masks_;
  delete rhs;
  return lhs;
}


string to_string(const NFAEdge &edge) {
  string s{"--"};
  for (char c = 0; c < CHAR_MAX; ++c) {
    if (edge.test(c)) s += c;
  }
  s += "--";
  return s;
}


/*----------------------------------------------------------------------------*/
/**
 * class Node
 */

Node::~Node() {
  // do nothing
}


void Node::AttachState(State state) {
  if ((kStart == state_ && kEnd == state)
      || (kStart == state && kEnd == state_)) {
    state_ = kStartEnd;

  } else if (kStartEnd == state_ && (kStart == state || kEnd == state)) {
    // do nothing
  } else {
    state_ = state;
  }
}


string to_string(const Node &node) {
  string s{"("};
  s += std::to_string(node.number());
  switch (node.state()) {
    case Node::kStart:
      s += ":start)";
      break;
    case Node::kEnd:
      s += ":end)";
      break;
    case Node::kStartEnd:
      s += ":start/end)";
      break;
    case Node::kNormal:
      s += ":normal)";
      break;
    default:
      s += ")";
      break;
  }
  return s;
}


/*----------------------------------------------------------------------------*/
/**
 * class NFA
 */

NFAComponent *NFAComponent::CreateFromEdge(NFAEdge *e) {
  return new NFAComponent(new NFANode(Node::kStart), e,
                          new NFANode(Node::kEnd));
}


NFAComponent *NFAComponent::CreateFromString(const string &s) {
  return CreateFromEdge(NFAEdge::CreateFromString(s));
}


NFANode *NFAComponent::SetNewStart(NFANode *start) {
  assert(start->IsStart());
  NFANode *old_start = start_;
  old_start->AttachState(Node::kNormal);
  start_ = start;
  nodes_manager_.insert(start_);
  return old_start;
}


NFANode *NFAComponent::SetNewEnd(NFANode *end) {
  assert(end->IsEnd());
  NFANode *old_end = end_;
  old_end->AttachState(Node::kNormal);
  end_ = end;
  nodes_manager_.insert(end_);
  return old_end;
}


NFANode *NFAComponent::RemoveStart() {
  NFANode *old_start = start_;
  start_ = nullptr;
  nodes_manager_.erase(old_start);
  return old_start;
}


NFANode *NFAComponent::RemoveEnd() {
  NFANode *old_end = end_;
  end_ = nullptr;
  nodes_manager_.erase(old_end);
  return old_end;
}


NFA *NFAComponent::BuildNFA() {
  auto nfa = new NFA(start_, end_, nodes_manager_);
  start_ = nullptr;
  end_ = nullptr;
  nodes_manager_.clear();
  return nfa;
}


/*----------------------------------------------------------------------------*/
/**
 * Auxiliary functions for composing
 */

NFAComponent *DoConcatenate(NFAComponent *lhs, NFAComponent *rhs) {
  // merge the end of lhs and the start of rhs
  NFANode *rhs_start = rhs->RemoveStart();
  lhs->end()->FetchEdges(rhs_start);
  delete rhs_start;

  lhs->SetNewEnd(rhs->RemoveEnd());

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFAComponent *DoLogicalOr(NFAComponent *lhs, NFAComponent *rhs) {
  NFANode *rhs_start = rhs->RemoveStart();
  lhs->start()->FetchEdges(rhs_start);
  delete rhs_start;

  auto *new_end = new NFANode(Node::kEnd);
  lhs->end()->AddEdge(NFAEdge::CreateEpsilon(), new_end);
  rhs->end()->AddEdge(NFAEdge::CreateEpsilon(), new_end);
  lhs->SetNewEnd(new_end);
  rhs->end()->AttachState(Node::kNormal);

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFAComponent *KleenStar(NFAComponent *nfa) {
  NFANode *old_start = nfa->SetNewStart(new NFANode(Node::kStart));
  NFANode *old_end = nfa->SetNewEnd(new NFANode(Node::kEnd));

  old_end->AddEdge(NFAEdge::CreateEpsilon(), old_start);

  nfa->start()->AddEdge(NFAEdge::CreateEpsilon(), old_start);
  old_start->AddEdge(NFAEdge::CreateEpsilon(), nfa->end());
  return nfa;
}


NFAComponent *Optional(NFAComponent *nfa) {
  nfa->start()->AddEdge(NFAEdge::CreateEpsilon(), nfa->end());
  return nfa;
}


NFAComponent *LeastOne(NFAComponent *nfa) {
  NFANode *old_start = nfa->SetNewStart(new NFANode(Node::kStart));
  NFANode *old_end = nfa->SetNewEnd(new NFANode(Node::kEnd));

  nfa->start()->AddEdge(NFAEdge::CreateEpsilon(), old_start);
  old_end->AddEdge(NFAEdge::CreateEpsilon(), nfa->end());

  old_end->AddEdge(NFAEdge::CreateEpsilon(), old_start);
  return nfa;
}


/*----------------------------------------------------------------------------*/
/**
 * class NFA
 */

void NFA::NumberNodes() {
  for (size_t i = 0; i < nodes_.size(); ++i) {
    nodes_[i]->set_number(i);
  }
}


const char *
NFA::MatchDFS(NFANode *curr, const char *beg, const char *end) const {
  logger.debug("{}(): {}", __func__, to_string(*curr));

  if (curr->IsEnd()) {
    // recurse base, arrive END state
    return beg;
  }

  for (NFAEdge *edge : curr->edges()) {
    const char *match_pos = nullptr;

    if (edge->IsEpsilon()) {
      // epsilon move
      match_pos = MatchDFS(edge->next_node(), beg, end);

    } else if (edge->test(*beg) && beg < end) {
      // match
      match_pos = MatchDFS(edge->next_node(), beg + 1, end);

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


const char *NFA::Match(const char *beg, const char *end) const {
  logger.debug("{}(): {}", __func__, beg);
  return (MatchDFS(start_, beg, end) == end) ? beg : nullptr;
}


const char *
NFA::SearchDFS(NFANode *curr, const char *beg, const char *end) const {
  // TODO
  return nullptr;
}


const char *NFA::Search(const char *begin, const char *end) const {
  // TODO
  return nullptr;
}


// for debug
void PrintNFARecur(const NFANode *u, vector<bool> &visit) {
  visit[u->number()] = true;

  for (NFAEdge *edge : u->edges()) {
    NFANode *v = edge->next_node();

    logger.debug("{}{}{}", to_string(*u), to_string(*edge), to_string(*v));

    if (!visit[v->number()]) {
      PrintNFARecur(v, visit);
    }
  }
}


/*----------------------------------------------------------------------------*/
/**
 * class NumberSet
 */

size_t NumberSet::Hasher::operator()(const NumberSet &num_set) const {
  size_t value = 0;
  for (int num : num_set.set()) {
    value |= num % sizeof(size_t);
  }
  return value;
}

std::string to_string(const NumberSet &num_set) {
  std::string str{"{"};
  for (int num : num_set.set()) {
    str += std::to_string(num);
  }
  str += '}';
  return str;
}


// for debug
string to_string(const set<int> &num_set) {
  string str{"{"};
  for (int num : num_set) {
    str += std::to_string(num);
  }
  str += '}';
  return str;
}

/*----------------------------------------------------------------------------*/
/**
 * class DFA
 */

DFA *DFA::ConvertFromNFA(NFA *nfa) {
  return DFAConverter::ConvertFromNFA(nfa);
}


void DFA::NumberNode() {
  for (size_t i = 0; i < nodes_.size(); ++i) {
    nodes_[i]->set_number(i);
  }
}


const char *DFA::Match(const char *beg, const char *end) const {
  const char *s = beg;
  const DFANode *curr_node = start_;

  logger.debug("{}", to_string(*curr_node));
  while (s != end) {
    const DFANode *next_node = curr_node->GetNextNode(*s);

    if (next_node) {
      curr_node = next_node;
      s+= 1;
      logger.debug("{}", to_string(*curr_node));

    } else {
      return nullptr;
    }
  }

  return curr_node->IsEnd() ? s : nullptr;
}


const char *DFA::Search(const char *begin, const char *end) const {
  // TODO
  return nullptr;
}


// for debug
void PrintDFARecur(const DFANode *u, std::vector<bool> &visit) {
  visit[u->number()] = true;

  for (auto p : u->edges()) {
    DFANode *v = p.second;
    logger.debug("{}--{}--{}", to_string(*u), p.first, to_string(*v));

    if (!visit[v->number()]) {
      PrintDFARecur(v, visit);
    }
  }
}

/*----------------------------------------------------------------------------*/
/**
 * class DFAConverter
 */

void DFAConverter::ConversionPreamble() {
  e_closures_.resize(nfa_->size());
  set_to_dfa_node_.reserve(nfa_->size());
}


const NumberSet &DFAConverter::EpsilonClosure(const NFANode *u) {
  NumberSet &s = e_closures_[u->number()];
  if (!s.empty()) {
    return s;
  }

  // construct epsilon closure
  s.insert(u->number());
  for (NFAEdge *edge : u->edges()) {
    if (edge->IsEpsilon()) {
      NFANode *v = edge->next_node();

      if (!s.contains(v->number())) {
        auto v_s = EpsilonClosure(v);
        s.insert(v_s);
      }
    }
  }
  return s;
}


NFAEdge::CharMasks DFAConverter::GetEdgeCharMasks(const NumberSet &num_set) {
  NFAEdge::CharMasks char_masks;
  for (int num : num_set) {
    for (NFAEdge *edge : GetNFANode(num)->edges()) {
      char_masks |= edge->char_masks();
    }
  }
  return char_masks;
}


NumberSet DFAConverter::GetAdjacentSet(const NumberSet &curr_set, char c) {
  NumberSet adjacent_set;
  for (int num : curr_set) {
    for (NFAEdge *edge : GetNFANode(num)->edges()) {
      if (edge->test(c)) {
        auto e_closure = EpsilonClosure(edge->next_node());
        adjacent_set.insert(e_closure);
      }
    }
  }
  return adjacent_set;
}


DFANode *DFAConverter::ConstructDFADiagram() {
  auto start_dfa_node = new DFANode(Node::kStart);

  NumberSet start_set = EpsilonClosure(nfa_->start());
  set_to_dfa_node_.insert({start_set, start_dfa_node});

  std::queue<NumberSet> q;
  q.push(start_set);

  while (!q.empty()) {
    NumberSet curr_set = q.front();
    DFANode *dfa_curr = set_to_dfa_node_[curr_set];

    logger.debug("current set {}", to_string(curr_set));

    NFAEdge::CharMasks chars = GetEdgeCharMasks(curr_set);
    for (char c = 0; c < CHAR_MAX; ++c) {
      if (chars.test(c)) {

        NumberSet adjacent_set = GetAdjacentSet(curr_set, c);

        auto iter = set_to_dfa_node_.find(adjacent_set);
        if (set_to_dfa_node_.end() == iter) {
          iter = set_to_dfa_node_.insert(
              {adjacent_set, new DFANode(Node::kNormal)}).first;
          q.push(adjacent_set);
        }
        DFANode *dfa_adjacent = iter->second;

        logger.debug("char {}: adjacent set {}, node: {}", c,
                     to_string(adjacent_set), dfa_adjacent);

        dfa_curr->AddEdge(c, dfa_adjacent);
      }
    }

    q.pop();
  }

  return start_dfa_node;
}


vector<DFANode *> DFAConverter::CollectEndNodes() {
  // collect END nodes
  vector<DFANode *> ends;
  for (auto p : set_to_dfa_node_) {
    NumberSet num_set = p.first;
    DFANode *dfa_node = p.second;

    for (int num : num_set) {
      if (GetNFANode(num)->IsEnd()) {
        if (!dfa_node->IsEnd()) {
          ends.push_back(dfa_node);
        }
        dfa_node->AttachState(DFANode::kEnd);
      }
    }
  }
  return ends;
}


vector<DFANode *> DFAConverter::CollectAllNodes() {
  // collect all nodes and number them
  vector<DFANode *> nodes;
  for (auto p : set_to_dfa_node_) {
    nodes.push_back(p.second);
  }
  return nodes;
}


DFA *DFAConverter::Convert() {

  ConversionPreamble();

  DFANode *start = ConstructDFADiagram();

  auto ends = CollectEndNodes();
  auto nodes = CollectAllNodes();

  return new DFA(start, move(ends), move(nodes));
}


DFA *DFAConverter::ConvertFromNFA(NFA *nfa) {
  return DFAConverter(nfa).Convert();
}


/*----------------------------------------------------------------------------*/
/**
 * class DFAOptimizer
 */

DFA *DFAOptimizer::Minimize(DFA *normal) {
  return DFAOptimizer(normal).Minimize();
}


void DFAOptimizer::InitPartition() {
  partition_.emplace_back();
  partition_.emplace_back();

  NumberSet &start_set = partition_.front();
  NumberSet &end_set = partition_.back();

  for (size_t i = 0; i < normal_->size(); ++i) {
    const DFANode *node = normal_->GetNode(i);
    if (node->IsEnd()) {
      end_set.insert(node->number());
    } else {
      start_set.insert(node->number());
    }
  }

  if (start_set.empty()) {
    partition_.pop_front();
  }
}


void DFAOptimizer::BuildPartitionMap() {
  for (NumberSet &s : partition_) {
    for (int num : s) {
      num_to_set_[num] = &s;
    }
  }
}


unordered_set<char> DFAOptimizer::GetSetEdgeChars(const NumberSet &s) {
  unordered_set<char> chars;
  for (int num : s) {
    for (auto p : GetNormalNode(num)->edges()) {
      chars.insert(p.first);
    }
  }
  return chars;
}


bool DFAOptimizer::PartSetByChar(list<NumberSet> &parted_sets,
                                 const NumberSet &curr_set, char c) {
  logger.debug("part set: {} by {}", to_string(curr_set), c);

  unordered_map<NumberSet *, NumberSet> old_to_new;
  for (int u_num : curr_set) {
    const DFANode *u = GetNormalNode(u_num);
    const DFANode *v = u->GetNextNode(c);

    if (v) {
      old_to_new[num_to_set_[v->number()]].insert(u_num);
    } else {
      old_to_new[nullptr].insert(u_num);
    }
  }

  bool is_parted = false;
  for (auto p : old_to_new) {
    if (p.second != curr_set) {
      is_parted = true;
      PartSetByChar(parted_sets, p.second, c);
    }
  }

  if (is_parted) {
    auto iter = std::unique(parted_sets.begin(), parted_sets.end());
    parted_sets.erase(iter, parted_sets.end());

  } else {
    parted_sets.push_back(curr_set);
  }
  return is_parted;
}


void DFAOptimizer::TryPartEachSet() {
  size_t last_size = 0;
  list<NumberSet> new_partition;
  do {
    logger.debug("new loop");

    last_size = partition_.size();
    new_partition.clear();

    BuildPartitionMap();

    for (NumberSet &curr_set : partition_) {

      logger.debug("current set: {}", to_string(curr_set));

      auto chars = GetSetEdgeChars(curr_set);
      bool is_parted = false;

      for (char c : chars) {
        list<NumberSet> parted_sets;
        is_parted = PartSetByChar(parted_sets, curr_set, c);

        if (is_parted) {
          new_partition.splice(new_partition.begin(), parted_sets);
          break;
        }
      }

      if (!is_parted) {
        new_partition.push_back(curr_set);
      }
    }

    std::swap(new_partition, partition_);
  } while (last_size < partition_.size());

  logger.log("end of partition");
  for (auto &s : partition_) {
    logger.log("final parted set: {}", to_string(s));
  }
}


DFA *DFAOptimizer::ConstructFromSets() {
  std::vector<DFANode *> normal_to_min(normal_->size());

  DFANode *start{nullptr};
  vector<DFANode *> ends;
  vector<DFANode *> nodes;

  // collect
  for (NumberSet &s : partition_) {
    auto *node = new DFANode(Node::kNormal);
    nodes.push_back(node);

    for (int old_num : s) {
      normal_to_min[old_num] = node;

      if (GetNormalNode(old_num)->IsStart()) {
        node->AttachState(Node::kStart);
        start = node;
      }
      if (GetNormalNode(old_num)->IsEnd()) {
        node->AttachState(Node::kEnd);
        ends.push_back(node);
      }
    }
  }

  // add edges
  for (NumberSet &s : partition_) {
    DFANode *min_u = normal_to_min[*s.begin()];

    for (int num : s) {
      for (auto p : GetNormalNode(num)->edges()) {
        char c = p.first;
        DFANode *min_v = normal_to_min[p.second->number()];
        min_u->AddEdge(c, min_v);
      }
    }
  }

  return new DFA(start, move(ends), move(nodes));
}


DFA *DFAOptimizer::Minimize() {

  InitPartition();
  if (partition_.size() <= 1) {
    // need not minimizing
    return normal_;
  }

  TryPartEachSet();

  DFA *minimum = ConstructFromSets();
  return minimum;
}


} // end of namespace finite_automaton
