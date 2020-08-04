#include <c-parser/root.h>

#include <c-parser/direction.h>

const Rule RootContext::rule = Rule()
    .context<RootContext>()
    .repeat(Rule()
        .peek("#", Rule().push(DirectionContext::rule))
    );
