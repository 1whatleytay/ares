#pragma once

#include <ares/rule.h>
#include <ares/callbacks.h>

#include <string>
#include <vector>
#include <functional>

namespace ares::instructions {
    class Next: public Instruction {
    public:
        std::string value;

        Next(std::string value);
    };

    class NextOptional: public Instruction {
    public:
        std::string value;
        Rule success;

        NextOptional(std::string value, Rule success);
    };

    class Peek: public Instruction {
    public:
        std::string value;
        Rule success;

        Peek(std::string value, Rule success);
    };

    class UntilPush: public Instruction {
    public:
        std::vector<std::string> values;

        UntilPush(std::vector<std::string> values);
    };

    class UntilAssign: public Instruction {
    public:
        std::vector<std::string> values;
        AssignToken assign;

        UntilAssign(std::vector<std::string> values, AssignToken assign);
    };

    class TokenPush: public Instruction {
    public:
        TokenPush();
    };

    class TokenAssign: public Instruction {
    public:
        AssignToken assign;

        TokenAssign(AssignToken assign);
    };

    class Point: public Instruction {
    public:
        Point();
    };

    class Jump: public Instruction {
    public:
        Jump();
    };

    class Repeat: public Instruction {
    public:
        Rule content;

        Repeat(Rule content);
    };

    class Fail: public Instruction {
    public:
        Rule failure;

        Fail(Rule failure);
    };

    class Stop: public Instruction {
    public:
        Stop();
    };

    class Push: public Instruction {
    public:
        Rule sub;

        Push(Rule sub);
    };

    class PushOptional: public Instruction {
    public:
        Rule sub;
        Rule success;

        PushOptional(Rule sub, Rule success);
    };

    class Quit: public Instruction {
    public:
        Quit();
    };

    class Run: public Instruction {
    public:
        Action action;

        Run(Action action);
    };

    class Dynamic: public Instruction {
    public:
        Decision decision;
        Rule success;

        Dynamic(Decision decision, Rule success);
    };
}
