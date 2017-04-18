#ifndef YOBAPERL_HASH_ITERATOR_HPP
#define YOBAPERL_HASH_ITERATOR_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/hash_entry.hpp"

namespace yoba {


////////////////////////////////////////////////////////////
/// \brief Hash iterator
///
/// \relates Hash
///
/// \cpp
/// for(Hash::Iterator it = hash.begin(); it != hash.end(); ++it) {
///     std::cout << *x << std::endl;
///     std::cout << (*x).getKey() << " => " << (*x).getValue() << std::endl;
/// }
///
/// for(HashEntry x : hash)
///    std::cout << x << std::endl;
/// \endcpp
///
////////////////////////////////////////////////////////////
class YOBAPERL_EXPORT HashIterator
{
   friend class Hash;

public:



   ////////////////////////////////////////////////////////////
   /// \brief Move constructor
   ///
   ////////////////////////////////////////////////////////////
   HashIterator(HashIterator && to_move);



   /// @{ \name Operators

   HashEntry operator* () const;
   HashIterator & operator++ ();
   HashIterator operator++ (int);
   HashIterator & operator+= (SSize_t diff);
   bool operator== (const HashIterator & other) const;
   bool operator!= (const HashIterator & other) const;
   operator bool() const;

   /// @}



protected:
   HE * _getHE() const;

private:
   HashIterator(const Hash & hash, bool is_null = false);
   HashIterator(const HashIterator & to_copy);

   const Hash & _hash;
   HE * _hash_entry = nullptr;
};



}

#endif // YOBAPERL_HASH_ITERATOR_HPP
