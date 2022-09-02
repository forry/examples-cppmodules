module;
#include <iostream>

export module mod2;
using namespace std;

namespace imp2
{
   export void mod_f(int i)
   {
      cout << "module 2\n";
   }
}