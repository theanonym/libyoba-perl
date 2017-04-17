#ifndef YOBAPERL_TEST_HPP
#define YOBAPERL_TEST_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/perl.hpp"

namespace yoba {



class Test
{
public:
   Test(Perl & perl, int tests_count = 0);
   ~Test();

   bool ok(bool cond, std::string test_name = "");
   template<class A, class B>
   bool is(const A & a, const B & b, std::string test_name = "");
   template<class A, class B>
   bool isnt(const A & a, const B & b, std::string test_name = "");
   Test & pass(std::string test_name = "");
   Test & fail(std::string test_name = "");
   IV doneTesting();

private:
   Perl & _perl;
   bool _done_testing = false;
};

#include "yobaperl/test.tpp"



} // namespace yoba

#endif // YOBAPERL_TEST_HPP
