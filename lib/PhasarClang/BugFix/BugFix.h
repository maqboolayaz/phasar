#pragma once
#include <string>

typedef enum {ADD, DELETE, MODIFY} OPERATION;

struct BugFix
{
    std::string error_statement; 
    std::string fix_statement;
    OPERATION fix_operation;
};