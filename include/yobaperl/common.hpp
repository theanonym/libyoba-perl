#ifndef YOBAPERL_COMMON_HPP
#define YOBAPERL_COMMON_HPP


// STL
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <memory>


// Defines
#define my_perl _interpreter

#ifndef YOBAPERL_MULTIPLICITY
   #define PERL_NO_GET_CONTEXT
#endif

#ifdef DEBUGGING
   #undef NDEBUG
   #include <cassert>
#endif

#define P(x)  std::cout << x << std::endl
#define YOBAPERL_UNUSED(x) PERL_UNUSED_ARG(x)
#define YOBAPERL_ASSERT(x) assert(x)

#if __GNUC__ >= 4
   #define YOBAPERL_EXPORT __attribute__ ((__visibility__ ("default")))
#elif defined(_WIN32)
   #define YOBAPERL_EXPORT __declspec(dllexport)
#else
   #define YOBAPERL_EXPORT
#endif


// Perl headers
namespace yoba { extern "C" {

#include <EXTERN.h>
#include <perl.h>

void xs_init(pTHX);

}}

#endif // YOBAPERL_COMMON_HPP
