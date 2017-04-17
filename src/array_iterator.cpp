#include "yobaperl/array_iterator.hpp"
#include "yobaperl/array.hpp"

namespace yoba {



ArrayIterator::ArrayIterator(Perl & perl, SV ** ptr)
   : _perl(perl),
     _array_ptr(ptr)
{
}

ArrayIterator::ArrayIterator(ArrayIterator && to_move)
   : _perl(to_move._perl),
     _array_ptr(to_move._array_ptr)
{
}



Scalar ArrayIterator::operator* () const
{
   return Scalar(_perl, *_array_ptr, true);
}

ArrayIterator & ArrayIterator::operator++ ()
{
   _array_ptr++;
   return *this;
}

ArrayIterator & ArrayIterator::operator-- ()
{
   _array_ptr--;
   return *this;
}

ArrayIterator ArrayIterator::operator++ (int)
{
   return (*this)++;
}

ArrayIterator ArrayIterator::operator-- (int)
{
   return (*this)--;
}

ArrayIterator ArrayIterator::operator+ (SSize_t diff) const
{
   return ArrayIterator(_perl, _array_ptr + diff);
}

ArrayIterator ArrayIterator::operator- (SSize_t diff) const
{
   return ArrayIterator(_perl, _array_ptr - diff);
}

ArrayIterator & ArrayIterator::operator+= (SSize_t diff)
{
   _array_ptr += diff;
   return *this;
}

ArrayIterator & ArrayIterator::operator-= (SSize_t diff)
{
   _array_ptr -= diff;
   return *this;
}

bool ArrayIterator::operator== (const ArrayIterator & other) const
{
   return _array_ptr == other._array_ptr;
}

bool ArrayIterator::operator!= (const ArrayIterator & other) const
{
   return !(*this == other);
}



} // namespace yoba
