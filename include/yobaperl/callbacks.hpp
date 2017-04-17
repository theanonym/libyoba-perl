#ifndef YOBAPERL_CALLBACKS_HPP
#define YOBAPERL_CALLBACKS_HPP

#include "yobaperl/common.hpp"

#include <unordered_map>
#include <functional>

namespace yoba {

class Perl;
class Scalar;

}

namespace yoba { namespace priv {



class YOBAPERL_EXPORT Callbacks
{
public:
   enum Context
   {
      VoidToVoid,
      ScalarToVoid,
      VoidToScalar,
      ScalarToScalar
   };

   using VoidToVoidCB     = std::function<void(void)>;
   using ScalarToVoidCB   = std::function<void(Scalar)>;
   using VoidToScalarCB   = std::function<Scalar(void)>;
   using ScalarToScalarCB = std::function<Scalar(Scalar)>;

   Callbacks(Perl & perl);
   Callbacks(const Callbacks & to_copy);
   Callbacks(Callbacks && to_move);

   void setVoidToVoid(const std::string & name, VoidToVoidCB function);
   void setScalarToVoid(const std::string & name, ScalarToVoidCB function);
   void setVoidToScalar(const std::string & name, VoidToScalarCB function);
   void setScalarToScalar(const std::string & name, ScalarToScalarCB function);
   VoidToVoidCB getVoidToVoid(const std::string & name);
   ScalarToVoidCB getScalarToVoid(const std::string & name);
   VoidToScalarCB getVoidToScalar(const std::string & name);
   ScalarToScalarCB getScalarToScalar(const std::string & name);

private:
   Perl & _perl;

   std::unordered_map<std::string, VoidToVoidCB>     _callbacks_void_to_void;
   std::unordered_map<std::string, ScalarToVoidCB>   _callbacks_scalar_to_void;
   std::unordered_map<std::string, VoidToScalarCB>   _callbacks_void_to_scalar;
   std::unordered_map<std::string, ScalarToScalarCB> _callbacks_scalar_to_scalar;
};



}} // namespace yoba::priv

#endif // YOBAPERL_CALLBACKS_HPP
