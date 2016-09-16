//
// Created by coder on 16-9-16.
//

#pragma once

#include <vector>
#include "mem_manager.h"
#include "token.h"

class AstNode {
 public:
  AstNode(bool is_terminal, int type)
      : is_terminal_(is_terminal), type_(type) {}

  int type() const {
    return type_;
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
  bool is_terminal_;
  int type_;
};

typedef SmallObjPool<AstNode> AstNodeManager;

class Ast {
 public:
  AstNode *CreateNode(bool is_terminal, int type) {
    return node_manager_.Create(is_terminal, type);
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
