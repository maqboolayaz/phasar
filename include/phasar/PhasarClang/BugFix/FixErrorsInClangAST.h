/******************************************************************************
 * Copyright (c) 2020 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#ifndef PHASAR_PHASARCLANG_BUGFIX_FIXERRORSINCLANGAST_H_
#define PHASAR_PHASARCLANG_BUGFIX_FIXERRORSINCLANGAST_H_

#include <iostream>
#include <string>

namespace psr {

    class FixErrorsInClangAST
    {

    public:
        FixErrorsInClangAST();
        int modifyFixInClangAST(std::string error_statement, std::string fix_statement, const char* filePath);
        int deleteFixInClangAST(std::string error_statement, const char* filePath);
        int addFixInClangAST(std::string error_statement, std::string fix_statement, const char* filePath);
        
    };

} // namespace psr

#endif
