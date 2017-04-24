#ifndef YOBAPERL_TEST_HPP
#define YOBAPERL_TEST_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/perl.hpp"

namespace yoba {



////////////////////////////////////////////////////////////
/// \brief Test::Simple interface
///
////////////////////////////////////////////////////////////
class Test
{
public:



   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   ////////////////////////////////////////////////////////////
   Test(Perl & perl, int planned_tests = 0);

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ///
   ////////////////////////////////////////////////////////////
   ~Test();



   /// @{ \name Testing

   bool ok(bool cond, std::string test_name = "");

   template<class A, class B>
   bool is(const A & a, const B & b, std::string test_name = "");

   template<class A, class B>
   bool isnt(const A & a, const B & b, std::string test_name = "");

   Test & pass(std::string test_name = "");

   Test & fail(std::string test_name = "");

   bool doneTesting();

   /// @}


private:
   Perl & _perl;
   bool _done_testing = false;
};

#include "yobaperl/test.tpp"



} // namespace yoba

#endif // YOBAPERL_TEST_HPP
