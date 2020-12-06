#include <tuple>

#include "gtest/gtest.h"

#include "phasar/DB/ProjectIRDB.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/IDELinearConstantAnalysis.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Solver/IDESolver.h"
#include "phasar/PhasarLLVM/Passes/ValueAnnotationPass.h"
#include "phasar/PhasarLLVM/Pointer/LLVMPointsToSet.h"
#include "phasar/PhasarLLVM/TypeHierarchy/LLVMTypeHierarchy.h"

#include "TestConfig.h"

using namespace psr;

/* ============== BASIC TESTS ============== */
TEST(IDELinearConstantAnalysisTest, HandleBasicTest_01) {
  const std::string PathToLlFiles =
  PhasarConfig::getPhasarConfig().PhasarDirectory() +
    "build/test/llvm_test_code/linear_constant/";
  ProjectIRDB IRDB ({PathToLlFiles + "basic_01_cpp_dbg.ll"}, IRDBOptions::WPA);
  LLVMTypeHierarchy TH (IRDB);
  LLVMPointsToSet PT (IRDB);
  LLVMBasedICFG ICFG (IRDB, CallGraphAnalysisType::OTF, {"main"}, &TH,
                            &PT);
  IDELinearConstantAnalysis LCAProblem(&IRDB, &TH, &ICFG, &PT,
                                         {"main"});
  IDESolver_P<IDELinearConstantAnalysis> LCASolver(LCAProblem);
  LCASolver.solve();
  LCASolver.emitTextReport();
}

// main function for the test case/*  */
int main(int Argc, char **Argv) {
  ::testing::InitGoogleTest(&Argc, Argv);
  return RUN_ALL_TESTS();
}
