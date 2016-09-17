//
// Created by coder on 16-9-16.
//

#pragma once

#include <vector>
#include "mem_manager.h"
#include "token.h"

class AstNode {
 public:
  AstNode(Symbol symbol) : symbol_(symbol) {}

  const Symbol &symbol() const {
    return symbol_;
  }

  const std::string &str() const {
    return str_;
  }

  void set_str(std::string &&str) {
    str_ = str;
  }

  void AddChild(AstNode *child) {
    children_.push_back(child);
  }

  const std::vector<AstNode *> children() const {
    return children_;
  }

 private:
  std::vector<AstNode *> children_;
  Symbol symbol_;
  std::string str_;
};

typedef SmallObjPool<AstNode> AstNodeManager;

class Ast {
 public:
  AstNode *CreateNode(Symbol symbol) {
    return node_manager_.Create(symbol);
  }

  void set_root(AstNode *node) {
    root_ = node;
  }

  AstNode *root() {
    return root_;
  }

 private:
  AstNode *root_;
  AstNodeManager node_manager_;
};
