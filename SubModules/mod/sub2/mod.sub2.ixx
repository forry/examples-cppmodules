export module mod.sub2;

export namespace mod::sub2
{
   int g();
   int h();
}

// Beginning of the private module fragment
module :private;

namespace mod::sub2
{
   int g()
   {
      return 904;
   }
}

int mod::sub2::h()
{
   return 589;
}