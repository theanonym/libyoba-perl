#include "yobaperl/hash_entry.hpp"
#include "yobaperl/hash.hpp"

namespace yoba {



HashEntry::HashEntry(const Hash & hash, HE * he)
   : _hash(hash),
     _he(he)
{
}

HashEntry::HashEntry(const HashEntry & to_copy)
   : _hash(to_copy._hash),
     _he(to_copy._he)
{
}

HashEntry::HashEntry(HashEntry && to_move)
   : _hash(to_move._hash),
     _he(to_move._he)
{
}



std::string HashEntry::getKey() const
{
   return std::string(HeKEY(getHE()), HeKLEN(getHE()));
}

Scalar HashEntry::getValue() const
{
   return Scalar(_hash.getPerl(), HeVAL(getHE()), true);
}

std::string HashEntry::toString() const
{
   return getKey() + " => " + getValue().toString();
}

std::pair<std::string, Scalar> HashEntry::toPair() const
{
   return std::make_pair(getKey(), getValue());
}



HE * HashEntry::getHE() const
{
   YOBAPERL_ASSERT(_he);
   return _he;
}

U32 HashEntry::getHashCode() const
{
   return HeHASH(getHE());
}



bool HashEntry::operator== (const HashEntry & other) const
{
   return getHE() == other.getHE();
}

bool HashEntry::operator!= (const HashEntry & other) const
{
   return !(*this == other);
}

std::ostream & operator<< (std::ostream & stream, const HashEntry & entry)
{
   return stream << entry.toString();
}



} // namespace yoba
