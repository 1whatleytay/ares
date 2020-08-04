#pragma once

#include <ares/rule.h>

using namespace ares;

struct VariableHeadContext : public Context {
    std::string name;

    size_t numPointer = 0;
    std::vector<size_t> arraySizes;

    const static Rule rule;
};

struct VariableContext : public Context {
    const static Rule rule;
};
