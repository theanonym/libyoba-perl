#ifndef YOBAPERL_POINTER_HPP
#define YOBAPERL_POINTER_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/perl_exception.hpp"

namespace yoba {



////////////////////////////////////////////////////////////
/// \class Pointer
///
/// \brief shared_ptr with automatic dynamic cast
///
////////////////////////////////////////////////////////////
template<typename T>
class YOBAPERL_EXPORT Pointer : public std::shared_ptr<T>
{
public:
   Pointer()
      : std::shared_ptr<T>() {}

   Pointer(T * ptr)
      : std::shared_ptr<T>(ptr) {}

   Pointer(const std::shared_ptr<T> & to_copy)
      : std::shared_ptr<T>(to_copy) {}

   Pointer(std::shared_ptr<T> && to_move)
      : std::shared_ptr<T>(to_move) {}

   template<typename T2>
   operator Pointer<T2>()
   {
      auto result = std::dynamic_pointer_cast<T2>(*this);
      if(!result)
         throw std::runtime_error("Ptr cast error");
      return result;
   }
};



using VariablePtr = Pointer<class Variable>; ///< Type alias
using ScalarPtr   = Pointer<class Scalar>;   ///< Type alias
using ArrayPtr    = Pointer<class Array>;    ///< Type alias
using HashPtr     = Pointer<class Hash>;     ///< Type alias
using CodePtr     = Pointer<class Code>;     ///< Type alias



} // namespace yoba

#endif // YOBAPERL_POINTER_HPP
