#ifndef YOBAPERL_EXCEPTION_HPP
#define YOBAPERL_EXCEPTION_HPP

#include "yobaperl/common.hpp"
#include "array.hpp"

#include <exception>

namespace yoba {



class YOBAPERL_EXPORT PerlException : public std::exception
{
   friend class Perl;

public:
   // Constructors
   PerlException(int perl_id, std::string info, std::string eval_sting = "");
   
   // Common
   const std::string & getInfo() const;
   bool isEval() const;
   const std::string & getEvalString() const;
   int getPerlId() const;

   const char * what() const noexcept;

private:
   int _perl_id;
   std::string _eval_string;
   std::string _info;
};



} // namespace yoba

#endif // YOBAPERL_EXCEPTION_HPP
