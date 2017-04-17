#include "yobaperl/array.hpp"
#include "yobaperl/perl.hpp"
#include "yobaperl/scalar.hpp"

namespace yoba {



Array::Array(Perl & perl, AV * av, bool increase_refcount)
   : Variable(perl, reinterpret_cast<SV *>(av), increase_refcount)
{
}



Scalar Array::get(const SSize_t index) const
{
   if(_checkIndex(index))
   {
      SV ** elements = av_fetch(getAV(), index, /* lval */FALSE); //TODO lval?
      YOBAPERL_ASSERT(elements);

      SV * result = *elements;
      YOBAPERL_ASSERT(result);

      return Scalar(_perl, result, /* ref++ */true);
   }
   else
   {
      std::string error = "Array element not exists (index: " + std::to_string(index) + ")";

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

      if(_perl.isWarningsEnabled())
         warn("%s", error.c_str());

      return _perl.newScalar();
   }
}

Scalar Array::getFirst() const
{
   return get(0);
}

Scalar Array::getLast() const
{
   return get(getSize() - 1);
}

Array & Array::replace(const SSize_t index, const Scalar & scalar)
{
   if(_checkIndex(index))
   {
      _store(index, scalar);
   }
   else
   {
      //TODO Complete
      std::string error = "Array element not exists (index: " + std::to_string(index) + ")";

//      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

//      if(_perl.isWarningsEnabled())
//         warn("%s", error.c_str());

//      _store(index, scalar.getSV());
   }

   return *this;
}

Array & Array::push(const Scalar & scalar)
{
   _push(scalar);
   return *this;
}

Array & Array::push(const Array & array)
{
   reserve(getSize() + array.getSize());
   for(int i = 0; i < array.getSize(); i++)
      push(array[i]);
   return *this;
}

Array & Array::push(const std::vector<Scalar> & scalars)
{
   reserve(getSize() + scalars.size());
   for(auto it = scalars.cbegin(); it != scalars.cend(); it++)
      push(*it);
   return *this;
}

Array & Array::push(const std::list<Scalar> & scalars)
{
   reserve(getSize() + scalars.size());
   for(auto it = scalars.cbegin(); it != scalars.cend(); it++)
      push(*it);
   return *this;
}

Array & Array::unshift(const Scalar & scalar)
{
   av_unshift(getAV(), 1);
   _store(0, scalar);

   return *this;
}

Array & Array::unshift(const Array & array)
{
   av_unshift(getAV(), array.getSize());

   const int size = array.getSize();
   for(int i = 0; i < size; i++)
      _store(i, array[i]);

   return *this;
}

Array & Array::unshift(const std::vector<Scalar> & scalars)
{
   av_unshift(getAV(), scalars.size());

   for(std::size_t i = 0; i < scalars.size(); i++)
      _store(i, scalars[i]);

   return *this;
}

Array & Array::unshift(const std::list<Scalar> & scalars)
{
   av_unshift(getAV(), scalars.size());

   SSize_t i = 0;
   auto end_ = scalars.cend();
   for(auto it = scalars.cbegin(); it != end_; ++it)
      _store(i++, *it);

   return *this;
}

Scalar Array::pop()
{
   if(!isEmpty())
   {
      SV * result = av_pop(getAV());
      YOBAPERL_ASSERT(result);
      return Scalar(_perl, result, false);
   }
   else
   {
      static const std::string error = "Pop empty array";

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

      if(_perl.isWarningsEnabled())
         warn("%s", error.c_str());

      return _perl.newScalar();
   }
}

Scalar Array::shift()
{
   if(!isEmpty())
   {
      SV * result = av_shift(getAV());
      YOBAPERL_ASSERT(result);
      return Scalar(_perl, result, false);
   }
   else
   {
      static const std::string error = "Pop empty array";

      if(_perl.isExceptionsEnabled())
         throw PerlException(_perl.getId(), error);

      if(_perl.isWarningsEnabled())
         warn("%s", error.c_str());

      return _perl.newScalar();
   }
}

Array & Array::clear()
{
   av_clear(getAV());
   return *this;
}



int Array::getSize() const
{
   return av_len(getAV()) + 1; // Perl_av_len returns -1 if array is empty
}

bool Array::isEmpty() const
{
   return getSize() == 0;
}

bool Array::isExists(SSize_t index) const
{
   return av_exists(getAV(), index);
}

Array & Array::reserve(SSize_t size)
{
   av_extend(getAV(), size - 1);
   return *this;
}

Array Array::makeCopy() const
{
   Array result = _perl.newArray();
   result.reserve(getSize());

   for(Scalar elem : *this)
      result.push(elem.makeCopy());

   return result;
}

Scalar Array::makeRef() const
{
   return Scalar(_perl, newRV_inc(getSV()), /*ref++*/false);
}

std::string Array::toString() const
{
   std::stringstream ss;

   ss << "(";

   const int array_size = getSize();
   for(SSize_t i = 0; i < array_size; i++)
   {
      ss << (*this).get(i);
      if(i < array_size - 1)
         ss << ", ";
   }

   ss << ")";

   return ss.str();
}

std::vector<Scalar> Array::toVector() const
{
   std::vector<Scalar> result;
   result.reserve(getSize());

   auto end_ = end();
   for(auto it = begin(); it != end_; ++it)
      result.push_back(*it);

   return result;
}

std::list<Scalar> Array::toList() const
{
   std::list<Scalar> result;

   auto end_ = end();
   for(auto it = begin(); it != end_; ++it)
      result.push_back(*it);

   return result;
}

Array::Iterator Array::begin() const
{
   return Iterator(getPerl(), _getFirstSV());
}

Array::Iterator Array::end() const
{
   return Iterator(getPerl(), _getLastSV() + 1);
}



AV * Array::getAV() const
{
   return reinterpret_cast<AV *>(getSV());
}



Scalar Array::operator[] (const SSize_t index) const noexcept
{
   return Scalar(_perl, *av_fetch(getAV(), index, /* lval */FALSE), true);
}

Array & Array::operator+= (const Scalar & scalar)
{
   push(scalar);
   return *this;
}

Array & Array::operator<< (const Scalar & scalar)
{
   push(scalar);
   return *this;
}

Array Array::operator+ (const Array & other) const
{
   Array result = _perl.newArray();
   result.reserve(getSize() + other.getSize());

   result.push(*this);
   result.push(other);

   return result;
}

Array::operator bool() const
{
   return !isEmpty();
}



void Array::_store(const int index, Scalar scalar)
{
   av_store(getAV(), index, scalar.detachSV());
}

void Array::_push(Scalar scalar)
{
   av_push(getAV(), scalar.detachSV());
}

bool Array::_checkIndex(const SSize_t index) const
{
   const SSize_t array_size = getSize();
   return array_size > 0 && index >= 0 && index < array_size && isExists(index);
}

SV ** Array::_getFirstSV() const
{
   SV ** result = av_fetch(getAV(), 0, /* lval */FALSE);
   YOBAPERL_ASSERT(result);
   YOBAPERL_ASSERT(*result);
   return result;
}

SV ** Array::_getLastSV() const
{
   SV ** result =  _getFirstSV() + (getSize() - 1);
   YOBAPERL_ASSERT(result);
   YOBAPERL_ASSERT(*result);
   return result;
}



} // namespace yoba
