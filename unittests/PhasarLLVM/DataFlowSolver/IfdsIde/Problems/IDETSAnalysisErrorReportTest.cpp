/******************************************************************************
 * Copyright (c) 2020 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#include "phasar/DB/ProjectIRDB.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/IDETypeStateAnalysis.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/TypeStateDescriptions/CSTDFILEIOTypeStateDescription.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Solver/IDESolver.h"
#include "phasar/PhasarLLVM/Passes/ValueAnnotationPass.h"
#include "phasar/PhasarLLVM/Pointer/LLVMPointsToSet.h"
#include "phasar/PhasarLLVM/TypeHierarchy/LLVMTypeHierarchy.h"

#include "gtest/gtest.h"

using namespace psr;


TEST(IDETSAnalysisErrorReportTest, HandleTypeState_01) {
    const std::string PathToLlFiles =
      PhasarConfig::getPhasarConfig().PhasarDirectory() +
      "build/test/llvm_test_code/typestate_analysis_errorreport/";
    ProjectIRDB IRDB ({PathToLlFiles + "typestate_er_01_c.ll"}, IRDBOptions::WPA);
    LLVMTypeHierarchy TH (IRDB);
    LLVMPointsToSet PT (IRDB);
    LLVMBasedICFG ICFG (IRDB, CallGraphAnalysisType::OTF, {"main"}, &TH,
                             &PT);
    CSTDFILEIOTypeStateDescription CSTDFILEIODesc;
    IDETypeStateAnalysis TSProblem (&IRDB, &TH, &ICFG, &PT, CSTDFILEIODesc,
                                         {"main"});
  IDESolver_P<IDETypeStateAnalysis> Llvmtssolver(TSProblem);
  Llvmtssolver.solve();
  Llvmtssolver.emitErrorReport();
}

// main function for the test case
int main(int Argc, char **Argv) {
  ::testing::InitGoogleTest(&Argc, Argv);
  return RUN_ALL_TESTS();
}
