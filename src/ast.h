//
// Created by coder on 16-9-16.
//

#pragma once

#include <vector>
#include <deque>
#include "mem_manager.h"
#include "token.h"

/**
 * @brief   Abstract Syntax Tree Node
 */
class AstNode {
 public:
  AstNode(Symbol symbol) : symbol_(symbol) {}

  AstNode(Token &&token)
      : symbol_(token.symbol),
        str_(std::move(token.text)),
        row_(token.row),
        column_(token.column) {}

  const Symbol &symbol() const {
    return symbol_;
  }

  const std::string &str() const {
    return str_;
  }

  size_t row() const {
    return row_;
  }

  size_t column() const {
    return column_;
  }

  void FetchToken(Token &&token) {
    str_ = std::move(token.text);
    row_ = token.row;
    column_ = token.column;
  }

  void push_child_back(AstNode *child) {
    children_.push_back(child);
  }

  void push_child_front(AstNode *child) {
    children_.push_front(child);
  }

  const std::deque<AstNode *> &children() const {
    return children_;
  }

 private:
  std::deque<AstNode *> children_;
  Symbol symbol_;
  std::string str_;
  size_t row_{0};
  size_t column_{0};
};

/**
 * @brief   Abstract Syntax Tree
 */
class Ast {
 public:
  AstNode *CreateNode(Symbol symbol) {
    return node_manager_.Create(symbol);
  }

  AstNode *CreateNode(Token &&token) {
    return node_manager_.Create(std::move(token));
  }

  void set_root(AstNode *node) {
    root_ = node;
  }

  AstNode *root() {
    return root_;
  }

 private:
  typedef SmallObjPool<AstNode> AstNodeManager;

  AstNode *root_;
  AstNodeManager node_manager_;
};
