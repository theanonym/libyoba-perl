#ifndef YOBAPERL_CODE_HPP
#define YOBAPERL_CODE_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/variable.hpp"

namespace yoba {



class YOBAPERL_EXPORT Code : public Variable
{
public:
   // Constructors
   Code(Perl & perl, CV * cv, bool increase_refcount);

   // Common
   std::string toString() const;
   Scalar makeRef() const;

   // Internals
   CV * getCV() const;
};



} // namespace yoba

#endif // YOBAPERL_CODE_HPP
