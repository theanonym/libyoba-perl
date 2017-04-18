#include "yobaperl/hash_iterator.hpp"
#include "yobaperl/perl.hpp"

namespace yoba {



HashIterator::HashIterator(const Hash & hash, bool is_null)
   : _hash(hash)
{
   if(!is_null)
   {
      _hash._interInit();
      _hash_entry = _hash._interNext();
   }
}

HashIterator::HashIterator(const HashIterator & to_copy)
   : _hash(to_copy._hash),
     _hash_entry(to_copy._getHE())
{
}

HashIterator::HashIterator(HashIterator && to_move)
   : _hash(to_move._hash),
     _hash_entry(to_move._getHE())
{
   to_move._hash_entry = nullptr;
}



HashEntry HashIterator::operator* () const
{
   return HashEntry(_hash, _getHE());
}

HashIterator & HashIterator::operator++ ()
{
   _hash_entry = _hash._interNext();
   return *this;
}

HashIterator HashIterator::operator++ (int)
{
   HashIterator copy = *this;
   ++(*this);
   return copy;
}

HashIterator & HashIterator::operator+= (SSize_t diff)
{
   for(SSize_t i = 0; i < diff; i++)
      ++(*this);
   return *this;
}

bool HashIterator::operator== (const HashIterator & other) const
{
   // Can be nullptr
   return _hash_entry == other._hash_entry;
}

bool HashIterator::operator!= (const HashIterator & other) const
{
   return !(*this == other);
}



HE * HashIterator::_getHE() const
{
   YOBAPERL_ASSERT(_hash_entry);
   return _hash_entry;
}



HashIterator::operator bool() const
{
   return _hash_entry != nullptr;
}



} // namespace yoba
