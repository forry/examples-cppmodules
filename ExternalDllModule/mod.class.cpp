module;

#include <iostream>

module mod:SomeClassPart;

using namespace std;
using namespace imp;

void SomeClass::print() const
{
   cout << i << "\n";
}