#ifndef YOBAPERL_PERL_HPP
#define YOBAPERL_PERL_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/variable.hpp"
#include "yobaperl/scalar.hpp"
#include "yobaperl/array.hpp"
#include "yobaperl/hash.hpp"
#include "yobaperl/code.hpp"
#include "yobaperl/perl_exception.hpp"
#include "yobaperl/perl_stack.hpp"
#include "yobaperl/callbacks.hpp"
#include "yobaperl/pointer.hpp"

namespace yoba {



////////////////////////////////////////////////////////////
/// \mainpage
///
/// Project site: https://github.com/theanonym/libyoba-perl
////////////////////////////////////////////////////////////



//TODO perl_clone
////////////////////////////////////////////////////////////
/// \brief Main class
///
////////////////////////////////////////////////////////////
class YOBAPERL_EXPORT Perl
{
   friend class Scalar;
   friend class Array;
   friend class Hash;

public:



   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ///
   /// \param do_init Calls init()
   ///
   ////////////////////////////////////////////////////////////
   Perl(bool do_init = true);

   ////////////////////////////////////////////////////////////
   /// \brief Move constructor
   ///
   ////////////////////////////////////////////////////////////
   Perl(Perl && to_move);

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ///
   ////////////////////////////////////////////////////////////
   ~Perl();



   /// @{ \name Creating %Perl variables

   ////////////////////////////////////////////////////////////
   /// \brief Create anonymous empty scalar
   ///
   ////////////////////////////////////////////////////////////
   Scalar newScalar();

   ////////////////////////////////////////////////////////////
   /// \brief Create empty scalar as \c \$name
   ///
   /// \perl $scalar = "" \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar newNamedScalar(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Create anonymous scalar and set value
   ///
   /// \see Scalar::setValue()
   ///
   ////////////////////////////////////////////////////////////
   template<typename ValueT>
   Scalar newScalar(const ValueT & value);

   ////////////////////////////////////////////////////////////
   /// \brief Create scalar as \c \$name and set value
   ///
   /// \perl $scalar = value \endperl
   ///
   /// \see Scalar::setValue()
   ///
   ////////////////////////////////////////////////////////////
   template<typename ValueT> Scalar newNamedScalar(const std::string & name, const ValueT & value);

   ////////////////////////////////////////////////////////////
   /// \brief Create anonymous empty array
   ///
   /// \perl () \endperl
   ///
   ////////////////////////////////////////////////////////////
   Array newArray();

   ////////////////////////////////////////////////////////////
   /// \brief Create anonymous array fill with integers
   ///
   /// \perl (begin .. end) \endperl
   ///
   ////////////////////////////////////////////////////////////
   Array newArray(IV begin, IV end);

