#ifndef YOBAPERL_PERL_STACK_HPP
#define YOBAPERL_PERL_STACK_HPP

#include "yobaperl/common.hpp"

namespace yoba {

class Perl;
class Scalar;
class Array;
class Code;

}

namespace yoba { namespace priv {



class PerlStack
{
public:
   PerlStack(Perl & perl);
   ~PerlStack();

   void extend(SSize_t size);

   void pushSV(SV * sv, bool extend);
   SV * popSV();

   void pushScalar(Scalar arg);
   void pushArray(Array args);
   Scalar popScalar();
   Array popArray();

   void call(Code code, I32 flags);
   void callMethod(Code code, I32 flags);
   void eval(const std::string & code, I32 flags);

private:
   Perl & _perl;
   PerlInterpreter * _interpreter = nullptr;
   SV ** sp = nullptr;

   I32 _returns_count = -1;
};



}} // namespace yoba::priv

#endif // YOBAPERL_PERL_STACK_HPP
