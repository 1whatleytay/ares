#pragma once

#include <ares/builder.h>
#include <ares/context.h>
#include <ares/callbacks.h>

#include <fmt/format.h>

#include <string>
#include <vector>
#include <functional>

namespace ares {
    namespace instructions {
        class Instruction {
        public:
            enum class Type {
                Next,
                NextOptional,
                Peek,
                UntilPush,
                UntilAssign,
                TokenPush,
                TokenAssign,
                Point,
                Jump,
                Repeat,
                Fail,
                Stop,
                Push,
                PushOptional,
                Quit,
                Run,
                Dynamic,
            };

            Type type;

            explicit Instruction(Type type);
            virtual ~Instruction() = default;
        };
    }

    using Allocator = std::function<void *()>;
    using Deleter = std::function<void(void *)>;

    class Rule {
        bool hasContext = false;
        bool isEnhanced = false;
        Allocator allocator;
        Deleter deleter;

        std::vector<std::shared_ptr<instructions::Instruction>> instructions;

        friend class ares::Builder;
    public:
        Rule &next(const std::string &value);
        Rule &next(const std::string &value, const Rule &success);

        Rule &peek(const std::string &value, const Rule &success);

        Rule &until(const std::vector<std::string> &values);
        Rule &until(const std::vector<std::string> &values, const AssignToken &assign);
        template<typename T>
        Rule &until(const std::vector<std::string> &values, std::string T::* destination) {
            return until(values, [destination](Builder &builder, const std::string &value) {
                builder.context<T>()->*destination = value;
            });
        }

        Rule &token();
        Rule &token(const AssignToken &assign);
        template<typename T>
        Rule &token(std::string T::* destination) {
            return token([destination](Builder &builder, const std::string &value) {
                builder.context<T>()->*destination = value;
            });
        }
        template<typename T>
        Rule &token(size_t T::* destination) {
            return token([destination](Builder &builder, const std::string &value) {
                try {
                    builder.context<T>()->*destination = std::stoull(value);
                } catch (std::invalid_argument argument) {
                    builder.fail(fmt::format("Expected integer but got {}.", value));
                }
            });
        }
        template<typename T>
        Rule &token(double T::* destination) {
            return token([destination](Builder &builder, const std::string &value) {
                try {
                    builder.context<T>()->*destination = std::stod(value);
                } catch (std::invalid_argument argument) {
                    builder.fail(fmt::format("Expected floating point but got {}.", value));
                }
            });
        }

        template <typename T>
        Rule &set(bool T::* destination) {
            return run([destination](Builder &builder) {
                builder.context<T>()->*destination = true;
            });
        }

        template <typename T, typename N>
        Rule &set(N T::* destination, N value) {
            return run([destination, value](Builder &builder) {
                builder.context<T>()->*destination = value;
            });
        }

        Rule &point();
        Rule &jump();

        Rule &repeat(const Rule &content);
        Rule &fail(const Rule &failure);
        Rule &stop();
        Rule &quit();

        Rule &push(const Rule &sub);
        Rule &push(const Rule &sub, const Rule &success);

        Rule &run(const Action &action);
        Rule &dynamic(const Decision &decision, const Rule &success);

        template <typename T, typename std::enable_if<std::is_base_of<Context, T>::value>::type* = nullptr>
        Rule &context(Allocator alloc) {
            if (hasContext)
                throw std::runtime_error("Cannot set a rule context twice.");

            hasContext = true;
            isEnhanced = true;

            allocator = std::move(alloc);

            deleter = [](void *context) {
                delete reinterpret_cast<T *>(context);
            };

            return *this;
        }

        template <typename T, typename std::enable_if<!std::is_base_of<Context, T>::value>::type* = nullptr>
        Rule &context(Allocator alloc) {
            if (hasContext)
                throw std::runtime_error("Cannot set a rule context twice.");

            hasContext = true;

            allocator = std::move(alloc);

            deleter = [](void *context) {
                delete reinterpret_cast<T *>(context);
            };

            return *this;
        }

        template <typename T>
        Rule &context() {
            return context<T>([]() {
                return new T();
            });
        }

        Builder build(const std::string &text) const;
    };
}
