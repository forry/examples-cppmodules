module;
#include <iostream>

export module mod;
using namespace std; 

namespace imp
{
   export void mod_f(int i)
   {
      cout << "module 1\n";
   }
}