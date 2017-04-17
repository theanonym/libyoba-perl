#include "yobaperl/hash.hpp"
#include "yobaperl/perl.hpp"
#include "yobaperl/scalar.hpp"

namespace yoba {



Hash::Hash(Perl & perl, HV * hv, bool increase_refcount)
   : Variable(perl, reinterpret_cast<SV *>(hv), increase_refcount)
{
}



I32 Hash::getSize() const
{
   return HvUSEDKEYS(getHV());
}

bool Hash::isEmpty() const
{
   return getSize() == 0;
}

bool Hash::isExists(const std::string & key) const
{
   return hv_exists(getHV(), key.c_str(), key.length());
}

Scalar Hash::get(const std::string & key)
{
   if(isExists(key))
   {
      SV ** elements = hv_fetch(getHV(), key.c_str(), key.length(), /* lval */NULL);
      YOBAPERL_ASSERT(elements);

      SV * element = *elements;
      YOBAPERL_ASSERT(element);

      return Scalar(_perl, element, /* ref++ */true);
   }
   else
   {
      std::string error = "Hash entry not exists (key: " + key + ")";

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

      if(_perl.isWarningsEnabled())
         warn("%s", error.c_str());

      return _perl.newScalar();
   }
}

Hash & Hash::insert(const std::string & key, const Scalar & value)
{
   _store(key, value.getSV());
   return *this;
}

Hash & Hash::insert(const std::pair<std::string, Scalar> & pair)
{
   insert(pair.first, pair.second);
   return *this;
}

Hash & Hash::insert(const std::unordered_map<std::string, Scalar> & hashmap)
{
   for(auto it = hashmap.cbegin(); it != hashmap.cend(); it++)
      insert(it->first, it->second);
   return *this;
}

Scalar Hash::remove(const std::string & key)
{
   if(isExists(key))
   {
      SV * result = hv_delete(getHV(), key.c_str(), static_cast<U32>(key.length()), /* flags */0);
      YOBAPERL_ASSERT(result);
      return Scalar(_perl, result, /* ref++ */true);
   }
   else
   {
      std::string error = "Hash entry not exists (key: " + key + ")";

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

      if(_perl.isWarningsEnabled())
         warn("%s", error.c_str());

      return _perl.newScalar();
   }
}

Hash & Hash::clear()
{
   hv_clear(getHV());
   return *this;
}

std::string Hash::toString() const
{
   std::stringstream ss;

   ss << "(";

   auto it = begin();
   while(it)
   {
      ss << *it;
      if(++it)
         ss << ", ";
   }

   ss << ")";

   return ss.str();
}

std::unordered_map<std::string, Scalar> Hash::toMap() const
{
   std::unordered_map<std::string, Scalar> result;
   result.reserve(getSize());

   auto it = begin();
   while(it)
   {
      result.insert((*it).toPair());
      ++it;
   }

   return result;
}

Scalar Hash::makeRef() const
{
   return Scalar(_perl, newRV_inc(MUTABLE_SV(getHV())), false);
}

Hash Hash::makeCopy() const
{
   Hash result = _perl.newHash();

   for(HashEntry entry : *this)
      result.insert(entry.getKey(), entry.getValue().makeCopy());

   return result;
}

Hash::Iterator Hash::begin() const
{
   return Iterator(*this);
}

Hash::Iterator Hash::end() const
{
   return Iterator(*this, /* is_null */true);
}



HV * Hash::getHV() const
{
   return reinterpret_cast<HV *>(getSV());
}



Hash::operator bool() const
{
   return !isEmpty();
}

Scalar Hash::operator[] (const std::string & key) noexcept
{
   return Scalar(_perl, *hv_fetch(getHV(), key.c_str(), key.length(), /* lval */NULL), /* ref++ */true);
}

Scalar Hash::operator[] (const char * key) noexcept
{
   return (*this)[std::string(key)];
}



void Hash::_store(const std::string & key, SV * value)
{
   SvREFCNT_inc_NN(value);
   hv_store(getHV(), key.c_str(), key.length(), value, /* hash */0);
}

I32 Hash::_interInit() const
{
   return hv_iterinit(getHV());
}

HE * Hash::_interNext() const
{
   return hv_iternext(getHV());
}



} // namespace yoba
