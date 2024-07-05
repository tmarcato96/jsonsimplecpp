#include <preproc_stream.hpp>

preprocStream::preprocStream(const getcFunc& getc_input):
    _getc(getc_input)
    {
    }

int preprocStream::getc() {
    return _getc();
}
