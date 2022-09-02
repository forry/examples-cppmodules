export module mod:func;

import :SomeClassPart;

namespace imp
{
   export DLLMODULE_EXPORT int mod_f(int i);
   export DLLMODULE_EXPORT void print(const SomeClass& someClass);
}