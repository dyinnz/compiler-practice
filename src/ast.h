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

  Symbol symbol() const {
    return symbol_;
  }

  void set_token(Token token) {
    token_ = token;
  }

  const Token &token() const {
    return token_;
  }

  void AddChild(AstNode *child) {
    children_.push_back(child);
  }

  const std::vector<AstNode *> children() const {
    return children_;
  }

 private:
  std::vector<AstNode *> children_;
  Token token_;
  Symbol symbol_;
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
