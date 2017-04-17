// Inline templates for scalar.hpp

template<typename ValueT>
Scalar & Scalar::operator= (const ValueT & value)
{
   setValue(value);
   return *this;
}
