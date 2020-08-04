#pragma once

#include <string>
#include <functional>

namespace ares {
    class Builder;

    using Action = std::function<void(Builder &builder)>;
    using Decision = std::function<bool(Builder &builder)>;
    using AssignToken = std::function<void(Builder &, const std::string &)>;
}
