#include <c-parser/direction.h>

const Rule DirectionIncludeContext::rule = Rule()
    .context<DirectionIncludeContext>()
    .next("include")
    .next("<", Rule()
        .until({ ">" }, &DirectionIncludeContext::path)
        .next(">")
    )
    .fail(Rule()
        .next("\"")
        .set(&DirectionIncludeContext::isDirect)
        .until({ "\"" }, &DirectionIncludeContext::path)
        .next("\"")
    );

const Rule DirectionContext::rule = Rule()
    .context<DirectionContext>()
    .next("#")
    // Include Directive
    .peek("include", Rule()
        .set(&DirectionContext::op, DirectionContext::DirectionOp::Include)
        .push(DirectionIncludeContext::rule)
    )
    .fail(Rule().quit());
