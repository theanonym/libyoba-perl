#ifndef YOBAPERL_HASH_HPP
#define YOBAPERL_HASH_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/variable.hpp"
#include "yobaperl/hash_iterator.hpp"

namespace yoba {



class YOBAPERL_EXPORT Hash : public Variable
{
   friend class HashIterator;

public:
   using Iterator = HashIterator;
   
   // Constructors
   Hash(Perl & perl, HV * hv, bool increase_refcount);
   
   // Elements manipulation
   Hash & insert(const std::string & key, const Scalar & value);
   Hash & insert(const std::pair<std::string, Scalar> & pair);
   Hash & insert(const std::unordered_map<std::string, Scalar> & hashmap);
   Scalar get(const std::string & key);
   Scalar remove(const std::string & key);
   Hash & clear();
   
   // Common
   I32 getSize() const;
   bool isEmpty() const;
   bool isExists(const std::string & key) const;
   std::string toString() const;
   std::unordered_map<std::string, Scalar> toMap() const;
   Scalar makeRef() const;
   Hash makeCopy() const;
   Iterator begin() const;
   Iterator end() const;
   
   // Internals
   HV * getHV() const;

   // Operators
   Scalar operator[] (const std::string & key) noexcept;
   Scalar operator[] (const char * key) noexcept;
   operator bool() const;

protected:
   void _store(const std::string & key, SV * value);

private:
   I32  _interInit() const;
   HE * _interNext() const;
};



} // namespace yoba

#endif // YOBAPERL_HASH_HPP
