#pragma once

#include <vector>

namespace ares {
    class Context {
    public:
        size_t index = 0;
        std::vector<std::shared_ptr<Context>> children;

        template <typename T>
        T *as() {
            return dynamic_cast<T *>(this);
        }

        virtual ~Context() = default;
    };
}
