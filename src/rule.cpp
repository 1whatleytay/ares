#include <ares/rule.h>

#include <ares/text.h>
#include <ares/instructions.h>

namespace ares {
    namespace instructions {
        Instruction::Instruction(Type type) : type(type) { }
    }

    Rule &Rule::next(const std::string &value) {
        instructions.push_back(std::make_shared<instructions::Next>(value));

        return *this;
    }

    Rule &Rule::next(const std::string &value, const Rule &success) {
        instructions.push_back(std::make_shared<instructions::NextOptional>(value, success));

        return *this;
    }

    Rule &Rule::peek(const std::string &value, const Rule &success) {
        instructions.push_back(std::make_shared<instructions::Peek>(value, success));

        return *this;
    }

    Rule &Rule::until(const std::vector<std::string> &values) {
        instructions.push_back(std::make_shared<instructions::UntilPush>(values));

        return *this;
    }

    Rule &Rule::until(const std::vector<std::string> &values, const AssignToken &assign) {
        instructions.push_back(std::make_shared<instructions::UntilAssign>(values, assign));

        return *this;
    }

    Rule &Rule::token() {
        instructions.push_back(std::make_shared<instructions::TokenPush>());

        return *this;
    }

    Rule &Rule::token(const AssignToken &assign) {
        instructions.push_back(std::make_shared<instructions::TokenAssign>(assign));

        return *this;
    }

    Rule &Rule::point() {
        instructions.push_back(std::make_shared<instructions::Point>());

        return *this;
    }

    Rule &Rule::jump() {
        instructions.push_back(std::make_shared<instructions::Jump>());

        return *this;
    }

    Rule &Rule::repeat(const Rule &content) {
        instructions.push_back(std::make_shared<instructions::Repeat>(content));

        return *this;
    }

    Rule &Rule::fail(const Rule &failure) {
        instructions.push_back(std::make_shared<instructions::Fail>(failure));

        return *this;
    }

    Rule &Rule::stop() {
        instructions.push_back(std::make_shared<instructions::Stop>());

        return *this;
    }

    Rule &Rule::quit() {
        instructions.push_back(std::make_shared<instructions::Quit>());

        return *this;
    }

    Rule &Rule::push(const Rule &sub) {
        if (!sub.isEnhanced)
            throw std::runtime_error("The sub rule must extend context.");

        instructions.push_back(std::make_shared<instructions::Push>(sub));

        return *this;
    }

    Rule &Rule::push(const Rule &sub, const Rule &success) {
        if (!sub.isEnhanced)
            throw std::runtime_error("The sub rule must extend context.");

        instructions.push_back(std::make_shared<instructions::PushOptional>(sub, success));

        return *this;
    }

    Rule &Rule::run(const Action &action) {
        instructions.push_back(std::make_shared<instructions::Run>(action));

        return *this;
    }

    Rule &Rule::dynamic(const Decision &decision, const Rule &success) {
        instructions.push_back(std::make_shared<instructions::Dynamic>(decision, success));

        return *this;
    }

    Builder Rule::build(const std::string &text) const {
        BuilderText builderText(text);

        Builder builder(*this, builderText);

        builder.execute();

        return builder;
    }
}
