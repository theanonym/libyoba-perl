#ifndef YOBAPERL_SCALAR_HPP
#define YOBAPERL_SCALAR_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/variable.hpp"

namespace yoba {



//TODO sv_grow
//TODO sv_insert
////////////////////////////////////////////////////////////
/// \brief %Scalar reference
///
/// \cpp
/// Scalar scalar = perl.newScalar("str"); // (my) $scalar = "str"
/// scalar.setValue(14);                   // $scalar = 14
/// Scalar copy = scalar.makeCopy();       // $copy = $scalar
/// Scalar ref = scalar.makeRef();         // $ref = \$scalar
/// \endcpp
///
////////////////////////////////////////////////////////////
class YOBAPERL_EXPORT Scalar : public Variable
{
public:



   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   /// Use Perl::newScalar() instead
   ///
   ////////////////////////////////////////////////////////////
   Scalar(Perl & perl, SV * sv, bool increase_refcount);



   /// @{ \name Modify/convert value

   ////////////////////////////////////////////////////////////
   /// \brief Replace scalar value by string
   ///
   /// \perl $scalar = "string" \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar & setValue(const std::string & value);
   Scalar & setValue(const char * value);

   ////////////////////////////////////////////////////////////
   /// \brief Replace scalar value by integer
   ///
   /// \perl $scalar = 123 \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar & setValue(IV value);
   Scalar & setValue(int value);

   ////////////////////////////////////////////////////////////
   /// \brief Replace scalar value by unsigned integer
   ///
   /// \perl $scalar = 123 \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar & setValue(UV value);
   Scalar & setValue(unsigned value);

   ////////////////////////////////////////////////////////////
   /// \brief Replace scalar value by float number
   ///
   /// \perl $scalar = 12.34 \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar & setValue(NV value);
   Scalar & setValue(float value);

   ////////////////////////////////////////////////////////////
   /// \brief Replace scalar value by boolean
   ///
   /// \perl $scalar = !0 \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar & setValue(bool value);


   ////////////////////////////////////////////////////////////
   /// \brief Convert scalar value to C++ string
   ///
   /// \perl "$scalar" \endperl
   ///
   ////////////////////////////////////////////////////////////
   std::string toString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Convert scalar value to integer
   ///
   /// \perl int $scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   IV toIV() const;

   ////////////////////////////////////////////////////////////
   /// \brief Convert scalar value to unsigned integer
   ///
   /// \perl sprintf "%u", $scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   UV toUV() const;

   ////////////////////////////////////////////////////////////
   /// \brief Convert scalar value to float number
   ///
   /// \perl sprintf "%f", $scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   NV toNV() const;

   ////////////////////////////////////////////////////////////
   /// \brief Convert scalar value to boolean
   ///
   /// \perl !!$scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool toBool() const;

   ////////////////////////////////////////////////////////////
   /// \brief Dereference scalar
   ///
   /// \perl $$scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar unrefScalar() const;

   ////////////////////////////////////////////////////////////
   /// \brief Dereference array
   ///
   /// \perl @$scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   Array unrefArray() const;

   ////////////////////////////////////////////////////////////
   /// \brief Dereference hash
   ///
   /// \perl %$scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   Hash unrefHash() const;

   ////////////////////////////////////////////////////////////
   /// \brief Dereference code (without call)
   ///
   /// \perl &$scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   Code unrefCode() const;

   /// @}



   /// @{ \name Misc

   ////////////////////////////////////////////////////////////
   /// \brief Value length
   ///
   /// \perl length $scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   I32 getLength() const;

   ////////////////////////////////////////////////////////////
   /// \brief Create copy
   ///
   /// \perl $copy = $scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar makeCopy() const;

   ////////////////////////////////////////////////////////////
   /// \brief Take reference
   ///
   /// \perl $ref = \$scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar makeRef() const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar is a reference
   ///
   /// \perl ref $scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool isRef() const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar is a scalar reference
   ///
   /// \perl ref $scalar eq "SCALAR" \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool isScalarRef() const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar is an array reference
   ///
   /// \perl ref $scalar eq "ARRAY" \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool isArrayRef() const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar is a hash reference
   ///
   /// \perl ref $scalar eq "HASH" \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool isHashRef() const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar is a subroutine reference
   ///
   /// \perl ref $scalar eq "CODE" \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool isCodeRef() const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar is a object
   ///
   /// \perl Scalar::Util::blessed($scalar) \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool isObject() const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar is a class object
   ///
   /// \perl ref $scalar eq "Classname" \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool isObjectOf(const std::string & class_name) const;

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar is a valid number
   ///
   /// \note "Inf" and "Infinity"" are treated as numbers
   ///
   /// \perl Scalar::Util::looks_like_number($scalar) \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool isValidNumber() const;

   /// @}



   /// @{ \name Operators

   ////////////////////////////////////////////////////////////
   /// \brief Alias to setValue()
   ///
   /// \perl $scalar = value \endperl
   ///
   ////////////////////////////////////////////////////////////
   template<typename ValueT>
   Scalar & operator= (const ValueT & value);

   ////////////////////////////////////////////////////////////
   /// \brief Check if scalar not empty
   ///
   /// \perl $scalar && length $scalar \endperl
   ///
   ////////////////////////////////////////////////////////////
   operator bool() const;

   /// @}



};

#include "yobaperl/scalar.tpp"



} // namespace yoba

#endif // YOBAPERL_SCALAR_HPP
