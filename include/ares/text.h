#pragma once

#include <string>
#include <vector>
#include <functional>

namespace ares {
    using Stoppable = std::function<bool(const char *, size_t)>;

    class BuilderText {
        std::string text;
        size_t index = 0;

    public:
//        static std::vector<char> defaultHard;
//        std::vector<char> hard = defaultHard;

        size_t get();
        void set(size_t value);

        void push();

        std::string pull(size_t size);
        void pop(size_t size);
        size_t until(const Stoppable &stoppable);
        bool complete(size_t size, const Stoppable &stoppable);

        BuilderText(std::string text);
    };
}