   ////////////////////////////////////////////////////////////
   /// \brief Create empty array as \c \@name
   ///
   /// \perl @array = () \endperl
   ///
   ////////////////////////////////////////////////////////////
   Array newNamedArray(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Create array as \c \@name array fill with integers
   ///
   /// \perl $array = (begin .. end) \endperl
   ///
   ////////////////////////////////////////////////////////////
   Array newNamedArray(const std::string & name, IV begin, IV end);

   ////////////////////////////////////////////////////////////
   /// \brief Create anonymous array and fill
   ///
   /// \perl (values) \endperl
   ///
   /// \see Scalar::setValue()
   ///
   ////////////////////////////////////////////////////////////
   template<typename ValueT>
   Array newArray(const std::vector<ValueT> & values);
   template<typename ValueT>
   Array newArray(const std::initializer_list<ValueT> & values);

   ////////////////////////////////////////////////////////////
   /// \brief Create array as \c \@name and fill
   ///
   /// \perl $array = (values) \endperl
   ///
   /// \see Scalar::setValue()
   ///
   ////////////////////////////////////////////////////////////
   template<typename ValueT>
   Array newNamedArray(const std::string & name, const std::vector<ValueT> & values);
   template<typename ValueT>
   Array newNamedArray(const std::string & name, const std::initializer_list<ValueT> & values);

   ////////////////////////////////////////////////////////////
   /// \brief Create anonymous empty hash
   ///
   /// \perl () \endperl
   ///
   ////////////////////////////////////////////////////////////
   Hash newHash();

   ////////////////////////////////////////////////////////////
   /// \brief Create empty hash as \c \%name
   ///
   /// \perl %hash = () \endperl
   ///
   ////////////////////////////////////////////////////////////
   Hash newNamedHash(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Create anonymous hash and fill
   ///
   /// \perl (key => value) \endperl
   ///
   ////////////////////////////////////////////////////////////
   template<typename ValueT>
   Hash newHash(const std::unordered_map<std::string, ValueT> & map);

   ////////////////////////////////////////////////////////////
   /// \brief Create hash as \c \%name and fill
   ///
   /// \perl %hash = (key => value) \endperl
   ///
   ////////////////////////////////////////////////////////////
   template<typename ValueT>
   Hash newNamedHash(const std::string & name, const std::unordered_map<std::string, ValueT> & map);

   ////////////////////////////////////////////////////////////
   /// \brief Create new object
   ///
   /// \perl Classname->new() \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar newObject(const std::string & class_name);

   ////////////////////////////////////////////////////////////
   /// \brief Create new object
   ///
   /// \perl Classname->new($param) \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar newObject(const std::string & class_name, const Scalar & param);

   ////////////////////////////////////////////////////////////
   /// \brief Create new object
   ///
   /// \perl Classname->new(@params) \endperl
   ///
   ////////////////////////////////////////////////////////////
   Scalar newObject(const std::string & class_name, const Array & params);

   /// @}



   /// @{ \name Getting %Perl variables

   ////////////////////////////////////////////////////////////
   /// \brief Get scalar by name
   ///
   /// \perl $name \endperl
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   Scalar getScalar(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Get array by name
   ///
   /// \perl @name \endperl
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   Array getArray(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Get hash by name
   ///
   /// \perl %name \endperl
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   Hash getHash(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Get subroutine by name
   ///
   /// \perl &name \endperl
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   Code getSubroutine(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Get class method by name
   ///
   /// \perl &Na::me \endperl
   ///
   ////////////////////////////////////////////////////////////
   Code getMethod(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Place scalar reference in heap
   ///
   /// \see Pointer
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   ScalarPtr getScalarPtr(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Place array reference in heap
   ///
   /// \see Pointer
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   ArrayPtr getArrayPtr(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Place hash reference in heap
   ///
   /// \see Pointer
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   HashPtr getHashPtr(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Place subroutine reference in heap
   ///
   /// \see Pointer
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   CodePtr getSubroutinePtr(const std::string & name);
   
   /// @}



   /// @{ \name Call %Perl code

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate string in generic context
   ///
   /// \tparam ReturnT
   ///    \li void (default)
   ///    \li Scalar
   ///    \li Array
   ///
   /// \cpp
   /// eval("code") // eval("code")
   ///
   /// Scalar result = eval<Scalar>("code") // $result = eval("code")
   ///
   /// Array result = eval<Array>("code")   // @result = eval("code")
   /// \endcpp
   ///
   /// \exception PerlException Syntax error or die()
   ///
   ////////////////////////////////////////////////////////////
   template<typename ReturnT = void>
   ReturnT eval(const std::string & code);

   ////////////////////////////////////////////////////////////
   /// \brief Call subroutine in generic context
   ///
   /// \tparam ReturnT
   ///    \li void (default)
   ///    \li Scalar
   ///    \li Array
   ///
   /// \tparam ParamT
   ///    \li Scalar
   ///    \li Array
   ///
   /// \cpp
   /// call("sub_name");         // sub_name()
   /// call("sub_name", Scalar); // sub_name($param)
   /// call("sub_name", Array);  // sub_name(@params)
   ///
   /// Scalar result = call<Scalar>("sub_name");                 // $result = sub_name()
   /// Scalar result = call<Scalar, Scalar>("sub_name", Scalar); // $result = sub_name($param)
   /// Scalar result = call<Scalar, Array> ("sub_name", Array);  // $result = sub_name(@params)
   ///
   /// Array result = call<Array>("sub_name");                 // $result = sub_name()
   /// Array result = call<Array, Scalar>("sub_name", Scalar); // $result = sub_name($param)
   /// Array result = call<Array, Array>("sub_name", Array);   // $result = sub_name(@params)
   /// \endcpp
   ///
   /// \exception PerlException Undefined subroutine
   /// \exception PerlException Syntax error or die()
   ///
   ////////////////////////////////////////////////////////////
   template<typename ReturnT, typename ParamT>
   ReturnT call(const std::string & sub_name, const ParamT & param);
   template<typename ReturnT = void>
   ReturnT call(const std::string & sub_name);
   template<typename ReturnT, typename ParamT>
   ReturnT call(const Code & subroutine, const ParamT & param);
   template<typename ReturnT = void>
   ReturnT call(const Code & subroutine);


   ////////////////////////////////////////////////////////////
   /// \brief Call method in generic context
   ///
   /// \see call()
   ///
   /// \exception PerlException Undefined subroutine
   /// \exception PerlException Syntax error or die()
   ///
   ////////////////////////////////////////////////////////////
   template<typename ReturnT, typename ParamT>
   ReturnT callMethod(const std::string & sub_name, const Scalar & object, const ParamT & param);
   template<typename ReturnT = void>
   ReturnT callMethod(const std::string & sub_name, const Scalar & object);
   template<typename ReturnT, typename ParamT>
   ReturnT callMethod(const Code & subroutine, const Scalar & object, const ParamT & param);
   template<typename ReturnT = void>
   ReturnT callMethod(const Code & subroutine, const Scalar & object);

   /// @}



   /// @{ \name C++ callbacks

   Perl & registerVoidToVoid(const std::string & name, priv::Callbacks::VoidToVoidCB function);
   Perl & registerVoidToScalar(const std::string & name, priv::Callbacks::VoidToScalarCB function);
   Perl & registerScalarToVoid(const std::string & name, priv::Callbacks::ScalarToVoidCB function);
   Perl & registerScalarToScalar(const std::string & name, priv::Callbacks::ScalarToScalarCB function);
   CV * registerStatic(const std::string & sub_name, XSUBADDR_t function);

   /// @}



   /// @{ \name Misc

   ////////////////////////////////////////////////////////////
   /// \brief Initialize the %Perl interpreter
   ///
   /// \exception PerlException Initialization error
   ///
   ////////////////////////////////////////////////////////////
   Perl & init();

   ////////////////////////////////////////////////////////////
   /// \brief Add folder to module search path
   ///
   /// \perl use lib 'path' \endperl
   ///
   ////////////////////////////////////////////////////////////
   Perl & lib(const std::string & path);

   ////////////////////////////////////////////////////////////
   /// \brief Import a module
   ///
   /// \perl use Module \endperl
   ///
   /// \exception PerlException Syntax error or die()
   ///
   ////////////////////////////////////////////////////////////
   Perl & use(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Unload a module
   ///
   /// \perl no Module \endperl
   ///
   ////////////////////////////////////////////////////////////
   Perl & no(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Load external file
   ///
   /// \perl
   /// require Module
   ///
   /// require 'file.pl'
   /// \endperl
   ///
   /// \exception PerlException Syntax error or die()
   ///
   ////////////////////////////////////////////////////////////
   Perl & require(const std::string & name);

   ////////////////////////////////////////////////////////////
   /// \brief Enable/disable all exceptions. Default: \b enabled
   ///
   ////////////////////////////////////////////////////////////
   Perl & setExceptionsEnabled(bool state);

   ////////////////////////////////////////////////////////////
   /// \brief Enable/disable all warnings. Default: \b disabled
   ///
   ////////////////////////////////////////////////////////////
   Perl & setWarningsEnabled(bool state);

   ////////////////////////////////////////////////////////////
   /// \brief Check %Perl error
   ///
   /// \perl $@ \endperl
   ///
   ////////////////////////////////////////////////////////////
   bool hasError() const;
   std::string getError() const;

   /// @}



   /// @{ \name Internals

   ////////////////////////////////////////////////////////////
   /// \brief Perl object id
   ///
   /// Starts from 0
   ///
   ////////////////////////////////////////////////////////////
   int getId() const;

   ////////////////////////////////////////////////////////////
   /// \brief Raw interpreter
   ///
   ////////////////////////////////////////////////////////////
   PerlInterpreter * getInterpreter() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get raw scalar by name
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   SV * getNamedSV(const std::string & name) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get raw array by name
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   AV * getNamedAV(const std::string & name) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get raw hash by name
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   HV * getNamedHV(const std::string & name) const;

   ////////////////////////////////////////////////////////////
   /// \brief Get raw subroutine by name
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   CV * getNamedCV(const std::string & name) const;

   ////////////////////////////////////////////////////////////
   /// \brief Convert subroutine to string using B::Deparse
   ///
   ////////////////////////////////////////////////////////////
   std::string deparse(const Code & code);

   ////////////////////////////////////////////////////////////
   /// \brief Set interpreter context
   ///
   /// If something segfaults, try place it before
   ///
   ////////////////////////////////////////////////////////////
   Perl & setContext();

   /// @}



   /// @{ \name Operators

   ////////////////////////////////////////////////////////////
   /// \brief Universal operator
   ///
   /// \cpp
   /// ScalarPtr var = perl["$name"];
   /// ArrayPtr var = perl["@name"];
   /// HashPtr var = perl["%name"];
   /// CodePtr var = perl["&name"];
   /// \endcpp
   ///
   /// \exception PerlException Undefined symbol
   ///
   ////////////////////////////////////////////////////////////
   VariablePtr operator[] (std::string query);

   /// @}



   ////////////////////////////////////////////////////////////
   /// \brief Get Perl instance by object id
   ///
   ////////////////////////////////////////////////////////////
   static Perl & getInstanceById(int perl_id);

   ////////////////////////////////////////////////////////////
   /// \brief Get Perl instance by associated interpreter
   ///
   /// Useful in XS
   ///
   ////////////////////////////////////////////////////////////
   static Perl & getInstance(PerlInterpreter * _interpreter);



   bool isExceptionsEnabled() const;
   bool isWarningsEnabled() const;
   priv::Callbacks & getCallbacks();

protected:
   SV * _createSV(STRLEN size = 0);
   AV * _createAV(SSize_t size = 0, SV ** elements = nullptr);
   HV * _createHV();
   SV * _getOrCreateNamedSV(const std::string & name) const;
   AV * _getOrCreateNamedAV(const std::string & name) const;
   HV * _getOrCreateNamedHV(const std::string & name) const;
   SV * _getNamedSV(const std::string & name) const;
   AV * _getNamedAV(const std::string & name) const;
   HV * _getNamedHV(const std::string & name) const;
   CV * _getNamedCV(const std::string & name) const;

private:
   PerlInterpreter * _interpreter = nullptr;
   int _id;
   static int _last_id;
   static std::vector<Perl *> _perls;

   bool _is_system_initialized = false;
   bool _is_exceptions_enabled = true;
   bool _is_warnings_enabled   = false;

   ScalarPtr _code_deparser;
   priv::Callbacks _callbacks;
};

#include "yobaperl/perl.tpp"



} // namespace yoba

#endif // YOBAPERL_PERL_HPP
