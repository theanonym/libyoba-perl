#include "yobaperl/perl_exception.hpp"

namespace yoba {



PerlException::PerlException(int perl_id, std::string info, std::string eval_sting)
   : _perl_id(perl_id),
     _eval_string(eval_sting)
{
   std::stringstream ss;

#ifdef YOBAPERL_MULTIPLICITY
   ss << "Perl #" << perl_id << ": ";
#endif

   ss << info;
   _info = ss.str();
}



const std:: string & PerlException::getInfo() const
{
   return _info;
}

bool PerlException::isEval() const
{
   return !_eval_string.empty();
}

const std::string & PerlException::getEvalString() const
{
   return _eval_string;
}

int PerlException::getPerlId() const
{
   return _perl_id;
}



const char * PerlException::what() const noexcept
{
   return _info.c_str();
}



} // namespace yoba
