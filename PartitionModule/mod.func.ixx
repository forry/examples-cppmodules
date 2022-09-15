export module mod:func;

import :SomeClassPart; //importing fellow partition

namespace imp
{
   export int mod_f(int i);
   export void print(const SomeClass& someClass);
}