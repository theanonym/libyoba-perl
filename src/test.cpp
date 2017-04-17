#include "yobaperl/test.hpp"

namespace yoba {



Test::Test(Perl & perl, int tests_count)
   : _perl(perl)
{
   _perl.use("Test::More");

   if(tests_count)
      _perl.eval(" plan(tests => " + std::to_string(tests_count) + ") ");
}

Test::~Test()
{
   if(!_done_testing)
      doneTesting();
}



bool Test::ok(bool cond, std::string test_name)
{
   if(cond)
      pass(test_name);
   else
      fail(test_name);

   return cond;
}

Test & Test::pass(std::string test_name)
{
   _perl.eval(" pass('" + test_name + "')");
   return *this;
}

Test & Test::fail(std::string test_name)
{
   _perl.eval(" fail('" + test_name + "')");
   return *this;
}

IV Test::doneTesting()
{
   _done_testing = true;
   return _perl.eval<Scalar>(" done_testing() ").toIV();
}



} // namespace yoba
