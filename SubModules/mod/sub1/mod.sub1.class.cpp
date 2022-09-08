module;

#include <iostream>

module mod.sub1:SomeClassPart;

using namespace std;
using namespace imp;

void SomeClass::print() const
{
   cout << i << "\n";
}