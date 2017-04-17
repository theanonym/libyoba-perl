#ifndef YOBAPERL_PTR_HPP
#define YOBAPERL_PTR_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/perl_exception.hpp"

namespace yoba {



template<typename T>
class YOBAPERL_EXPORT Ptr : public std::shared_ptr<T>
{
public:
   Ptr()
      : std::shared_ptr<T>() {}

   Ptr(T * ptr)
      : std::shared_ptr<T>(ptr) {}

   Ptr(const std::shared_ptr<T> & to_copy)
      : std::shared_ptr<T>(to_copy) {}

   Ptr(std::shared_ptr<T> && to_move)
      : std::shared_ptr<T>(to_move) {}

   template<typename T2>
   operator Ptr<T2>()
   {
      auto result = std::dynamic_pointer_cast<T2>(*this);
      if(!result)
         throw std::runtime_error("Ptr cast error");
      return result;
   }
};



using VariablePtr = Ptr<class Variable>;
using ScalarPtr   = Ptr<class Scalar>;
using ArrayPtr    = Ptr<class Array>;
using HashPtr     = Ptr<class Hash>;
using CodePtr     = Ptr<class Code>;



} // namespace yoba

#endif // YOBAPERL_SCALAR_HPP
