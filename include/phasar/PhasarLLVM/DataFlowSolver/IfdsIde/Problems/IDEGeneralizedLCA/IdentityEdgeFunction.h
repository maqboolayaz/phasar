#pragma once

#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/EdgeFunction.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/EdgeFunctions/EdgeIdentity.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/IDEGeneralizedLCA/IDEGeneralizedLCA.h"

namespace psr {

class IdentityEdgeFunction
    : public EdgeFunction<IDEGeneralizedLCA::v_t>,
      public std::enable_shared_from_this<IdentityEdgeFunction> {
  size_t maxSize;

public:
  IdentityEdgeFunction(size_t maxSize);
  IDEGeneralizedLCA::v_t
  computeTarget(IDEGeneralizedLCA::v_t source) override;

  std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::v_t>>
  composeWith(
      std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::v_t>>
          secondFunction) override;

  std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::v_t>>
  joinWith(std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::v_t>>
               otherFunction) override;

  bool
  equal_to(std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::v_t>>
               other) const override;
  void print(std::ostream &OS, bool isForDebug = false) const override;
  static std::shared_ptr<IdentityEdgeFunction> getInstance(size_t maxSize);
};

// typedef EdgeIdentity<IDEGeneralizedLCA::v_t>
//   IdentityEdgeFunction;

} // namespace psr