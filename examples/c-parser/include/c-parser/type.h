#pragma once

#include <ares/rule.h>

using namespace ares;

struct TypeContext : public Context {
    std::string name;

    const static Rule rule;
};
