export module mod:SomeClassPart;

export namespace imp
{
   class DLLMODULE_EXPORT SomeClass
   {
   public:
      int i = 34;
      void print() const;
   };
}

