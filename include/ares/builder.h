#pragma once

#include <ares/text.h>

#include <stack>
#include <string>
#include <memory>
#include <vector>

namespace ares {
    class Rule;

    class Builder {
        static const std::vector<char> hard;

        static bool stoppable(const char *text, size_t size);

        Builder(const Rule &rule, const Builder &builder);

    public:
        bool contextEnhanced = false;
        std::shared_ptr<void> contextData;

        std::stack<size_t> indexStack;

        bool stop = false;
        bool quit = false;

        enum FailType {
            NoFail,
            SoftFail,
            HardFail,
        };

        FailType failType = FailType::NoFail;
        std::string failMessage;

        void fail(std::string message, FailType type = FailType::HardFail);

        const Rule &rule;
        BuilderText &text;

        std::vector<std::string> tokens;

        template <typename T>
        T *context() {
            return reinterpret_cast<T *>(contextData.get());
        }

        std::shared_ptr<void> extend(const Rule &addition);
        void execute();

        Builder(const Rule &rule, BuilderText &text);
    };
}
