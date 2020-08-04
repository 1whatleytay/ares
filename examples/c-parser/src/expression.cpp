#include <c-parser/expression.h>

const Rule ExpressionContext::rule = Rule()
    .token(&ExpressionContext::value);
