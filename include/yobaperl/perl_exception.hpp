#ifndef YOBAPERL_EXCEPTION_HPP
#define YOBAPERL_EXCEPTION_HPP

#include "yobaperl/common.hpp"
#include "array.hpp"

#include <exception>

namespace yoba {



////////////////////////////////////////////////////////////
/// \brief Perl exception
///
////////////////////////////////////////////////////////////
class YOBAPERL_EXPORT PerlException : public std::exception
{
   friend class Perl;

public:
   PerlException(int perl_id, std::string info, std::string eval_sting = "");
   


   ////////////////////////////////////////////////////////////
   /// \brief Error string
   ///
   ////////////////////////////////////////////////////////////
   const std::string & getInfo() const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if exception thrown in Perl::eval()
   ///
   ////////////////////////////////////////////////////////////
   bool isEval() const;

   ////////////////////////////////////////////////////////////
   /// \brief Code string passed to Perl::eval()
   ///
   ////////////////////////////////////////////////////////////
   const std::string & getEvalString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Perl object id
   ///
   ////////////////////////////////////////////////////////////
   int getPerlId() const;



   const char * what() const noexcept;

private:
   int _perl_id;
   std::string _eval_string;
   std::string _info;
};



} // namespace yoba

#endif // YOBAPERL_EXCEPTION_HPP
