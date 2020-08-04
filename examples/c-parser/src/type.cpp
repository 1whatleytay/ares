#include <c-parser/type.h>

const Rule TypeContext::rule = Rule()
    .context<TypeContext>()
    .token(&TypeContext::name);
