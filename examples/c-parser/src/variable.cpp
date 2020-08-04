#include <c-parser/variable.h>

#include <c-parser/type.h>
#include <c-parser/expression.h>

#include <fmt/format.h>

const Rule VariableHeadContext::rule = Rule()
    .context<VariableHeadContext>()
    .repeat(Rule()
        .next("*", Rule().run([](Builder &builder) {
            builder.context<VariableHeadContext>()->numPointer++;
        }))
        .fail(Rule().stop())
    )
    .token(&VariableHeadContext::name)
    .repeat(Rule()
        .next("[", Rule()
            .token([](Builder &builder, std::string value) {
                try {
                    size_t size = std::stoull(value);
                    builder.context<VariableHeadContext>()->arraySizes.push_back(size);
                } catch (std::invalid_argument) {
                    builder.fail(fmt::format("{} is not a valid int.", value));
                }
            })
            .next("]")
        )
        .fail(Rule().stop())
    )
    .next("=", Rule()
        .push(ExpressionContext::rule)
    );

const Rule VariableContext::rule = Rule()
    .context<VariableContext>()
    .push(TypeContext::rule)
    .repeat(Rule()
        .push(VariableHeadContext::rule)
        .next(",", Rule())
        .fail(Rule().stop())
    );
