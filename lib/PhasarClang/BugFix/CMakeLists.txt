project(BugFix)

add_library(${PROJECT_NAME} STATIC
    FixErrorsInClangAST.cpp
    PhasarASTConsumer.cpp
    PhasarASTVisitor.cpp
    PhasarFrontendAction.cpp
)

target_link_libraries(${PROJECT_NAME} ${CLANG_LIBRARY})

target_include_directories(${PROJECT_NAME} PUBLIC ${PHASAR_SRC_DIR}/include)

