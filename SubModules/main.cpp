import std.core;

// You can import the submodules or the whole module

//import mod.sub2;
//import mod.sub1;
import mod;

using namespace std;

using mod::sub2::h;

int main()
{
   // mod
   mod::printModules();

   // mod.sub2
   cout << mod::sub2::g() << "\n";
   cout << h() << "\n";

   // mod.sub1
   imp::SomeClass cl;
   imp::print(cl);
   return 0;
}