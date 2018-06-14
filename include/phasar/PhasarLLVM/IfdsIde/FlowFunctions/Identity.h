/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * Identity.h
 *
 *  Created on: 04.08.2016
 *      Author: pdschbrt
 */

#ifndef ANALYSIS_IFDS_IDE_FLOW_FUNC_IDENTITY_H_
#define ANALYSIS_IFDS_IDE_FLOW_FUNC_IDENTITY_H_

#include <memory>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunction.h>
#include <set>

namespace psr {

template <typename D> class Identity : public FlowFunction<D> {
private:
  Identity() = default;

public:
  virtual ~Identity() = default;
  Identity(const Identity &i) = delete;
  Identity &operator=(const Identity &i) = delete;
  // simply return what the user provides
  std::set<D> computeTargets(D source) override { return {source}; }
  static std::shared_ptr<Identity> getInstance() {
    static std::shared_ptr<Identity> instance =
        std::shared_ptr<Identity>(new Identity);
    return instance;
  }
};

} // namespace psr

#endif /* ANALYSIS_IFDS_IDE_FLOW_FUNC_IDENTITY_HH_ */
