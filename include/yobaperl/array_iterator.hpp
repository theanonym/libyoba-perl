#ifndef YOBAPERL_ARRAY_ITERATOR_HPP
#define YOBAPERL_ARRAY_ITERATOR_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/scalar.hpp"

namespace yoba {



class YOBAPERL_EXPORT ArrayIterator
{
   friend class Array;

public:
   // Constructors
   ArrayIterator(ArrayIterator && to_move);

   // Operators
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

private:
   ArrayIterator(Perl & perl, SV ** ptr);

   Perl & _perl;
   SV ** _array_ptr;
};



} // namespace yoba

#endif // YOBAPERL_ARRAY_ITERATOR_HPP
