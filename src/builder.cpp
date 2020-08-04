#include <ares/builder.h>

#include <ares/text.h>
#include <ares/rule.h>
#include <ares/instructions.h>

#include <fmt/printf.h>

#include <memory>

namespace ares {
    const std::vector<char> Builder::hard = {
        ':',
        ';',
        ',',
        '.',
        '{',
        '}',
        '+',
        '-',
        '=',
        '/',
        '\\',
        '@',
        '#',
        '$',
        '%',
        '^',
        '&',
        '|',
        '*',
        '(',
        ')',
        '!',
        '?',
        '<',
        '>',
        '~',
        '[',
        ']',
        '\"',
        '\''
    };

    bool Builder::stoppable(const char *text, size_t size) {
        return std::isspace(*text) || (std::find(hard.begin(), hard.end(), *text) != hard.end());
    }

    void Builder::fail(std::string message, FailType type) {
        failType = type;
        failMessage = message;
    }

    std::shared_ptr<void> Builder::extend(const Rule &addition) {
        Builder builder(addition, *this);

        builder.execute();

        // inherit stop/quit
        stop = builder.stop;
        quit = builder.quit;
        failType = builder.failType;
        failMessage = builder.failMessage;

        return builder.contextData;
    }

    void Builder::execute() {
        for (const auto &instruction : rule.instructions) {
            if (instruction->type != instructions::Instruction::Type::Fail) {
                if (failType == FailType::HardFail) {
                    fmt::print("ERROR {}\n", failMessage);
                    break;
                } else {
                    failType = FailType::NoFail;
                }
            }

            switch (instruction->type) {
                case instructions::Instruction::Type::Next: {
                    auto *e = reinterpret_cast<const instructions::Next *>(instruction.get());

                    std::string value = text.pull(e->value.size());
                    if (value == e->value && text.complete(e->value.size(), stoppable)) {
                        text.pop(e->value.size());
                    } else {
                        failType = FailType::HardFail;
                        failMessage = fmt::format("Expected \"{}\" but got \"{}\".", e->value, value);
                    }

                    break;
                }
                case instructions::Instruction::Type::NextOptional: {
                    auto *e = reinterpret_cast<const instructions::NextOptional *>(instruction.get());

                    if (text.pull(e->value.size()) == e->value && text.complete(e->value.size(), stoppable)) {
                        text.pop(e->value.size());
                        extend(e->success);
                    } else {
                        failType = FailType::SoftFail;
                        failMessage = "";
                    }

                    break;
                }
                case instructions::Instruction::Type::Peek: {
                    auto *e = reinterpret_cast<const instructions::Peek *>(instruction.get());

                    if (text.pull(e->value.size()) == e->value && text.complete(e->value.size(), stoppable)) {
                        extend(e->success);
                    } else {
                        failType = FailType::SoftFail;
                        failMessage = "";
                    }

                    break;
                }

                case instructions::Instruction::Type::UntilPush: {
                    auto *e = reinterpret_cast<const instructions::UntilPush *>(instruction.get());

                    std::string until = text.pull(text.until([e](const char *test, size_t size) {
                        for (const std::string &value : e->values) {
                            if (size < value.size())
                                continue;

                            if (std::memcmp(test, value.c_str(), value.size()) == 0)
                                return true;
                        }

                        return false;
                    }));
                    text.pop(until.size());

                    tokens.push_back(until);

                    break;
                }
                case instructions::Instruction::Type::UntilAssign: {
                    auto *e = reinterpret_cast<const instructions::UntilAssign *>(instruction.get());

                    std::string until = text.pull(text.until([e](const char *test, size_t size) {
                        for (const std::string &value : e->values) {
                            if (size < value.size())
                                continue;

                            if (std::memcmp(test, value.c_str(), value.size()) == 0)
                                return true;
                        }

                        return false;
                    }));
                    text.pop(until.size());

                    e->assign(*this, until);

                    break;
                }
                case instructions::Instruction::Type::TokenPush: {
//                    auto *e = reinterpret_cast<const instructions::TokenPush *>(instruction.get());

                    std::string token = text.pull(text.until(stoppable));
                    text.pop(token.size());

                    tokens.push_back(token);

                    break;
                }
                case instructions::Instruction::Type::TokenAssign: {
                    auto *e = reinterpret_cast<const instructions::TokenAssign *>(instruction.get());

                    std::string token = text.pull(text.until(stoppable));
                    e->assign(*this, token);

                    text.pop(token.size());

                    break;
                }
                case instructions::Instruction::Type::Point: {
                    indexStack.push(text.get());

                    break;
                }
                case instructions::Instruction::Type::Jump: {
                    text.set(indexStack.top());
                    indexStack.pop();

                    break;
                }
                case instructions::Instruction::Type::Repeat: {
                    auto *e = reinterpret_cast<const instructions::Repeat *>(instruction.get());

                    while (!stop) {
                        extend(e->content);
                    }

                    stop = false; // catch stop

                    break;
                }
                case instructions::Instruction::Type::Fail: {
                    auto *e = reinterpret_cast<const instructions::Fail *>(instruction.get());

                    if (failType != FailType::NoFail) {
                        extend(e->failure);
                        failType = FailType::NoFail;
                    }

                    break;
                }
                case instructions::Instruction::Type::Stop: {
//                    auto *e = reinterpret_cast<const instructions::Stop *>(instruction.get());

                    stop = true;

                    break;
                }
                case instructions::Instruction::Type::Quit: {
//                    auto *e = reinterpret_cast<const instructions::Quit *>(instruction.get());

                    quit = true;

                    break;
                }
                case instructions::Instruction::Type::Push: {
                    auto *e = reinterpret_cast<const instructions::Push *>(instruction.get());

                    if (!contextEnhanced)
                        throw std::runtime_error("Current context is not enhanced.");

                    std::shared_ptr<Context> value = std::static_pointer_cast<Context>(extend(e->sub));

                    if (failType != FailType::HardFail) {
                        context<Context>()->children.push_back(value);
                    }

                    break;
                }
                case instructions::Instruction::Type::PushOptional: {
                    auto *e = reinterpret_cast<const instructions::PushOptional *>(instruction.get());

                    if (!contextEnhanced)
                        throw std::runtime_error("Current context is not enhanced.");

                    std::shared_ptr<Context> value = std::static_pointer_cast<Context>(extend(e->sub));

                    if (failType != FailType::HardFail) {
                        context<Context>()->children.push_back(value);
                        extend(e->success);
                    }

                    break;
                }
                case instructions::Instruction::Type::Run: {
                    auto *e = reinterpret_cast<const instructions::Run *>(instruction.get());

                    e->action(*this);

                    break;
                }
                case instructions::Instruction::Type::Dynamic: {
                    auto *e = reinterpret_cast<const instructions::Dynamic *>(instruction.get());

                    if (e->decision(*this)) {
                        extend(e->success);
                    } else {
                        failType = FailType::SoftFail;
                    }

                    break;
                }
                default:
                    fmt::print("Unknown instruction type.");
                    break;
            }

            if (stop || quit)
                break;
        }
    }

    Builder::Builder(const Rule &rule, const Builder &builder)
        : rule(rule), text(builder.text), contextEnhanced(builder.contextEnhanced), contextData(builder.contextData) {
        if (rule.hasContext) {
            contextData = std::shared_ptr<void>(rule.allocator(), rule.deleter);
            contextEnhanced = rule.isEnhanced;
        }
    }

    Builder::Builder(const Rule &rule, BuilderText &text) : rule(rule), text(text) {
        if (rule.hasContext) {
            contextData = std::shared_ptr<void>(rule.allocator(), rule.deleter);
            contextEnhanced = rule.isEnhanced;
        }
    }
}
