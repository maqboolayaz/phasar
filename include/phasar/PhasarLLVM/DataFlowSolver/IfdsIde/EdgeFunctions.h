/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * AbstractEdgeFunctions.h
 *
 *  Created on: 04.08.2016
 *      Author: pdschbrt
 */

#ifndef PHASAR_PHASARLLVM_IFDSIDE_EDGEFUNCTIONS_H_
#define PHASAR_PHASARLLVM_IFDSIDE_EDGEFUNCTIONS_H_

#include <iosfwd>
#include <iostream>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <string>

namespace psr {

//
// This class models an edge function for distributive data-flow problems.
//
// An edge function describes a value computation problem along an exploded
// supergraph edge.
//
template <typename L> class EdgeFunction {
public:
  using EdgeFunctionPtrType = std::shared_ptr<EdgeFunction<L>>;

  virtual ~EdgeFunction() = default;

  //
  // This function describes the concrete value computation for its respective
  // exploded supergraph edge. The function(s) will be evaluated once the
  // exploded supergraph has been constructed and the concrete values of the
  // various value computation problems along the supergraph edges are
  // evaluated.
  //
  // Please also refer to the various edge function factories of the
  // EdgeFunctions interface: EdgeFunctions::get*EdgeFunction() for more
  // details.
  //
  virtual L computeTarget(L Source) = 0;

  //
  // This function composes the two edge functions this and SecondFunction. This
  // function is used to extend an edge function in order to construct so-called
  // jump functions that describe the effects of everlonger sequences of code.
  //
  virtual EdgeFunctionPtrType
  composeWith(EdgeFunctionPtrType secondFunction) = 0;

  //
  // This function describes the join of the two edge functions this and
  // OtherFunction. The function is called whenever two edge functions need to
  // be joined, for instance, when two branches lead to a common successor
  // instruction.
  //
  virtual EdgeFunctionPtrType joinWith(EdgeFunctionPtrType OtherFunction) = 0;

  virtual bool equal_to(EdgeFunctionPtrType OtherFunction) const = 0;

  virtual void print(std::ostream &OS, bool IsForDebug = false) const {
    OS << "EdgeFunction";
  }

  std::string str() {
    std::ostringstream OSS;
    print(OSS);
    return OSS.str();
  }
};

template <typename L>
static inline bool operator==(const EdgeFunction<L> &F,
                              const EdgeFunction<L> &G) {
  return F.equal_to(G);
}

template <typename L>
static inline std::ostream &operator<<(std::ostream &OS,
                                       const EdgeFunction<L> &F) {
  F.print(OS);
  return OS;
}

template <typename L>
class AllTop : public EdgeFunction<L>,
               public std::enable_shared_from_this<AllTop<L>> {
public:
  using typename EdgeFunction<L>::EdgeFunctionPtrType;

private:
  const L topElement;

public:
  AllTop(L topElement) : topElement(topElement) {}

  ~AllTop() override = default;

  L computeTarget(L source) override { return topElement; }

  EdgeFunctionPtrType composeWith(EdgeFunctionPtrType secondFunction) override {
    return this->shared_from_this();
  }

  EdgeFunctionPtrType joinWith(EdgeFunctionPtrType otherFunction) override {
    return otherFunction;
  }

  bool equal_to(EdgeFunctionPtrType other) const override {
    if (auto *alltop = dynamic_cast<AllTop<L> *>(other.get())) {
      return (alltop->topElement == topElement);
    }
    return false;
  }

  void print(std::ostream &OS, bool isForDebug = false) const override {
    OS << "AllTop";
  }
};

template <typename L> class EdgeIdentity;

template <typename L>
class AllBottom : public EdgeFunction<L>,
                  public std::enable_shared_from_this<AllBottom<L>> {
public:
  using typename EdgeFunction<L>::EdgeFunctionPtrType;

private:
  const L bottomElement;

public:
  AllBottom(L bottomElement) : bottomElement(bottomElement) {}

  ~AllBottom() override = default;

  L computeTarget(L source) override { return bottomElement; }

  EdgeFunctionPtrType composeWith(EdgeFunctionPtrType secondFunction) override {
    if (auto *ab = dynamic_cast<AllBottom<L> *>(secondFunction.get())) {
      return this->shared_from_this();
    }
    if (auto *ei = dynamic_cast<EdgeIdentity<L> *>(secondFunction.get())) {
      return this->shared_from_this();
    }
    return secondFunction->composeWith(this->shared_from_this());
  }

  EdgeFunctionPtrType joinWith(EdgeFunctionPtrType otherFunction) override {
    if (otherFunction.get() == this ||
        otherFunction->equal_to(this->shared_from_this())) {
      return this->shared_from_this();
    }
    if (auto *alltop = dynamic_cast<AllTop<L> *>(otherFunction.get())) {
      return this->shared_from_this();
    }
    if (auto *ei = dynamic_cast<EdgeIdentity<L> *>(otherFunction.get())) {
      return this->shared_from_this();
    }
    return this->shared_from_this();
  }

  bool equal_to(EdgeFunctionPtrType other) const override {
    if (auto *allbottom = dynamic_cast<AllBottom<L> *>(other.get())) {
      return (allbottom->bottomElement == bottomElement);
    }
    return false;
  }

  void print(std::ostream &OS, bool isForDebug = false) const override {
    OS << "AllBottom";
  }
};

template <typename L>
class EdgeIdentity : public EdgeFunction<L>,
                     public std::enable_shared_from_this<EdgeIdentity<L>> {
public:
  using typename EdgeFunction<L>::EdgeFunctionPtrType;

private:
  EdgeIdentity() = default;

public:
  EdgeIdentity(const EdgeIdentity &ei) = delete;

  EdgeIdentity &operator=(const EdgeIdentity &ei) = delete;

  ~EdgeIdentity() override = default;

  L computeTarget(L source) override { return source; }

  EdgeFunctionPtrType composeWith(EdgeFunctionPtrType secondFunction) override {
    return secondFunction;
  }

  EdgeFunctionPtrType joinWith(EdgeFunctionPtrType otherFunction) override {
    if ((otherFunction.get() == this) ||
        otherFunction->equal_to(this->shared_from_this())) {
      return this->shared_from_this();
    }
    if (auto *ab = dynamic_cast<AllBottom<L> *>(otherFunction.get())) {
      return otherFunction;
    }
    if (auto *at = dynamic_cast<AllTop<L> *>(otherFunction.get())) {
      return this->shared_from_this();
    }
    // do not know how to join; hence ask other function to decide on this
    return otherFunction->joinWith(this->shared_from_this());
  }

  bool equal_to(EdgeFunctionPtrType other) const override {
    return this == other.get();
  }

  static EdgeFunctionPtrType getInstance() {
    // implement singleton C++11 thread-safe (see Scott Meyers)
    static EdgeFunctionPtrType instance(new EdgeIdentity<L>());
    return instance;
  }

  void print(std::ostream &OS, bool isForDebug = false) const override {
    OS << "EdgeIdentity";
  }
};

//===----------------------------------------------------------------------===//
// EdgeFunctions interface

template <typename AnalysisDomainTy> class EdgeFunctions {
public:
  using n_t = typename AnalysisDomainTy::n_t;
  using d_t = typename AnalysisDomainTy::d_t;
  using f_t = typename AnalysisDomainTy::f_t;
  using l_t = typename AnalysisDomainTy::l_t;

  using EdgeFunctionType = EdgeFunction<l_t>;
  using EdgeFunctionPtrType = typename EdgeFunctionType::EdgeFunctionPtrType;

  virtual ~EdgeFunctions() = default;

  //
  // Also refer to FlowFunctions::getNormalFlowFunction()
  //
  // Describes a value computation problem along a normal (non-call, non-return)
  // intra-procedural exploded supergraph edge. A normal edge function
  // implementation is queried for each edge that has been generated by appling
  // the flow function returned by FlowFunctions::getNormalFlowFunction(). The
  // supergraph edge whose computation is requested is defined by the supergraph
  // nodes CurrNode and SuccNode.
  //
  // Let instruction_1 := Curr, instruction_2 := Succ, and 0 the tautological
  // lambda fact.
  //
  // The concrete implementation of an edge function e is depending on the
  // analysis problem. In the following, we present a brief, contrived example:
  //
  // Consider the following flow function implementation (cf.
  // FlowFunctions::getNormalFlowfunction()):
  //
  //    f(0) -> {0}       // pass the lambda (or zero fact) as identity
  //    f(o) -> {o, x}    // generate a new fact x from o
  //    f(.) -> {.}       // pass all other facts that hold before instruction_1
  //                      // as identity
  //
  // The above flow-function implementation corresponds to the following edges
  // in the exploded supergraph.
  //
  //                                 0  o      ...
  //                                 |  |\     ...
  // Curr := x = instruction_1 o p   |  | \    ...
  //                                 |  |  |   ...
  //                                 v  v  v   ...
  //                                 0  o  x   ...
  //
  // Succ := y = instruction_2 q r
  //
  // For each edge generated by the respective flow function a normal edge
  // function is queried that describes a value computation. This results in the
  // following queries:
  //
  // getNormalEdgeFunction(0, Curr, 0 Succ);
  // getNormalEdgeFunction(o, Curr, o Succ);
  // getNormalEdgeFunction(o, Curr, x Succ);
  //
  virtual EdgeFunctionPtrType getNormalEdgeFunction(n_t Curr, d_t CurrNode,
                                                    n_t Succ, d_t SuccNode) = 0;

  //
  // Also refer to FlowFunctions::getCallFlowFunction()
  //
  // Describes a value computation problem along a call flow. A call edge
  // function is queried for each edge that has been generated by applying the
  // flow function that has been returned by FlowFunctions::getCallFlowFunction.
  // The supergraph edge whose computation is requested is defined by the
  // supergraph nodes SrcNode and DestNode.
  //
  // The concrete implementation of an edge function e is depending on the
  // analysis problem. In the following, we present a brief, contrived example:
  //
  // Consider the following flow function implementation (cf.
  // FlowFunctions::getCallFlowFunction()):
  //
  //    f(0) -> {0}       // pass as identity into the callee target
  //    f(o) -> {q}       // map actual o into formal q
  //    f(p) -> {r}       // map actual p into formal r
  //    f(.) -> {}        // kill all other facts that are not visible to the
  //                      // callee target
  //
  // The above implementation corresponds to the following edges in the exploded
  // supergraph.
  //
  //                                        0  o  p   ...
  //                                         \  \  \  ...
  // CallInst := x = CalleeFun(o, p, ...)     \  \  +----------------+
  //                                           \  +----------------  |
  //                                             +-------------+  +  |
  //                                                  ...      |  |  |
  //                                                  ...      |  |  |
  //                                        0  o  p   ...      |  |  |
  //                                                           |  |  |
  //                                                           |  |  |
  //                                                           |  |  |
  //                                 Ty CalleeFun(q, r, ...)   |  |  |
  //                                                           v  v  v
  //                                                           0  q  r   ...
  //
  //                                             start point
  //
  // For each edge generated by the respective flow function a call edge
  // function is queried that describes a value computation. This results in the
  // following queries:
  //
  // getCallEdgeFunction(CallInst, 0, CalleeFun, 0);
  // getCallEdgeFunction(CallInst, o, CalleeFun, q);
  // getCallEdgeFunction(CallInst, p, CalleeFun, r);
  //
  virtual EdgeFunctionPtrType getCallEdgeFunction(n_t CallInst, d_t SrcNode,
                                                  f_t CalleeFun,
                                                  d_t DestNode) = 0;

  //
  // Also refer to FlowFunction::getRetFlowFunction()
  //
  // Describes a value computation problem along a return flow. A return edge
  // function implementation is queried for each edge that has been generated by
  // applying the flow function that has been returned by
  // FlowFunctions::getRetFlowFunction(). The supergraph edge whose computation
  // is requested is defined by the supergraph nodes ExitNode and RetNode.
  //
  // The concrete implementation of an edge function e is depending on the
  // analysis problem. In the following, we present a brief, contrived example:
  //
  // Consider the following flow function implementation (cf.
  // FlowFunctions::getRetFlowFunction()):
  //
  //    f(0) -> {0}       // pass as identity into the callee target
  //    f(r) -> {x}       // map return value to lhs variable at CallSite
  //    f(q) -> {o}       // map pointer-typed formal q to actual o
  //    f(.) -> {}        // kill all other facts that are not visible to the
  //                      // caller
  //
  // The above implementation corresponds to the following edges in the exploded
  // supergraph.
  //
  //                                               0  o   ...
  //
  // CallSite = RetSite := x = CalleeFun(o, ...)
  //                                                     +------------------+
  //                                                  +--|---------------+  |
  //                                               +--|--|------------+  |  |
  //                                               v  v  v   ...      |  |  |
  //                                               0  o  x   ...      |  |  |
  //                                                                  |  |  |
  //                                                                  |  |  |
  //                                                                  |  |  |
  //                                           Ty CalleeFun(q, ...)   |  |  |
  //                                                                  |  |  |
  //                                                                  0  q  r
  //
  //                                           ExitInst := return r
  //
  // For each edge generated by the respective flow function a return edge
  // function is queried that describes a value computation. This results in the
  // following queries:
  //
  // getReturnEdgeFunction(CallSite, CalleeFun, ExitInst, 0, RetSite, 0);
  // getReturnEdgeFunction(CallSite, CalleeFun, ExitInst, q, RetSite, o);
  // getReturnEdgeFunction(CallSite, CalleeFun, ExitInst, r, RetSite, x);
  //
  virtual EdgeFunctionPtrType getReturnEdgeFunction(n_t CallSite, f_t CalleeFun,
                                                    n_t ExitInst, d_t ExitNode,
                                                    n_t RetSite,
                                                    d_t RetNode) = 0;

  //
  // Also refer to FlowFunctions::getCallToRetFlowFunction()
  //
  // Describes a value computation problem along data-flows alongsite a
  // CallSite. A return edge function implementation is queried for each edge
  // that has been generated by applying the flow function that has been
  // returned by FlowFunctions::getCallToRetFlowFunction(). The supergraph edge
  // whose computation is requested is defined by the supergraph nodes CallNode
  // and RetSiteNode.
  //
  // The concrete implementation of an edge function e is depending on the
  // analysis problem. In the following, we present a brief, contrived example:
  //
  // Consider the following flow function implementation (cf.
  // FlowFunctions::getCallToRetFlowFunction()):
  //
  //    f(0) -> {0}       // pass lambda as identity alongsite the CallSite
  //    f(o) -> {o}       // assuming that o is passed by value, it is passed
  //                      // alongsite the CallSite
  //    f(p) -> {}        // assuming that p is a pointer-typed value, we need
  //                      // to kill p, as it will be handled by the call- and
  //                      // return-flow functions
  //    f(.) -> {.}       // pass everything that is not involved in the call as
  //                      // identity
  //
  // The above implementation corresponds to the following edges in the exploded
  // supergraph.
  //
  //                                                  0  o   ...
  //                                                  |  |
  //                                                  |  +-------+
  //                                                  +--------+ |
  //                                                           | |
  // CallSite = RetSite := x = CalleeFun(o, p, ...)            | |
  //                                                           | |
  //                                                  +--------+ |
  //                                                  |  +-------+
  //                                                  v  v
  //                                                  0  o  x   ...
  //
  // For each edge generated by the respective flow function a call-to-return
  // edge function is queried that describes a value computation. This results
  // in the following queries:
  //
  // getCallToRetEdgeFunction(CallSite, 0, RetSite, 0, {CalleeFun});
  // getCallToRetEdgeFunction(CallSite, o, RetSite, o, {CalleeFun});
  //
  virtual EdgeFunctionPtrType
  getCallToRetEdgeFunction(n_t CallSite, d_t CallNode, n_t RetSite,
                           d_t RetSiteNode, std::set<f_t> Callees) = 0;

  //
  // Also refer to FlowFunction::getSummaryFlowFunction()
  //
  // Describes a value computation problem along a summary data flow. A summary
  // edge function implementation is queried for each edge that has been
  // generated by FlowFunctions::getSummaryFlowFunction(). The supergraph edge
  // whose computation is requested is defined by the supergraph nodes CurrNode
  // and SuccNode.
  //
  // The default implementation returns a nullptr to indicate that the mechanism
  // should not be used.
  //
  virtual EdgeFunctionPtrType
  getSummaryEdgeFunction(n_t Curr, d_t CurrNode, n_t Succ, d_t SuccNode) = 0;
};

} // namespace psr

#endif
