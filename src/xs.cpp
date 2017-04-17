#include "yobaperl/perl.hpp"
#include "yobaperl/perl_stack.hpp"

using namespace yoba;
using namespace yoba::priv;

#include <XSUB.h>
#include "ppport.h"


EXTERN_C void boot_DynaLoader (pTHX_ CV * cv);
EXTERN_C void XS_YobaPerl_call(pTHX_ CV * cv);

EXTERN_C void xs_init(pTHX)
{
   dXSUB_SYS;
   PERL_UNUSED_CONTEXT;
   
   newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, __FILE__);
   newXS("YobaPerl::call", XS_YobaPerl_call, __FILE__);
}

Callbacks::Context determineContext(const U32 gimme, const bool has_param)
{
   switch(gimme)
   {
      case G_VOID:  return has_param ? Callbacks::ScalarToVoid : Callbacks::VoidToVoid;
      case G_SCALAR:
      case G_ARRAY: return has_param ? Callbacks::ScalarToScalar : Callbacks::VoidToScalar;

      default: return Callbacks::VoidToVoid; // -Wreturn-type
   }
}

extern void XS_YobaPerl_call(PerlInterpreter * my_perl, CV * cv)
{
   dVAR;
   dXSARGS;

   Perl & perl = Perl::getInstance(my_perl);

   if(items == 0)
   {
      if(perl.isExceptionsEnabled())
         throw PerlException(perl.getId(), "YobaPerl::call(): Missing callback name");

      if(perl.isWarningsEnabled())
         warn("YobaPerl::call(): Missing callback name");

      XSRETURN_EMPTY;
   }

   STRLEN len;
   const char * ptr = SvPVx(ST(0), len);
   std::string callback_name(ptr, len);
   
   switch(determineContext(GIMME_V, items > 1))
   {
      case Callbacks::VoidToVoid:
      {
         perl.getCallbacks().getVoidToVoid(callback_name)();
         XSRETURN_EMPTY;
      }

      case Callbacks::ScalarToVoid:
      {
         perl.getCallbacks().getScalarToVoid(callback_name)(Scalar(perl, ST(1), true));
         XSRETURN_EMPTY;
      }

      case Callbacks::VoidToScalar:
      {
         ST(0) = perl.getCallbacks().getVoidToScalar(callback_name)().detachSV();
         XSRETURN(1);
      }

      case Callbacks::ScalarToScalar:
      {
         ST(0) = perl.getCallbacks().getScalarToScalar(callback_name)(Scalar(perl, ST(1), true)).detachSV();
         XSRETURN(1);
      }
   }
}
