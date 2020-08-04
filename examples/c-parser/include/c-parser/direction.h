#pragma once

#include <ares/rule.h>

using namespace ares;

struct DirectionIncludeContext : public Context {
    bool isDirect = false;
    std::string path;

    const static Rule rule;
};

struct DirectionContext : public Context {
    enum class DirectionOp {
        Unknown,
        Include,
    };

    std::string value;
    DirectionOp op = DirectionOp::Unknown;

    const static Rule rule;
};
