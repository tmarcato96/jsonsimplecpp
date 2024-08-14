#pragma once

#include <stack>
#include <functional>

namespace Json {
    using getcFunc = std::function<int()>;

    class preprocStream {
        private:
            std::stack<int> _stack;
            const getcFunc& _getc;

        public:
            preprocStream(const getcFunc& getc_function);

            int getc();

            void push_back(int c);
    };
}