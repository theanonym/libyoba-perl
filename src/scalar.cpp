#include "yobaperl/scalar.hpp"
#include "yobaperl/array.hpp"
#include "yobaperl/hash.hpp"
#include "yobaperl/code.hpp"
#include "yobaperl/perl.hpp"

namespace yoba {



Scalar::Scalar(Perl & perl, SV * sv, bool increase_refcount)
   : Variable(perl, sv, increase_refcount)
{
}



Scalar & Scalar::setValue(const std::string & value)
{
   sv_setpvn(getSV(), value.c_str(), value.length());
   return *this;
}

Scalar & Scalar::setValue(IV value)
{
   sv_setiv(getSV(), value);
   return *this;
}

Scalar & Scalar::setValue(UV value)
{
   sv_setuv(getSV(), value);
   return *this;
}

Scalar & Scalar::setValue(NV value)
{
   sv_setnv(getSV(), value);
   return *this;
}

Scalar & Scalar::setValue(bool value)
{
   sv_setiv(getSV(), value ? 1 : 0);
   return *this;
}

Scalar & Scalar::setValue(const char * value)
{
   setValue(std::string(value));
   return *this;
}

Scalar & Scalar::setValue(int value)
{
   setValue(static_cast<IV>(value));
   return *this;
}

Scalar & Scalar::setValue(unsigned value)
{
   setValue(static_cast<UV>(value));
   return *this;
}

Scalar & Scalar::setValue(float value)
{
   setValue(static_cast<NV>(value));
   return *this;
}



std::string Scalar::toString() const
{
   STRLEN len;
   const char * ptr = SvPVx(const_cast<SV *>(getSV()), len);

   return std::string(ptr, len);
}

IV Scalar::toIV() const
{
   return SvIVx(getSV());
}

UV Scalar::toUV() const
{
   return SvUVx(getSV());
}

NV Scalar::toNV() const
{
   return SvNVx(getSV());
}

bool Scalar::toBool() const
{
   return SvTRUEx(getSV());
}

Scalar Scalar::unrefScalar() const
{
   if(isScalarRef())
   {
      return Scalar(_perl, SvRVx(getSV()), true);
   }
   else
   {
      std::string error = "Not a scalar reference (" + toString().substr(0, 10) + ")";

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

      if(_perl.isWarningsEnabled())
         warn("%s", error.c_str());

      return _perl.newScalar();
   }
}

Array Scalar::unrefArray() const
{
   if(isArrayRef())
   {
      return Array(_perl, MUTABLE_AV(SvRVx(getSV())), true);
   }
   else
   {
      std::string error = "Not an array reference (" + toString().substr(0, 10) + ")";

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

      if(_perl.isWarningsEnabled())
         warn("%s", error.c_str());

      return _perl.newArray();
   }
}

Hash Scalar::unrefHash() const
{
   if(isHashRef())
   {
      return Hash(_perl, MUTABLE_HV(SvRVx(getSV())), true);
   }
   else
   {
      std::string error = "Not a hash reference (" + toString().substr(0, 10) + ")";

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

      if(_perl.isWarningsEnabled())
         warn("%s", error.c_str());

      return _perl.newHash();
   }
}

Code Scalar::unrefCode() const
{
   if(isCodeRef())
   {
      return Code(_perl, MUTABLE_CV(SvRVx(getSV())), true);
   }
   else
   {
      std::string error = "Not a code reference (" + toString().substr(0, 10) + ")";

//      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

//      if(_perl.isWarningsEnabled())
//         warn("%s", error.c_str());
   }
}



I32 Scalar::getLength() const
{
   return sv_len(getSV());
}

Scalar Scalar::makeCopy() const
{
   return Scalar(_perl, newSVsv(_sv), false);
}

Scalar Scalar::makeRef() const
{
   return Scalar(_perl, newRV_inc(getSV()), false);
}

bool Scalar::isRef() const
{
   return SvROK(getSV());
}

bool Scalar::isScalarRef() const
{
   return SvTYPE(SvRVx(getSV())) < SVt_PVAV;
}

bool Scalar::isArrayRef() const
{
   return SvTYPE(SvRVx(getSV())) == SVt_PVAV;
}

bool Scalar::isHashRef() const
{
   return SvTYPE(SvRVx(getSV())) == SVt_PVHV;
}

bool Scalar::isCodeRef() const
{
   return SvTYPE(SvRVx(getSV())) == SVt_PVCV;
}

bool Scalar::isObject() const
{
   return sv_isobject(getSV());
}

bool Scalar::isObjectOf(const std::string & class_name) const
{
   return sv_isa(getSV(), class_name.c_str());
}

bool Scalar::isValidNumber() const
{
   return looks_like_number(getSV());
}



yoba::Scalar::operator bool() const
{
   return getLength() != 0;
}



} // namespace yoba
