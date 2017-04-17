#ifndef YOBAPERL_HASH_ENTRY_HPP
#define YOBAPERL_HASH_ENTRY_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/scalar.hpp"

namespace yoba {



class YOBAPERL_EXPORT HashEntry
{
   friend class Hash;
   friend class HashIterator;

public:
   // Constructors
   HashEntry(const HashEntry & to_copy);
   HashEntry(HashEntry && to_move);

   // Common
   std::string getKey() const;
   Scalar getValue() const;
   std::string toString() const;
   std::pair<std::string, Scalar> toPair() const;

   // Internals
   HE * getHE() const;
   U32 getHashCode() const;

   // Operators
   bool operator== (const HashEntry & other) const;
   bool operator!= (const HashEntry & other) const;
   friend std::ostream & operator<< (std::ostream & stream, const HashEntry & entry);

private:
   HashEntry(const Hash & hash, HE * he);

   const Hash & _hash;
   HE * _he = nullptr;
};



} // namespace yoba

#endif // YOBAPERL_HASH_ENTRY_HPP
