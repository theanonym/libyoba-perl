#ifndef YOBAPERL_VARIABLE_HPP
#define YOBAPERL_VARIABLE_HPP

#include "yobaperl/common.hpp"

namespace yoba {

class Perl;
class Scalar;
class Array;
class Hash;
class Code;



class YOBAPERL_EXPORT Variable
{
public:
   Variable(Perl & perl, SV * sv, bool increase_refcount);
   Variable(const Variable & to_copy);
   Variable(Variable && to_move);
   virtual ~Variable();

   // Common
   virtual std::string toString() const = 0;
   void setReadOnly(bool state);
   bool isReadOnly() const;
   Perl & getPerl() const;

   // Internals
   SV * getSV() const;
   SV * detachSV();
   SV * detachMortalSV();
   U32 getRefcount() const;
   void increaseRefcount();
   void decreaseRefcount();
   void dump() const;

   // Operators
   bool operator== (const Variable & other);
   bool operator!= (const Variable & other);
   friend std::ostream & operator<< (std::ostream & stream, const Variable & var);

protected:
   Perl & _perl;
   PerlInterpreter * _interpreter = nullptr;
   SV * _sv = nullptr; // Can be SV, AV, HV, CV
};



} // namespace yoba

#endif // YOBAPERL_VARIABLE_HPP
