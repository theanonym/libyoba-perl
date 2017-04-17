#ifndef YOBAPERL_SCALAR_HPP
#define YOBAPERL_SCALAR_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/variable.hpp"

namespace yoba {



//TODO sv_grow
class YOBAPERL_EXPORT Scalar : public Variable
{
public:
   // Constructors
   Scalar(Perl & perl, SV * sv, bool increase_refcount);
//   Scalar(const Scalar & to_copy);
//   Scalar(Scalar && to_move);
//   ~Scalar();
   
   // Value manipulation
   Scalar & setValue(const std::string & value);
   Scalar & setValue(IV value);
   Scalar & setValue(UV value);
   Scalar & setValue(NV value);
   Scalar & setValue(bool value);
   Scalar & setValue(const char * value);
   Scalar & setValue(int value);
   Scalar & setValue(unsigned value);
   Scalar & setValue(float value);

   // Convert
   std::string toString() const;
   IV toIV() const;
   UV toUV() const;
   NV toNV() const;
   bool toBool() const;
   Scalar unrefScalar() const;
   Array unrefArray() const;
   Hash unrefHash() const;
   Code unrefCode() const;

   // Common
   I32 getLength() const;
   Scalar makeCopy() const;
   Scalar makeRef() const;
   bool isRef() const;
   bool isScalarRef() const;
   bool isArrayRef() const;
   bool isHashRef() const;
   bool isCodeRef() const;
   bool isObject() const;
   bool isObjectOf(const std::string & class_name) const;
   bool isValidNumber() const;

   // Operators
   template<typename ValueT>
   Scalar & operator= (const ValueT & value);
   operator bool() const;
};

#include "yobaperl/scalar.tpp"



} // namespace yoba

#endif // YOBAPERL_SCALAR_HPP
