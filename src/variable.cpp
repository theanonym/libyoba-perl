#include "yobaperl/variable.hpp"
#include "yobaperl/perl.hpp"

namespace yoba {



Variable::Variable(Perl & perl, SV * sv, bool increase_refcount)
    : _perl(perl),
      _interpreter(perl.getInterpreter()),
      _sv(sv)
{
   YOBAPERL_ASSERT(sv);
   if(increase_refcount)
      increaseRefcount();
}

Variable::Variable(const Variable & to_copy)
   : _perl(to_copy._perl),
     _interpreter(to_copy._interpreter),
     _sv(to_copy._sv)
{
   YOBAPERL_ASSERT(to_copy._sv);
   increaseRefcount();
}

Variable::Variable(Variable && to_move)
   : _perl(to_move._perl),
     _interpreter(to_move._interpreter),
     _sv(to_move._sv)
{
   YOBAPERL_ASSERT(to_move._sv);
   to_move._sv = nullptr; // Skip destructor
}

Variable::~Variable()
{
   if(_sv)
   {
      decreaseRefcount();
   }
}



Perl & Variable::getPerl() const
{
   return _perl;
}

void Variable::setReadOnly(bool state)
{
   state ? SvREADONLY_on(getSV()) : SvREADONLY_off(getSV());
}

bool Variable::isReadOnly() const
{
   return SvREADONLY(getSV());
}



SV * Variable::getSV() const
{
   YOBAPERL_ASSERT(_sv);
   YOBAPERL_ASSERT(_sv != &PL_sv_undef);
   return _sv;
}

SV * Variable::detachSV()
{
   SV * sv = getSV();
   _sv = nullptr;
   return sv;
}

SV * Variable::detachMortalSV()
{
   SV * sv = detachSV();
   return sv_2mortal(sv);
}

U32 Variable::getRefcount() const
{
   return SvREFCNT(getSV());
}

void Variable::increaseRefcount()
{
   SvREFCNT_inc_NN(getSV());
}

void Variable::decreaseRefcount()
{
   SvREFCNT_dec_NN(getSV());
}

void Variable::dump() const
{
   sv_dump(getSV());
}



bool Variable::operator== (const Variable & other)
{
   return getSV() == other.getSV();
}

bool Variable::operator!= (const Variable & other)
{
   return !(*this == other);
}

std::ostream & operator<< (std::ostream & stream, const Variable & var)
{
   return stream << var.toString();
}



} // namespace yoba
