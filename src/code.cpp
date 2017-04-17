#include "yobaperl/code.hpp"
#include "yobaperl/perl.hpp"

namespace yoba {



Code::Code(Perl & perl, CV * cv, bool increase_refcount)
   : Variable(perl, reinterpret_cast<SV *>(cv), increase_refcount)
{
}



std::string Code::toString() const
{
   return getPerl().deparse(*this);
}

Scalar Code::makeRef() const
{
   return Scalar(_perl, newRV_inc(getSV()), false);
}



CV * Code::getCV() const
{
   return reinterpret_cast<CV *>(getSV());
}



} // namespace yoba
