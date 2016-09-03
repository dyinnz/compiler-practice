/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <algorithm>
#include <iostream>
#include <queue>
#include "simplelogger.h"
#include "nfa_graph.h"

using std::vector;
using std::string;
using std::set;
using std::pair;
using std::list;
using std::unordered_map;
using std::move;
using std::cout;
using std::endl;

extern simple_logger::BaseLogger logger;

namespace finite_automaton {

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
  lhs->char_masks_ |= rhs->char_masks_;
  delete rhs;
  return lhs;
}


string to_string(const Edge &edge) {
  string s{"--"};
  for (char c = 0; c < CHAR_MAX; ++c) {
    if (edge.test(c)) s += c;
  }
  s += "--";
  return s;
}

/*----------------------------------------------------------------------------*/

/**
 * Node
 */
void Node::AttachState(State state) {
  if ((kStart == state_ && kEnd == state)
      || (kStart == state && kEnd == state_)) {
    state_ = kStartEnd;
  } else {
    state_ = state;
  }
}

string to_string(const Node &node) {
  string s {"("};
  s += std::to_string(node.number());
  switch (node.state()) {
    case Node::kStart:    s += ":start)"; break;
    case Node::kEnd:      s += ":end)"; break;
    case Node::kStartEnd: s += ":start/end)"; break;
    case Node::kNormal:   s += ":normal)"; break;
    default:              s += ")"; break;
  }
  return s;
}

/*----------------------------------------------------------------------------*/

/**
 * class NFA
 */
NFAComponent *NFAComponent::CreateFromEdge(Edge *e) {
  return new NFAComponent(new Node(Node::kStart), e, new Node(Node::kEnd));
}


NFAComponent *NFAComponent::CreateFromString(const string &s) {
  return CreateFromEdge(Edge::CreateFromString(s));
}


Node *NFAComponent::SetNewStart(Node *start) {
  assert(start->IsStart());
  Node *old_start = start_;
  old_start->AttachState(Node::kNormal);
  start_ = start;
  nodes_manager_.insert(start_);
  return old_start;
}


Node *NFAComponent::SetNewEnd(Node *end) {
  assert(end->IsEnd());
  Node *old_end = end_;
  old_end->AttachState(Node::kNormal);
  end_ = end;
  nodes_manager_.insert(end_);
  return old_end;
}


Node *NFAComponent::RemoveStart() {
  Node *old_start = start_;
  start_ = nullptr;
  nodes_manager_.erase(old_start);
  return old_start;
}


Node *NFAComponent::RemoveEnd() {
  Node *old_end = end_;
  end_ = nullptr;
  nodes_manager_.erase(old_end);
  return old_end;
}


NFA* NFAComponent::BuildNFA() {
  auto nfa = new NFA(start_, end_, nodes_manager_);
  start_ = nullptr;
  end_ = nullptr;
  nodes_manager_.clear();
  return nfa;
}

/*----------------------------------------------------------------------------*/

/**
 * Auxiliary functions
 */
