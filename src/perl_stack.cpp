#include "yobaperl/perl_stack.hpp"
#include "yobaperl/perl.hpp"

namespace yoba { namespace priv {



PerlStack::PerlStack(Perl & perl)
   : _perl(perl),
     _interpreter(perl.getInterpreter()),
     sp(PL_stack_sp)
{
   ENTER;
   SAVETMPS;
   PUSHMARK(SP);
}

PerlStack::~PerlStack()
{
   PUTBACK;
   FREETMPS;
   LEAVE;
}



void PerlStack::extend(ssize_t size)
{
   EXTEND(SP, size);
}

void PerlStack::pushSV(SV * sv, bool extend)
{
   if(extend)
      XPUSHs(sv);
   else
      PUSHs(sv);
}

SV * PerlStack::popSV()
{
   return POPs;
}

void PerlStack::pushScalar(Scalar arg)
{
   pushSV(arg.detachMortalSV(), /*extend*/true);
}

void PerlStack::pushArray(Array args)
{
   extend(args.getSize());
   for(int i = 0; i < args.getSize(); i++)
      pushSV(args[i].detachMortalSV(), /*extend*/false);
}

Scalar PerlStack::popScalar()
{
   YOBAPERL_ASSERT(_returns_count == 1);
   return Scalar(_perl, popSV(), /*ref++*/true);
}

Array PerlStack::popArray()
{
   YOBAPERL_ASSERT(_returns_count != -1);

   AV * result = av_make(_returns_count, SP - _returns_count + 1);
   SP -= _returns_count;
   return Array(_perl, result, /*ref++*/false);
}

void PerlStack::call(Code code, I32 flags)
{
   PUTBACK;
   _returns_count = call_sv(code.detachSV(), flags);
   SPAGAIN;

   if(_perl.hasError())
   {
      if(_returns_count > 0)
         POPs;

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), _perl.getError());

      if(_perl.isWarningsEnabled())
         warn("%s", _perl.getError().c_str());
   }
}

void PerlStack::callMethod(Code code, I32 flags)
{
   PUTBACK;
//   _returns_count = call_method(sub_name.c_str(), flags);


   _returns_count = call_sv(code.detachSV(), flags | G_METHOD);
   SPAGAIN;

   if(_perl.hasError())
   {
      if(_returns_count > 0)
         POPs;

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), _perl.getError());

      if(_perl.isWarningsEnabled())
         warn("%s", _perl.getError().c_str());
   }
}

void PerlStack::eval(const std::string & code, I32 flags)
{
   // Prevent segfault
#ifdef YOBAPERL_MULTIPLICITY
   _perl.setContext();
#endif

   // YobaPerl::call() may throw exceptions inside eval
   try
   {
      _returns_count = eval_sv(_perl.newScalar(code).detachMortalSV(), flags);
      SPAGAIN;
   }
   catch(const PerlException & e)
   {
      // Add code string and rethrow
      throw PerlException(e.getPerlId(), e.getInfo(), code);
   }

   if(_perl.hasError())
   {
      if(_returns_count > 0)
         POPs;

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), _perl.getError(), code);

      if(_perl.isWarningsEnabled())
         warn("%s\nCode:\n%s", _perl.getError().c_str(), code.c_str());
   }
}



}} // namespace yoba::priv
