// Inline templates for perl.hpp

// newScalar(ValueT)
template<typename ValueT> inline
Scalar Perl::newScalar(const ValueT & value)
{
   Scalar result = newScalar();
   result.setValue(value);
   return result;
}

// newNamedScalar(name, ValueT)
template<typename ValueT> inline
Scalar Perl::newNamedScalar(const std::string & name, const ValueT & value)
{
   Scalar result = newNamedScalar(name);
   result.setValue(value);
   return result;
}

// newArray(vector { Scalar })
template<> inline
Array Perl::newArray<Scalar>(const std::vector<Scalar> & values)
{
   Array result = newArray();
   result.push(values);
   return result;
}

// newArray({ Scalar })
template<> inline
Array Perl::newArray<Scalar>(const std::initializer_list<Scalar> & values)
{
   return newArray<Scalar>(std::vector<Scalar>(values));
}

// newNamedArray(name, vector { Scalar })
template<> inline
Array Perl::newNamedArray<Scalar>(const std::string & name, const std::vector<Scalar> & values)
{
   Array result = newNamedArray(name);
   result.push(values);
   return result;
}

// newNamedArray(name, { Scalar })
template<> inline
Array Perl::newNamedArray<Scalar>(const std::string & name, const std::initializer_list<Scalar> & values)
{
   return newNamedArray<Scalar>(name, std::vector<Scalar>(values));
}

// newArray(vector { ValueT })
template<typename ValueT> inline
Array Perl::newArray(const std::vector<ValueT> & values)
{
   std::vector<Scalar> scalars;
   scalars.reserve(values.size());

   for(auto it = values.cbegin(); it != values.cend(); it++)
      scalars.push_back(newScalar<ValueT>(*it));

   return newArray<Scalar>(scalars);
}

// newArray({ ValueT })
template<typename ValueT> inline
Array Perl::newArray(const std::initializer_list<ValueT> & values)
{
   return newArray(std::vector<ValueT>(values));
}

// newNamedArray(name, vector { ValueT })
template<typename ValueT> inline
Array Perl::newNamedArray(const std::string & name, const std::vector<ValueT> & values)
{
   std::vector<Scalar> scalars;
   scalars.reserve(values.size());

   for(auto it = values.cbegin(); it != values.cend(); it++)
      scalars.push_back(newScalar<ValueT>(*it));

   return newNamedArray<Scalar>(name, scalars);
}

// newNamedArray(name, { ValueT })
template<typename ValueT> inline
Array Perl::newNamedArray(const std::string & name, const std::initializer_list<ValueT> & values)
{
   return newNamedArray<ValueT>(name, std::vector<ValueT>(values));
}

// newHash({ { "key", Scalar } })
template<> inline
Hash Perl::newHash<Scalar>(const std::unordered_map<std::string, Scalar> & map)
{
   Hash result = newHash();
   result.insert(map);
   return result;
}

// newNamedHash(name, { { "key", Scalar } })
template<> inline
Hash Perl::newNamedHash<Scalar>(const std::string & name, const std::unordered_map<std::string, Scalar> & map)
{
   Hash result = newNamedHash(name);
   result.insert(map);
   return result;
}

// newHash({ { "key", ValueT } })
template<typename ValueT> inline
Hash Perl::newHash(const std::unordered_map<std::string, ValueT> & map)
{
   std::unordered_map<std::string, Scalar> scalar_map;
   scalar_map.reserve(map.size());

   for(auto it = map.cbegin(); it != map.cend(); it++)
      scalar_map.insert(std::make_pair(it->first, newScalar<ValueT>(it->second)));

   return newHash<Scalar>(scalar_map);
}

// newNamedHash(name, { { "key", ValueT } })
template<typename ValueT> inline
Hash Perl::newNamedHash(const std::string & name, const std::unordered_map<std::string, ValueT> & map)
{
   std::unordered_map<std::string, Scalar> scalar_map;
   scalar_map.reserve(map.size());

   for(auto it = map.cbegin(); it != map.cend(); it++)
      scalar_map.insert(std::make_pair(it->first, newScalar<ValueT>(it->second)));

   return newNamedHash<Scalar>(name, scalar_map);
}



// eval(code)
template<> inline
void Perl::eval<void>(const std::string & code)
{
   priv::PerlStack stack(*this);
   stack.eval(code, G_VOID | G_DISCARD);
}

