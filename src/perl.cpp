#include "yobaperl/perl.hpp"
#include "yobaperl/perl_stack.hpp"
#include "yobaperl/callbacks.hpp"

namespace yoba {



int Perl::_last_id = -1;
std::vector<Perl *> Perl::_perls;



Perl::Perl(bool do_init)
   : _callbacks(*this)
{
#ifndef YOBAPERL_MULTIPLICITY
   if(_last_id != -1)
      throw PerlException(0, "Only one Perl instance allowed without -DYOBAPERL_MULTIPLICITY");
#endif

   _id = ++_last_id;
   _perls.push_back(this);

   if(do_init)
      init();
}

Perl::Perl(Perl && to_move)
   : _interpreter(to_move._interpreter),
     _id(to_move._id),
     _is_exceptions_enabled(to_move.isExceptionsEnabled()),
     _is_warnings_enabled(to_move.isWarningsEnabled()),
     _callbacks(std::move(to_move._callbacks))
{
   to_move._interpreter = nullptr; // Skip destuctor
}

Perl::~Perl()
{
   // Skip destruction if object moved
   if(_interpreter)
   {
      perl_destruct(_interpreter);
      perl_free(_interpreter);
   }
}



Scalar Perl::newScalar()
{
   return Scalar(*this, _createSV(), /* ref++ */false);
}

Scalar Perl::newNamedScalar(const std::string & name)
{
   return Scalar(*this, _getOrCreateNamedSV(name), /* ref++ */true);
}

Array Perl::newArray()
{
   return Array(*this, _createAV(), /* ref++ */false);
}

Array Perl::newArray(IV begin, IV end)
{
   YOBAPERL_ASSERT(end > begin);

   Array result = newArray();
   result.reserve(end - begin);
   for(IV i = begin; i <= end; i++)
      result.push(newScalar<IV>(i));

   return result;
}

Array Perl::newNamedArray(const std::string & name)
{
   return Array(*this, _getOrCreateNamedAV(name), /* ref++ */true);
}

Hash Perl::newHash()
{
   return Hash(*this, _createHV(), /* ref++ */false);
}

Hash Perl::newNamedHash(const std::string & name)
{
   return Hash(*this, _getOrCreateNamedHV(name), /* ref++ */true);
}

//TODO complete
Scalar Perl::newObject(const std::string & class_name)
{
   return callMethod<Scalar>(class_name + "::new", newScalar(class_name));
}

Scalar Perl::newObject(const std::string & class_name, const Scalar & param)
{
   return callMethod<Scalar>(class_name + "::new", newScalar(class_name), param);
}

Scalar Perl::newObject(const std::string & class_name, const Array & params)
{
   return callMethod<Scalar>(class_name + "::new", newScalar(class_name), params);
}



Scalar Perl::getScalar(const std::string & name)
{
   return Scalar(*this, _getNamedSV(name), /* ref++ */true);
}

Array Perl::getArray(const std::string & name)
{
   return Array(*this, _getNamedAV(name), /* ref++ */true);
}

Hash Perl::getHash(const std::string & name)
{
   return Hash(*this, _getNamedHV(name), /* ref++ */true);
}

Code Perl::getSubroutine(const std::string & name)
{
   return Code(*this, _getNamedCV(name), /* ref++ */true);
}

Code Perl::getMethod(const std::string & name)
{
   return Code(*this, MUTABLE_CV(newScalar(name).detachSV()), true);
}

ScalarPtr Perl::getScalarPtr(const std::string & name)
{
   return ScalarPtr(new Scalar(*this, _getNamedSV(name), /* ref++ */true));
}

ArrayPtr Perl::getArrayPtr(const std::string & name)
{
   return ArrayPtr(new Array(*this, _getNamedAV(name), /* ref++ */true));
}

HashPtr Perl::getHashPtr(const std::string & name)
{
   return HashPtr(new Hash(*this, _getNamedHV(name), /* ref++ */true));
}

CodePtr Perl::getSubroutinePtr(const std::string & name)
{
   return CodePtr(new Code(*this, _getNamedCV(name), /* ref++ */true));
}



Perl & Perl::init()
{
   static const char * argv[] = { "", "-f", "-e", "0" };
   static const int argc = 4;

   // First object
   if(getId() == 0)
   {
      PERL_SYS_INIT((int *)&argc, (char ***)&argv);
      std::atexit(Perl_sys_term);
   }

   _interpreter = perl_alloc();
   PERL_SET_CONTEXT(_interpreter);

   perl_construct(_interpreter);

   PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
   PL_perl_destruct_level = 2;

   if(0 != perl_parse(_interpreter, xs_init, argc, (char **)argv, (char **)NULL))
      throw PerlException(getId(), "Init error");

#ifdef YOBAPERL_MULTIPLICITY
   Hash(*this, PL_modglobal, true).insert("YobaPerl::id", newScalar(getId()));
#endif

   return *this;
}

Perl & Perl::lib(const char * path)
{
   getArray("INC").push(newScalar(path));
   return *this;
}

Perl & Perl::use(const std::string & name)
{

   // load_module(0, newSVpv(name.c_str(),name.length()), NULL, NULL);
   eval("use " + name);

   if(hasError())
   {
      if(isExceptionsEnabled())
         throw PerlException(getId(), getError());

      if(isWarningsEnabled())
         warn("%s", getError().c_str());
   }

   return *this;
}

Perl & Perl::no(const std::string & name)
{
   load_module(PERL_LOADMOD_DENY, newSVpv(name.c_str(),name.length()), NULL, NULL);
   return *this;
}

Perl & Perl::require(const std::string & name)
{
//   require_pv(name.c_str());
   eval(std::string("require ") + name);

   if(hasError())
   {
      if(isExceptionsEnabled())
         throw PerlException(getId(), getError());

      if(isWarningsEnabled())
         warn("%s", getError().c_str());
   }

   return *this;
}

Perl & Perl::setExceptionsEnabled(bool state)
{
   _is_exceptions_enabled = state;
   return *this;
}

Perl & Perl::setWarningsEnabled(bool state)
{
   _is_warnings_enabled = state;
   return *this;
}

bool Perl::hasError() const
{
   return SvTRUE_NN(ERRSV);
}

std::string Perl::getError() const
{
   return SvPVx_nolen_const(ERRSV);
}



Perl & Perl::registerVoidToVoid(const std::string & name, priv::Callbacks::VoidToVoidCB function)
{
   _callbacks.setVoidToVoid(name, function);
   return *this;
}

Perl & Perl::registerVoidToScalar(const std::string & name, priv::Callbacks::VoidToScalarCB function)
{
   _callbacks.setVoidToScalar(name, function);
   return *this;
}

Perl & Perl::registerScalarToVoid(const std::string & name, priv::Callbacks::ScalarToVoidCB function)
{
   _callbacks.setScalarToVoid(name, function);
   return *this;
}

Perl & Perl::registerScalarToScalar(const std::string & name, priv::Callbacks::ScalarToScalarCB function)
{
   _callbacks.setScalarToScalar(name, function);
   return *this;
}

CV * Perl::registerStatic(const std::string & sub_name, void(*function)(PerlInterpreter *, CV *))
{
   CV * sub = Perl_newXS_len_flags(getInterpreter(), sub_name.c_str(), sub_name.length(), function,
                                   /*filename*/NULL, /*proto*/NULL, /*svp*/NULL, /*flags*/0);
   YOBAPERL_ASSERT(sub);
   return sub;
}

priv::Callbacks & Perl::getCallbacks()
{
   return _callbacks;
}



int Perl::getId() const
{
   return _id;
}

PerlInterpreter * Perl::getInterpreter() const
{
   return _interpreter;
}

SV * Perl::getNamedSV(const std::string & name) const
{
   return _getNamedSV(name);
}

AV * Perl::getNamedAV(const std::string & name) const
{
   return _getNamedAV(name);
}

HV * Perl::getNamedHV(const std::string & name) const
{
   return _getNamedHV(name);
}

CV * Perl::getNamedCV(const std::string & name) const
{
   return _getNamedCV(name);
}

bool Perl::isExceptionsEnabled() const
{
   return _is_exceptions_enabled;
}

bool Perl::isWarningsEnabled() const
{
   return _is_warnings_enabled;
}

std::string Perl::deparse(const Code & code)
{
   // Static block
   if(!_code_deparser)
   {
      require("B::Deparse");
      _code_deparser.reset(new Scalar(newObject("B::Deparse", newArray({ "-p", "-sC" }))));
//      _code_deparser.reset(new Scalar(eval<Scalar>("B::Deparse->new('-p', '-sC')")));
   }

   return callMethod<Scalar>("coderef2text", *_code_deparser, code.makeRef()).toString();
}

Perl & Perl::setContext()
{
   PERL_SET_CONTEXT(_interpreter);
   return *this;
}

Perl & Perl::getInstanceById(int id)
{
   return *_perls.at(id);
}

Perl & Perl::getInstance(PerlInterpreter * _interpreter)
{
#ifdef YOBAPERL_MULTIPLICITY
   static const std::string name = "YobaPerl::id";

   SV ** elements = hv_fetch(PL_modglobal, name.c_str(), name.length(), NULL);
   YOBAPERL_ASSERT(elements);

   SV * element = *elements;
   YOBAPERL_ASSERT(element);

   return getInstanceById(SvIV(element));
#else
   return getInstanceById(0);
#endif
}



VariablePtr Perl::operator[] (std::string query)
{
   if(query.size() < 2)
      throw PerlException(getId(), "[]");

   char prefix = query[0];
   query.erase(0, 1);

   switch(prefix)
   {
      case '$': return getScalarPtr(query);
      case '@': return getArrayPtr(query);
      case '%': return getHashPtr(query);
      case '&': return getSubroutinePtr(query);

      default:  throw PerlException(getId(), "[]");
   }
}



SV * Perl::_createSV(STRLEN size)
{
   SV * result = newSV(size);
   YOBAPERL_ASSERT(result);
   return result;
}

AV * Perl::_createAV(SSize_t size, SV ** elements)
{
   AV * result = nullptr;
   if(size)
      result = av_make(size, elements);
   else
      result = newAV();

   YOBAPERL_ASSERT(result);
   return result;
}

HV * Perl::_createHV()
{
   HV * result = newHV();
   YOBAPERL_ASSERT(result);
   return result;
}

SV * Perl::_getOrCreateNamedSV(const std::string & name) const
{
   GV * gv = gv_fetchpvn_flags(name.c_str(), name.length(), GV_ADD, SVt_PV);
   YOBAPERL_ASSERT(gv);
   return GvSV(gv);
}

AV * Perl::_getOrCreateNamedAV(const std::string & name) const
{
   GV * gv = gv_fetchpvn_flags(name.c_str(), name.length(), GV_ADD, SVt_PVAV);
   YOBAPERL_ASSERT(gv);
   return GvAV(gv);
}

HV * Perl::_getOrCreateNamedHV(const std::string & name) const
{
   GV * gv = gv_fetchpvn_flags(name.c_str(), name.length(), GV_ADD, SVt_PVHV);
   YOBAPERL_ASSERT(gv);
   return GvHV(gv);
}

SV * Perl::_getNamedSV(const std::string & name) const
{
   if(isExceptionsEnabled() || isWarningsEnabled())
   {
      GV * gv = gv_fetchpvn_flags(name.c_str(), name.length(), /*flags*/0, SVt_PV);
      if(gv == NULL)
      {
         std::string error = "Undefined symbol: $" + name;

         if(isExceptionsEnabled())
            throw PerlException(getId(), error);

         if(isWarningsEnabled())
            warn("%s", error.c_str());
      }
   }

   return _getOrCreateNamedSV(name);
}

AV * Perl::_getNamedAV(const std::string & name) const
{
   if(isExceptionsEnabled() || isWarningsEnabled())
   {
      GV * gv = gv_fetchpvn_flags(name.c_str(), name.length(), /*flags*/0, SVt_PVAV);

      if(gv == NULL)
      {
         std::string error = "Undefined symbol: @" + name;

         if(isExceptionsEnabled())
            throw PerlException(getId(), error);

         if(isWarningsEnabled())
            warn("%s", error.c_str());
      }

      return GvAV(gv);
   }

   return _getOrCreateNamedAV(name);
}

HV * Perl::_getNamedHV(const std::string & name) const
{
   if(isExceptionsEnabled() || isWarningsEnabled())
   {
      GV * gv = gv_fetchpvn_flags(name.c_str(), name.length(), /*flags*/0, SVt_PVHV);

      if(gv == NULL)
      {
         std::string error = "Undefined symbol: %" + name;

         if(isExceptionsEnabled())
            throw PerlException(getId(), error);

         if(isWarningsEnabled())
            warn("%s", error.c_str());
      }

      return GvHV(gv);
   }

   return _getOrCreateNamedHV(name);
}

CV * Perl::_getNamedCV(const std::string & name) const
{
   GV * gv = gv_fetchpvn_flags(name.c_str(), name.length(), /*flags*/0, SVt_PVCV);

   if(gv == NULL)
   {
      //TODO Complete
      std::string error = "Undefined symbol: &" + name;

//      if(isExceptionsEnabled())
         throw PerlException(getId(), error);

//      if(isWarningsEnabled())
//         warn("%s", error.c_str());
   }

   return GvCV(gv);
}



} // namespace yoba
