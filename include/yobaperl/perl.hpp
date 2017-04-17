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
#include "yobaperl/ptr.hpp"

namespace yoba {



//TODO perl_clone
/// \brief Main class
class YOBAPERL_EXPORT Perl
{
   friend class Scalar;
   friend class Array;
   friend class Hash;

public:
   // Constructors
   Perl(bool do_init = true);
   Perl(Perl && to_move);
   ~Perl();

   // Creating variables
   Scalar newScalar();
   Scalar newNamedScalar(const std::string & name);
   template<typename ValueT> Scalar newScalar(const ValueT & value);
   template<typename ValueT> Scalar newNamedScalar(const std::string & name, const ValueT & value);
   Array newArray();
   Array newArray(IV begin, IV end);
   Array newNamedArray(const std::string & name);
   template<typename ValueT> Array newArray(const std::vector<ValueT> & values);
   template<typename ValueT> Array newArray(const std::initializer_list<ValueT> & values);
   template<typename ValueT> Array newNamedArray(const std::string & name, const std::vector<ValueT> & values);
   template<typename ValueT> Array newNamedArray(const std::string & name, const std::initializer_list<ValueT> & values);
   Hash newHash();
   Hash newNamedHash(const std::string & name);
   template<typename ValueT> Hash newHash(const std::unordered_map<std::string, ValueT> & map);
   template<typename ValueT> Hash newNamedHash(const std::string & name, const std::unordered_map<std::string, ValueT> & map);
   Scalar newObject(const std::string & class_name);
   Scalar newObject(const std::string & class_name, const Scalar & param);
   Scalar newObject(const std::string & class_name, const Array & params);

   // Get variables
   Scalar getScalar(const std::string & name);
   Array getArray(const std::string & name);
   Hash getHash(const std::string & name);
   Code getSubroutine(const std::string & name);
   Code getMethod(const std::string & name);
   ScalarPtr getScalarPtr(const std::string & name);
   ArrayPtr getArrayPtr(const std::string & name);
   HashPtr getHashPtr(const std::string & name);
   CodePtr getSubroutinePtr(const std::string & name);
   
   // Common
   Perl & init();
   Perl & lib(const char * path);
   Perl & use(const std::string & name);
   Perl & no(const std::string & name);
   Perl & require(const std::string & name);
   Perl & setExceptionsEnabled(bool state);
   Perl & setWarningsEnabled(bool state);
   bool hasError() const;
   std::string getError() const;

   // Call perl code
   template<typename ReturnT = void>
   ReturnT eval(const std::string & code);

   template<typename ReturnT, typename ParamT>
   ReturnT call(const Code & subroutine, const ParamT & param);
   template<typename ReturnT = void>
   ReturnT call(const Code & subroutine);
   template<typename ReturnT, typename ParamT>
   ReturnT call(const std::string & sub_name, const ParamT & param);
   template<typename ReturnT = void>
   ReturnT call(const std::string & sub_name);

   template<typename ReturnT, typename ParamT>
   ReturnT callMethod(const Code & subroutine, const Scalar & object, const ParamT & param);
   template<typename ReturnT = void>
   ReturnT callMethod(const Code & subroutine, const Scalar & object);
   template<typename ReturnT, typename ParamT>
   ReturnT callMethod(const std::string & sub_name, const Scalar & object, const ParamT & param);
   template<typename ReturnT = void>
   ReturnT callMethod(const std::string & sub_name, const Scalar & object);

   // Callbacks
   Perl & registerVoidToVoid(const std::string & name, priv::Callbacks::VoidToVoidCB function);
   Perl & registerVoidToScalar(const std::string & name, priv::Callbacks::VoidToScalarCB function);
   Perl & registerScalarToVoid(const std::string & name, priv::Callbacks::ScalarToVoidCB function);
   Perl & registerScalarToScalar(const std::string & name, priv::Callbacks::ScalarToScalarCB function);
   CV * registerStatic(const std::string & sub_name, XSUBADDR_t function);
   priv::Callbacks & getCallbacks();

   // Internals
   int getId() const;
   PerlInterpreter * getInterpreter() const;
   SV * getNamedSV(const std::string & name) const;
   AV * getNamedAV(const std::string & name) const;
   HV * getNamedHV(const std::string & name) const;
   CV * getNamedCV(const std::string & name) const;
   bool isExceptionsEnabled() const;
   bool isWarningsEnabled() const;
   std::string deparse(const Code & code);
   Perl & setContext();

   // Static
   static Perl & getInstanceById(int perl_id);
   static Perl & getInstance(PerlInterpreter * _interpreter);

   // Operators
   VariablePtr operator[] (std::string query);

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
   bool _is_warnings_enabled   = true;

   ScalarPtr _code_deparser;
   priv::Callbacks _callbacks;
};



#include "yobaperl/perl.tpp"



} // namespace yoba

#endif // YOBAPERL_PERL_HPP
