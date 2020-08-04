#include <ares/instructions.h>

#include <utility>

namespace ares::instructions {
    Next::Next(std::string value)
        : Instruction(Type::Next), value(std::move(value)) { }

    NextOptional::NextOptional(std::string value, Rule success)
        : Instruction(Type::NextOptional), value(std::move(value)), success(std::move(success)) { }

    Peek::Peek(std::string value, Rule success)
        : Instruction(Type::Peek), value(std::move(value)), success(std::move(success)) { }

    UntilPush::UntilPush(std::vector<std::string> values)
        : Instruction(Type::UntilPush), values(std::move(values)) { }

    UntilAssign::UntilAssign(std::vector<std::string> values, AssignToken assign)
        : Instruction(Type::UntilAssign), values(std::move(values)), assign(std::move(assign)) { }

    TokenPush::TokenPush()
        : Instruction(Type::TokenPush) { }

    TokenAssign::TokenAssign(AssignToken assign)
        : Instruction(Type::TokenAssign), assign(std::move(assign)) { }

    Point::Point()
        : Instruction(Type::Point) { }

    Jump::Jump()
        : Instruction(Type::Jump) { }

    Repeat::Repeat(Rule content)
        : Instruction(Type::Repeat), content(std::move(content)) { }

    Fail::Fail(Rule failure)
        : Instruction(Type::Fail), failure(std::move(failure)) { }

    Stop::Stop()
        : Instruction(Type::Stop) { }

    Push::Push(Rule sub)
        : Instruction(Type::Push), sub(std::move(sub)) { }

    PushOptional::PushOptional(Rule sub, Rule success)
        : Instruction(Type::PushOptional), sub(std::move(sub)), success(std::move(success)) { }

    Quit::Quit()
        : Instruction(Type::Quit) { }

    Run::Run(Action action)
        : Instruction(Type::Run), action(std::move(action)) { }

    Dynamic::Dynamic(Decision decision, Rule success)
        : Instruction(Type::Dynamic), decision(std::move(decision)), success(std::move(success)) { }
}
