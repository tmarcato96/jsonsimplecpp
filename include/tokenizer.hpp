#pragma once

#include <preproc_stream.hpp>
#include <tokens.hpp>

namespace Json {
    Token tokenize(preprocStream& stream);

    std::ostream& operator<<(std::ostream& os, reservedToken t);
}