// eval<Scalar>(code)
template<> inline
Scalar Perl::eval<Scalar>(const std::string & code)
{
   priv::PerlStack stack(*this);
   stack.eval(code, G_SCALAR);
   return stack.popScalar();
}

// eval<Array>(code)
template<> inline
Array Perl::eval<Array>(const std::string & code)
{
   priv::PerlStack stack(*this);
   stack.eval(code, G_ARRAY);
   return stack.popArray();
}



// call(name)
template<> inline
void Perl::call<void>(const Code & subroutine)
{
   priv::PerlStack stack(*this);
   stack.call(subroutine, G_VOID | G_DISCARD | G_NOARGS | G_EVAL);
}

template<> inline
void Perl::call<void>(const std::string & sub_name)
{
   call<void>(getSubroutine(sub_name));
}


// call<void, Scalar>(name, Scalar)
template<> inline
void Perl::call<void, Scalar>(const Code & subroutine, const Scalar & param)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(param);
   stack.call(subroutine, G_VOID | G_DISCARD | G_EVAL);
}

template<> inline
void Perl::call<void, Scalar>(const std::string & sub_name, const Scalar & param)
{
   call<void, Scalar>(getSubroutine(sub_name), param);
}


// call<void, Array>(name, Array)
template<> inline
void Perl::call<void, Array>(const Code & subroutine, const Array & params)
{
   priv::PerlStack stack(*this);
   stack.pushArray(params);
   stack.call(subroutine, G_VOID | G_DISCARD | G_EVAL);
}

template<> inline
void Perl::call<void, Array>(const std::string & sub_name, const Array & params)
{
   call<void, Array>(getSubroutine(sub_name), params);
}


// call<Scalar>(name)
template<> inline
Scalar Perl::call<Scalar>(const Code & subroutine)
{
   priv::PerlStack stack(*this);
   stack.call(subroutine, G_SCALAR | G_NOARGS | G_EVAL);
   return stack.popScalar();
}

template<> inline
Scalar Perl::call<Scalar>(const std::string & sub_name)
{
   return call<Scalar>(getSubroutine(sub_name));
}


// call<Scalar, Scalar>(name, Scalar)
template<> inline
Scalar Perl::call<Scalar, Scalar>(const Code & subroutine, const Scalar & param)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(param);
   stack.call(subroutine, G_SCALAR | G_EVAL);
   return stack.popScalar();
}

template<> inline
Scalar Perl::call<Scalar, Scalar>(const std::string & sub_name, const Scalar & param)
{
   return call<Scalar, Scalar>(getSubroutine(sub_name), param);
}


// call<Scalar, Array>(name, Array)
template<> inline
Scalar Perl::call<Scalar, Array>(const Code & subroutine, const Array & params)
{
   priv::PerlStack stack(*this);
   stack.pushArray(params);
   stack.call(subroutine, G_ARRAY | G_EVAL);
   return stack.popScalar();
}

template<> inline
Scalar Perl::call<Scalar, Array>(const std::string & sub_name, const Array & params)
{
   return call<Scalar, Array>(getSubroutine(sub_name), params);
}


// call<Array>(name)
template<> inline
Array Perl::call<Array>(const Code & subroutine)
{
   priv::PerlStack stack(*this);
   stack.call(subroutine, G_ARRAY | G_NOARGS | G_EVAL);
   return stack.popArray();
}

template<> inline
Array Perl::call<Array>(const std::string & sub_name)
{
   return call<Array>(getSubroutine(sub_name));
}


// call<Array, Scalar>(name, Scalar)
template<> inline
Array Perl::call<Array, Scalar>(const Code & subroutine, const Scalar & param)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(param);
   stack.call(subroutine, G_ARRAY | G_EVAL);
   return stack.popArray();
}

template<> inline
Array Perl::call<Array, Scalar>(const std::string & sub_name, const Scalar & param)
{
   return call<Array, Scalar>(getSubroutine(sub_name), param);
}


// call<Array, Array>(name, Array)
template<> inline
Array Perl::call<Array, Array>(const Code & subroutine, const Array & params)
{
   priv::PerlStack stack(*this);
   stack.pushArray(params);
   stack.call(subroutine, G_ARRAY | G_EVAL);
   return stack.popArray();
}

template<> inline
Array Perl::call<Array, Array>(const std::string & sub_name, const Array & params)
{
   return call<Array, Array>(getSubroutine(sub_name), params);
}



