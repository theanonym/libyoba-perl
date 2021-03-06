#ifndef YOBAPERL_HASH_ENTRY_HPP
#define YOBAPERL_HASH_ENTRY_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/scalar.hpp"

namespace yoba {



////////////////////////////////////////////////////////////
/// \brief Key-value pair
///
/// \relates HashIterator
///
////////////////////////////////////////////////////////////
class YOBAPERL_EXPORT HashEntry
{
   friend class Hash;
   friend class HashIterator;

public:



   ////////////////////////////////////////////////////////////
   /// \brief Copy constructor
   ///
   ////////////////////////////////////////////////////////////
   HashEntry(const HashEntry & to_copy);

   ////////////////////////////////////////////////////////////
   /// \brief Move constructor
   ///
   ////////////////////////////////////////////////////////////
   HashEntry(HashEntry && to_move);



   /// @{ \name Misc

   ////////////////////////////////////////////////////////////
   /// \brief Get key string
   ///
   ////////////////////////////////////////////////////////////
   std::string getKey() const;

   ////////////////////////////////////////////////////////////
   /// \brief Get value
   ///
   ////////////////////////////////////////////////////////////
   Scalar getValue() const;

   ////////////////////////////////////////////////////////////
   /// \brief Convert to C++ string
   ///
   ////////////////////////////////////////////////////////////
   std::string toString() const;

   ////////////////////////////////////////////////////////////
   /// \brief Convert to C++ pair
   ///
   ////////////////////////////////////////////////////////////
   std::pair<std::string, Scalar> toPair() const;

   /// @}



   /// @{ \name Internals

   ////////////////////////////////////////////////////////////
   /// \brief Raw hash entry
   ///
   ////////////////////////////////////////////////////////////
   HE * getHE() const;

   ////////////////////////////////////////////////////////////
   /// \brief Internal hash code
   ///
   ////////////////////////////////////////////////////////////
   U32 getHashCode() const;

   /// @}



   /// @{ \name Operators

   bool operator== (const HashEntry & other) const;
   bool operator!= (const HashEntry & other) const;
   friend std::ostream & operator<< (std::ostream & stream, const HashEntry & entry);

   /// @}



private:
   HashEntry(const Hash & hash, HE * he);

   const Hash & _hash;
   HE * _he = nullptr;
};



} // namespace yoba

#endif // YOBAPERL_HASH_ENTRY_HPP
