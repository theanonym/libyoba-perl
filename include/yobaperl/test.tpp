// Inline templates for test.hpp

template<class A, class B>
bool Test::is(const A & a, const B & b, std::string test_name)
{
   // is(a, b, test_name)
   std::stringstream ss;
   ss << "is '" << a << "', '" << b << "'";

   if(!test_name.empty())
      ss << ", '" << test_name << "'";

   return _perl.eval<Scalar>(ss.str()).toBool();
}

template<class A, class B>
bool Test::isnt(const A & a, const B & b, std::string test_name)
{
   // isnt(a, b, test_name)
   std::stringstream ss;
   ss << "isnt '" << a << "', '" << b << "'";

   if(!test_name.empty())
      ss << ", '" << test_name << "'";

   return _perl.eval<Scalar>(ss.str()).toBool();
}
