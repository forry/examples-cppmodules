export module mod;

export import mod.sub1;
export import mod.sub2;

import std.core;

using namespace std;

namespace mod
{
   export void printModules()
   {
      cout << "mod.sub1\nmod.sub2\n";
   }
}