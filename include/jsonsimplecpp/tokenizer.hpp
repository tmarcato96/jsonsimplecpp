#pragma once

#include <jsonsimplecpp/preproc_stream.hpp>
#include <jsonsimplecpp/tokens.hpp>

namespace Json {
    Token tokenize(preprocStream& stream);

    std::ostream& operator<<(std::ostream& os, reservedToken t);
}