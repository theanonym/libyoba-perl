#include "yobaperl/callbacks.hpp"
#include "yobaperl/perl.hpp"
#include "yobaperl/perl_exception.hpp"

namespace yoba { namespace priv {



Callbacks::Callbacks(Perl & perl)
   : _perl(perl)
{
}

Callbacks::Callbacks(const Callbacks & to_copy)
   : _perl(to_copy._perl),
     _callbacks_void_to_void(to_copy._callbacks_void_to_void),
     _callbacks_scalar_to_void(to_copy._callbacks_scalar_to_void),
     _callbacks_void_to_scalar(to_copy._callbacks_void_to_scalar),
     _callbacks_scalar_to_scalar(to_copy._callbacks_scalar_to_scalar)
{
}

Callbacks::Callbacks(Callbacks && to_move)
   : _perl(to_move._perl),
     _callbacks_void_to_void(std::move(to_move._callbacks_void_to_void)),
     _callbacks_scalar_to_void(std::move(to_move._callbacks_scalar_to_void)),
     _callbacks_void_to_scalar(std::move(to_move._callbacks_void_to_scalar)),
     _callbacks_scalar_to_scalar(std::move(to_move._callbacks_scalar_to_scalar))
{
}

void Callbacks::setVoidToVoid(const std::string & name, Callbacks::VoidToVoidCB function)
{
   _callbacks_void_to_void.insert({ name, function });
}

void Callbacks::setScalarToVoid(const std::string & name, Callbacks::ScalarToVoidCB function)
{
   _callbacks_scalar_to_void.insert({ name, function });
}

void Callbacks::setVoidToScalar(const std::string & name, Callbacks::VoidToScalarCB function)
{
   _callbacks_void_to_scalar.insert({ name, function });
}

void Callbacks::setScalarToScalar(const std::string & name, Callbacks::ScalarToScalarCB function)
{
   _callbacks_scalar_to_scalar.insert({ name, function });
}

Callbacks::VoidToVoidCB Callbacks::getVoidToVoid(const std::string & name)
{
   auto it = _callbacks_void_to_void.find(name);
   if(it == _callbacks_void_to_void.end())
      throw PerlException(_perl.getId(), "Callback not registered or wrong context (void -> void)");

   return it->second;
}

Callbacks::ScalarToVoidCB Callbacks::getScalarToVoid(const std::string & name)
{
   auto it = _callbacks_scalar_to_void.find(name);
   if(it == _callbacks_scalar_to_void.end())
      throw PerlException(_perl.getId(), "Callback not registered or wrong context (Scalar -> void)");

   return it->second;
}

Callbacks::VoidToScalarCB Callbacks::getVoidToScalar(const std::string & name)
{
   auto it = _callbacks_void_to_scalar.find(name);
   if(it == _callbacks_void_to_scalar.end())
      throw PerlException(_perl.getId(), "Callback not registered or wrong context (void -> Scalar)");

   return it->second;
}

Callbacks::ScalarToScalarCB Callbacks::getScalarToScalar(const std::string & name)
{
   auto it = _callbacks_scalar_to_scalar.find(name);
   if(it == _callbacks_scalar_to_scalar.end())
      throw PerlException(_perl.getId(), "Callback not registered or wrong context (Scalar -> Scalar)");

   return it->second;
}



}} // namespace yoba::priv
