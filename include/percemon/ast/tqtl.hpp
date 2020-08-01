#pragma once

#ifndef __PERCEMON_AST_TQTL_HPP__
#define __PERCEMON_AST_TQTL_HPP__

#include "percemon/ast/ast.hpp"

namespace percemon {
namespace ast {

/**
 * Datastructure to pin frames
 */
struct Pin {
  std::optional<Var_x> x;
  std::optional<Var_f> f;

  Expr phi = {};

  Pin(std::optional<Var_x> x_, std::optional<Var_f> f_) : x{x_}, f{f_} {
    if (!x_.has_value() and !f_.has_value()) {
      throw std::invalid_argument(
          "Either time variable or frame variable must be specified when a frame is pinned.");
    }
  };

  Pin(std::optional<Var_x> x_) : Pin{x_, {}} {};
  Pin(std::optional<Var_f> f_) : Pin{{}, f_} {};

  Pin(const Pin& other) : x{other.x}, f{other.f}, phi{other.phi} {};
  Pin& operator=(const Pin& other) noexcept {
    if (this != &other) {
      x   = other.x;
      f   = other.f;
      phi = other.phi;
    }
    return *this;
  };

  Pin(Pin&& other) noexcept :
      x{std::move(other.x)}, f{std::move(other.f)}, phi{std::move(other.phi)} {};
  Pin& operator=(Pin&& other) noexcept {
    if (this != &other) {
      x   = std::move(other.x);
      f   = std::move(other.f);
      phi = std::move(other.phi);
    }
    return *this;
  };

  PinPtr dot(const Expr& e) const {
    auto ret = std::make_shared<Pin>(x, f);
    ret->phi = e;
    return ret;
  };
};

/**
 * Existential quantifier over IDs in either the current frame or a pinned frame.
 */
struct Exists {
  std::vector<Var_id> ids;
  std::optional<Pin> pinned_at = {};
  std::optional<Expr> phi      = {};

  Exists(std::vector<Var_id> id_list) : ids{id_list}, pinned_at{}, phi{} {};

  ExistsPtr at(const Pin& pin) const {
    auto ret       = std::make_shared<Exists>(ids);
    ret->pinned_at = pin;
    return ret;
  };

  ExistsPtr at(Pin&& pin) {
    auto ret       = std::make_shared<Exists>(ids);
    ret->pinned_at = pin;
    return ret;
  };

  ExistsPtr dot(const Expr& e) {
    auto ret = std::make_shared<Exists>(ids);
    if (auto& pin = this->pinned_at) {
      ret->pinned_at      = pin;
      ret->pinned_at->phi = e;
    } else {
      ret->phi = e;
    }
    return ret;
  };
};

/**
 * Universal quantifier over IDs in either the current frame or a pinned frame.
 */
struct Forall {
  std::vector<Var_id> ids;
  std::optional<Pin> pinned_at = {};
  std::optional<Expr> phi      = {};

  Forall(std::vector<Var_id> id_list) : ids{id_list}, pinned_at{}, phi{} {};

  ForallPtr at(const Pin& pin) const {
    auto ret       = std::make_shared<Forall>(ids);
    ret->pinned_at = pin;
    return ret;
  };

  ForallPtr at(Pin&& pin) {
    auto ret       = std::make_shared<Forall>(ids);
    ret->pinned_at = pin;
    return ret;
  };

  ForallPtr dot(const Expr& e) {
    auto ret = std::make_shared<Forall>(ids);
    if (auto& pin = this->pinned_at) {
      ret->pinned_at      = pin;
      ret->pinned_at->phi = e;
    } else {
      ret->phi = e;
    }
    return ret;
  };
};

struct Not {
  Expr arg;

  Not() = delete;
  Not(const Expr& arg_) : arg{arg_} {};
};

struct And {
  std::vector<Expr> args;
  std::vector<TemporalBoundExpr> temporal_bound_args;

  And() = delete;
  And(const std::vector<Expr>& args_) {
    if (args_.size() < 2) {
      throw std::invalid_argument(
          "It doesn't make sense to have an And operator with < 2 operands");
    }
    for (auto&& e : args_) {
      if (auto tb_ptr = std::get_if<TimeBound>(&e)) {
        temporal_bound_args.push_back(*tb_ptr);
      } else if (auto fb_ptr = std::get_if<FrameBound>(&e)) {
        temporal_bound_args.push_back(*fb_ptr);
      } else {
        args.push_back(e);
      }
    }
  };
};

struct Or {
  std::vector<Expr> args;
  std::vector<TemporalBoundExpr> temporal_bound_args;

  Or() = delete;
  Or(const std::vector<Expr>& args_) {
    if (args_.size() < 2) {
      throw std::invalid_argument(
          "It doesn't make sense to have an Or operator with < 2 operands");
    }
    for (auto&& e : args_) {
      if (auto tb_ptr = std::get_if<TimeBound>(&e)) {
        temporal_bound_args.push_back(*tb_ptr);
      } else if (auto fb_ptr = std::get_if<FrameBound>(&e)) {
        temporal_bound_args.push_back(*fb_ptr);
      } else {
        args.push_back(e);
      }
    }
  };
};

struct Previous {
  Expr arg;

  Previous() = delete;
  Previous(const Expr& arg_) : arg{arg_} {};
};

struct Always {
  Expr arg;

  Always() = delete;
  Always(const Expr& arg_) : arg{arg_} {};
};

struct Sometimes {
  Expr arg;

  Sometimes() = delete;
  Sometimes(const Expr& arg_) : arg{arg_} {};
};

struct Since {
  std::pair<Expr, Expr> args;

  Since() = delete;
  Since(const Expr& arg0, const Expr& arg1) : args{std::make_pair(arg0, arg1)} {};
};

struct BackTo {
  std::pair<Expr, Expr> args;

  BackTo() = delete;
  BackTo(const Expr& arg0, const Expr& arg1) : args{std::make_pair(arg0, arg1)} {};
};

} // namespace ast
} // namespace percemon

#endif /* end of include guard: __PERCEMON_AST_TQTL_HPP__ */