NFAComponent *DoConcatenate(NFAComponent *lhs, NFAComponent *rhs) {
  // merge the end of lhs and the start of rhs
  Node *rhs_start = rhs->RemoveStart();
  lhs->end()->FetchEdges(rhs_start);
  delete rhs_start;

  lhs->SetNewEnd(rhs->RemoveEnd());

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFAComponent *DoLogicalOr(NFAComponent *lhs, NFAComponent *rhs) {
  Node *rhs_start = rhs->RemoveStart();
  lhs->start()->FetchEdges(rhs_start);
  delete rhs_start;

  auto *new_end = new Node(Node::kEnd);
  lhs->end()->AddEdge(Edge::CreateEpsilon(), new_end);
  rhs->end()->AddEdge(Edge::CreateEpsilon(), new_end);
  lhs->SetNewEnd(new_end);
  rhs->end()->AttachState(Node::kNormal);

  lhs->FetchNodes(rhs);
  delete rhs;

  return lhs;
}


NFAComponent *KleenStar(NFAComponent *nfa) {
  Node *old_start = nfa->SetNewStart(new Node(Node::kStart));
  Node *old_end = nfa->SetNewEnd(new Node(Node::kEnd));

  old_end->AddEdge(Edge::CreateEpsilon(), old_start);

  nfa->start()->AddEdge(Edge::CreateEpsilon(), old_start);
  old_start->AddEdge(Edge::CreateEpsilon(), nfa->end());
  return nfa;
}


NFAComponent *Optional(NFAComponent *nfa) {
  nfa->start()->AddEdge(Edge::CreateEpsilon(), nfa->end());
  return nfa;
}


NFAComponent *LeastOne(NFAComponent *nfa) {
  Node *old_start = nfa->SetNewStart(new Node(Node::kStart));
  Node *old_end = nfa->SetNewEnd(new Node(Node::kEnd));

  nfa->start()->AddEdge(Edge::CreateEpsilon(), old_start);
  old_end->AddEdge(Edge::CreateEpsilon(), nfa->end());

  old_end->AddEdge(Edge::CreateEpsilon(), old_start);
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


const char * NFA::MatchDFS(Node *curr, const char *beg, const char *end) const {
  logger.log("{}", to_string(*curr));

  if (curr->IsEnd()) {
    // recurse base, arrive END state
    return beg;
  }

  for (Edge *edge : curr->edges()) {
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
  return (MatchDFS(start_, beg, end) == end) ? beg : nullptr;
}


const char *NFA::SearchDFS(Node *curr, const char *beg, const char *end) const {
  // TODO
  return nullptr;
}


const char *NFA::Search(const char *begin, const char *end) const {
  // TODO
  return nullptr;
}


/**
 * for debug
 */
void PrintFARecur(const Node *u, vector<bool> &visit) {
  visit[u->number()] = true;

  for (Edge *edge : u->edges()) {
    Node *v = edge->next_node();

    logger.debug("{}{}{}", to_string(*u), to_string(*edge), to_string(*v));

    if (!visit[v->number()]) {
      PrintFARecur(v, visit);
    }
  }
}

/*----------------------------------------------------------------------------*/

size_t NumberSet::Hasher::operator()(NumberSet &num_set) const {
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

/*----------------------------------------------------------------------------*/

// for debug
string to_string(const set<int> &num_set) {
  string str{"{"};
  for (int num : num_set) {
    str += std::to_string(num);
  }
  str += '}';
  return str;
}

/**
 * class DFAConverter
 */
size_t DFAConverter::SetHash::operator()(const std::set<int> &s) const {
  size_t value = 0;
  for (int num : s) {
    value |= num % sizeof(size_t);
  }
  return value;
}


void DFAConverter::ConversionPreamble() {
  e_closures_.resize(nfa_->size());
  set_to_dfa_node_.reserve(nfa_->size());
}


const set<int> &DFAConverter::EpsilonClosure(const Node *u) {
  set<int> &s = e_closures_[u->number()];
  if (!s.empty()) {
    return s;
  }

  // construct epsilon closure
  s.insert(u->number());
  for (Edge *edge : u->edges()) {
    if (edge->IsEpsilon()) {
      Node *v = edge->next_node();

      if (s.end() == s.find(v->number())) {
        auto v_s = EpsilonClosure(v);
        s.insert(v_s.begin(), v_s.end());
      }
    }
  }
  return s;
}


Edge::CharMasks DFAConverter::GetEdgeCharMasks(const set<int> &s) {
  Edge::CharMasks char_masks;
  for (int num : s) {
    for (Edge *edge : GetNode(num)->edges()) {
      char_masks |= edge->char_masks();
    }
  }
  return char_masks;
}


set<int> DFAConverter::GetAdjacentSet(const std::set<int> &curr_set, char c) {
  set<int> adjacent_set;
  for (int num : curr_set) {
    for (Edge *edge : GetNode(num)->edges()) {
      if (edge->test(c)) {
        const set<int> &e_closure = EpsilonClosure(edge->next_node());
        adjacent_set.insert(e_closure.begin(), e_closure.end());
      }
    }
  }
  return adjacent_set;
}


void DFAConverter::ConstructDFADiagram() {
  auto start_dfa_node = new Node(Node::kStart);
  dfa_->start_ = start_dfa_node;

  set<int> start_set = EpsilonClosure(nfa_->start());
  set_to_dfa_node_.insert({start_set, start_dfa_node});

  std::queue<set<int>> q;
  q.push(start_set);

  while (!q.empty()) {
    set<int> &curr_set = q.front();
    Node *dfa_curr = set_to_dfa_node_[curr_set];

    logger.log("current set {}", to_string(curr_set));

    Edge::CharMasks chars = GetEdgeCharMasks(curr_set);
    for (char c = 0; c < CHAR_MAX; ++c) {
      if (chars.test(c)) {

        set<int> adjacent_set = GetAdjacentSet(curr_set, c);

        auto iter = set_to_dfa_node_.find(adjacent_set);
        if (set_to_dfa_node_.end() == iter) {
          iter = set_to_dfa_node_.insert(
              {adjacent_set, new Node(Node::kNormal)}).first;
          q.push(adjacent_set);
        }
        Node *dfa_adjacent = iter->second;

        logger.log("char {}: adjacent set {}, node: {}", c,
                   to_string(adjacent_set), dfa_adjacent);

        dfa_curr->AddEdge(Edge::CreateFromChar(c), dfa_adjacent);
      }
    }

    q.pop();
  }
}


void DFAConverter::CollectNodes() {
  for (auto p : set_to_dfa_node_) {
    const set<int> &s = p.first;
    Node *dfa_node = p.second;

    dfa_node->set_number(dfa_->nodes_.size());
    dfa_->nodes_.push_back(dfa_node);

    for (int num : s) {
      if (GetNode(num)->IsEnd()) {
        if (!dfa_node->IsEnd()) {
          dfa_->ends_.push_back(dfa_node);
        }
        dfa_node->AttachState(Node::kEnd);
      }
    }
  }
}


DFA *DFAConverter::Convert() {
  dfa_ = new DFA;

  ConversionPreamble();

  ConstructDFADiagram();

  CollectNodes();

  return dfa_;
}


DFA *DFAConverter::ConvertFromNFA(NFA *nfa) {
  return DFAConverter(nfa).Convert();
}

/*----------------------------------------------------------------------------*/

/**
 *  class DFA
 */
DFA *DFA::ConvertFromNFA(NFA *nfa) {
  return DFAConverter::ConvertFromNFA(nfa);
}


const char *DFA::Match(const char *beg, const char *end) {
  const char *s = beg;
  Node *curr_node = start_;

  auto find_next_node = [&](Node *u, char c) -> Node * {
    for (Edge *edge : u->edges()) {
      if (edge->test(c)) return edge->next_node();
    }
    return nullptr;
  };

  logger.debug("{}", to_string(*curr_node));

  while (s != end) {
    Node *next_node = find_next_node(curr_node, *s);
    if (nullptr == next_node) {
      return nullptr;
    }

    curr_node = next_node;
    s += 1;

    logger.debug("{}", to_string(*curr_node));
  }

  return curr_node->IsEnd() ? s : nullptr;
}


const char *DFA::Search(const char *begin, const char *end) {
  return nullptr;
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

  set<int> &start_set = partition_.front();
  set<int> &end_set = partition_.back();

  for (Node *node : normal_->nodes_) {

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
  for (set<int> &s : partition_) {
    for (int num : s) {
      partition_map_[num] = &s;
    }
  }
}

Edge::CharMasks DFAOptimizer::GetSetCharMasks(const std::set<int> &s) {
  Edge::CharMasks char_masks;
  for (int num : s) {
    for (Edge *edge : normal_nodes_[num]->edges()) {
      char_masks |= edge->char_masks();
    }
  }
  return char_masks;
}


bool DFAOptimizer::PartSetByChar(list<set<int>> &parted_sets,
                                 const set<int> &curr_set, char c) {
  logger.log("part set: {} by {}", to_string(curr_set), c);

  unordered_map<set<int> *, set<int>> old_to_new;
  for (int u_num : curr_set) {

    bool is_match = false;
    for (Edge *edge : normal_nodes_[u_num]->edges()) {
      if (edge->test(c)) {
        int v_num = edge->next_node()->number();
        old_to_new[partition_map_[v_num]].insert(u_num);
        is_match = true;
      }
    }

    if (!is_match) {
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
  list<set<int>> new_partition;
  do {
    logger.log("new loop");

    last_size = partition_.size();
    new_partition.clear();

    BuildPartitionMap();

    for (set<int> &curr_set : partition_) {

      logger.log("current set: {}", to_string(curr_set));

      list<set<int>> parted_sets;
      Edge::CharMasks chars = GetSetCharMasks(curr_set);
      bool is_parted = false;

      for (char c = 0; c < CHAR_MAX; ++c) {
        if (chars.test(c)) {
          is_parted = PartSetByChar(parted_sets, curr_set, c);
          if (is_parted) {
            new_partition.splice(new_partition.begin(), parted_sets);
            break;
          }
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


void DFAOptimizer::ConstructFromSets() {
  std::vector<Node *> _normal_to_min(normal_nodes_.size());

  for (set<int> &s : partition_) {
    auto *node = new Node(Node::kNormal);
    node->set_number(minimum_->nodes_.size());
    minimum_->nodes_.push_back(node);

    for (int old_num : s) {
      _normal_to_min[old_num] = node;

      if (normal_nodes_[old_num]->IsStart()) {
        node->AttachState(Node::kStart);
        minimum_->start_ = node;
      }
      if (normal_nodes_[old_num]->IsEnd()) {
        node->AttachState(Node::kEnd);
        minimum_->ends_.push_back(node);
      }
    }
  }

  for (set<int> &s : partition_) {
    Node *u = _normal_to_min[*s.begin()];

    for (int num : s) {
      for (Edge *edge : normal_nodes_[num]->edges()) {
        Node *v = _normal_to_min[edge->next_node()->number()];
        u->AddEdge(new Edge(*edge), v);
      }
    }
  }
}


DFA *DFAOptimizer::Minimize() {

  InitPartition();
  if (partition_.size() <= 1) {
    // need not minimizing
    return normal_;
  }

  minimum_ = new DFA;

  TryPartEachSet();

  ConstructFromSets();

  return minimum_;
}


} // end of namespace finite_automaton
