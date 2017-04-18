#ifndef YOBAPERL_ARRAY_ITERATOR_HPP
#define YOBAPERL_ARRAY_ITERATOR_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/scalar.hpp"

namespace yoba {



////////////////////////////////////////////////////////////
/// \brief Array iterator
///
/// \relates Array
///
/// \cpp
/// for(Array::Iterator it = array.begin(); it != array.end(); ++it)
///     std::cout << *x << std::endl;
///
/// for(Scalar x : array)
///    std::cout << x << std::endl;
/// \endcpp
///
////////////////////////////////////////////////////////////
class YOBAPERL_EXPORT ArrayIterator
{
   friend class Array;

public:



   ////////////////////////////////////////////////////////////
   /// \brief Move constructor
   ///
   ////////////////////////////////////////////////////////////
   ArrayIterator(ArrayIterator && to_move);



   /// @{ \name Operators

   Scalar operator* () const;
   ArrayIterator & operator++ ();
   ArrayIterator & operator-- ();
   ArrayIterator operator++ (int);
   ArrayIterator operator-- (int);
   ArrayIterator operator+ (SSize_t diff) const;
   ArrayIterator operator- (SSize_t diff) const;
   ArrayIterator & operator+= (SSize_t diff);
   ArrayIterator & operator-= (SSize_t diff);
   bool operator== (const ArrayIterator & other) const;
   bool operator!= (const ArrayIterator & other) const;

   /// @}



private:
   ArrayIterator(Perl & perl, SV ** ptr);

   Perl & _perl;
   SV ** _array_ptr;
};



} // namespace yoba

#endif // YOBAPERL_ARRAY_ITERATOR_HPP