// callMethod(name)
template<> inline
void Perl::callMethod<void>(const Code & subroutine, const Scalar & object)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.callMethod(subroutine, G_VOID | G_DISCARD | G_EVAL);
}

template<> inline
void Perl::callMethod<void>(const std::string & sub_name, const Scalar & object)
{
   callMethod<void>(getMethod(sub_name), object);
}


// callMethod<void, Scalar>(name, Scalar)
template<> inline
void Perl::callMethod<void, Scalar>(const Code & subroutine, const Scalar & object, const Scalar & param)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.pushScalar(param);
   stack.callMethod(subroutine, G_VOID | G_DISCARD | G_EVAL);
}

template<> inline
void Perl::callMethod<void, Scalar>(const std::string & sub_name, const Scalar & object, const Scalar & param)
{
   callMethod<void, Scalar>(getMethod(sub_name), object, param);
}


// callMethod<void, Array>(name, Array)
template<> inline
void Perl::callMethod<void, Array>(const Code & subroutine, const Scalar & object, const Array & params)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.pushArray(params);
   stack.callMethod(subroutine, G_VOID  | G_EVAL);
}

template<> inline
void Perl::callMethod<void, Array>(const std::string & sub_name, const Scalar & object, const Array & params)
{
   callMethod<void, Array>(getMethod(sub_name), object, params);
}


// callMethod<Scalar>(name)
template<> inline
Scalar Perl::callMethod<Scalar>(const Code & subroutine, const Scalar & object)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.callMethod(subroutine, G_SCALAR | G_EVAL);
   return stack.popScalar();
}

template<> inline
Scalar Perl::callMethod<Scalar>(const std::string & sub_name, const Scalar & object)
{
   return callMethod<Scalar>(getMethod(sub_name), object);
}


// callMethod<Scalar, Scalar>(name, Scalar)
template<> inline
Scalar Perl::callMethod<Scalar, Scalar>(const Code & subroutine, const Scalar & object, const Scalar & param)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.pushScalar(param);
   stack.callMethod(subroutine, G_SCALAR | G_EVAL);
   return stack.popScalar();
}

template<> inline
Scalar Perl::callMethod<Scalar, Scalar>(const std::string & sub_name, const Scalar & object, const Scalar & param)
{
   return callMethod<Scalar, Scalar>(getMethod(sub_name), object, param);
}


// callMethod<Scalar, Array>(name, Array)
template<> inline
Scalar Perl::callMethod<Scalar, Array>(const Code & subroutine, const Scalar & object, const Array & params)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.pushArray(params);
   stack.callMethod(subroutine, G_ARRAY | G_EVAL);
   return stack.popScalar();
}

template<> inline
Scalar Perl::callMethod<Scalar, Array>(const std::string & sub_name, const Scalar & object, const Array & params)
{
   return callMethod<Scalar, Array>(getMethod(sub_name), object, params);
}


// callMethod<Array>(name)
template<> inline
Array Perl::callMethod<Array>(const Code & subroutine, const Scalar & object)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.callMethod(subroutine, G_ARRAY | G_EVAL);
   return stack.popArray();
}

template<> inline
Array Perl::callMethod<Array>(const std::string & sub_name, const Scalar & object)
{
   return callMethod<Array>(getMethod(sub_name), object);
}


// callMethod<Array, Scalar>(name, Scalar)
template<> inline
Array Perl::callMethod<Array, Scalar>(const Code & subroutine, const Scalar & object, const Scalar & param)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.pushScalar(param);
   stack.callMethod(subroutine, G_ARRAY | G_EVAL);
   return stack.popArray();
}

template<> inline
Array Perl::callMethod<Array, Scalar>(const std::string & sub_name, const Scalar & object, const Scalar & param)
{
   return callMethod<Array, Scalar>(getMethod(sub_name), object, param);
}


// callMethod<Array, Array>(name, Array)
template<> inline
Array Perl::callMethod<Array, Array>(const Code & subroutine, const Scalar & object, const Array & params)
{
   priv::PerlStack stack(*this);
   stack.pushScalar(object);
   stack.pushArray(params);
   stack.callMethod(subroutine, G_ARRAY | G_EVAL);
   return stack.popArray();
}

template<> inline
Array Perl::callMethod<Array, Array>(const std::string & sub_name, const Scalar & object, const Array & params)
{
   return callMethod<Array, Array>(getMethod(sub_name), object, params);
}
