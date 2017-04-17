#ifndef YOBAPERL_HASH_ITERATOR_HPP
#define YOBAPERL_HASH_ITERATOR_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/hash_entry.hpp"

namespace yoba {



class YOBAPERL_EXPORT HashIterator
{
   friend class Hash;

public:
   // Constructors
   HashIterator(HashIterator && to_move);

   // Operators
   HashEntry operator* () const;
   HashIterator & operator++ ();
   HashIterator operator++ (int);
   HashIterator & operator+= (SSize_t diff);
   bool operator== (const HashIterator & other) const;
   bool operator!= (const HashIterator & other) const;
   operator bool() const;

   // Internals
   HE * getHE() const;

private:
   HashIterator(const Hash & hash, bool is_null = false);
   HashIterator(const HashIterator & to_copy);

   const Hash & _hash;
   HE * _hash_entry = nullptr;
};



}

#endif // YOBAPERL_HASH_ITERATOR_HPP
