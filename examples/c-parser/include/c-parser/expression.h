#pragma once

#include <ares/rule.h>

using namespace ares;

struct ExpressionContext : public Context {
    size_t value;

    const static Rule rule;
};
