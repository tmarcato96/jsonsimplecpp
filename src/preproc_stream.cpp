#include <preproc_stream.hpp>

using namespace Json;

preprocStream::preprocStream(const getcFunc& getc_input):
    _getc(getc_input)
    {
    }

int preprocStream::getc() {

    int ret;
    if (_stack.empty()) return _getc();
    else {
        ret = _stack.top();
        _stack.pop();
        return ret;
    }
}

void preprocStream::push_back(int c) {
    _stack.push(c);
}
