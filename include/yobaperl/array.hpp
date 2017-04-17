#ifndef YOBAPERL_ARRAY_HPP
#define YOBAPERL_ARRAY_HPP

#include "yobaperl/common.hpp"
#include "yobaperl/variable.hpp"
#include "yobaperl/array_iterator.hpp"

namespace yoba {



//TODO av_delete
//TODO av_fill
class YOBAPERL_EXPORT Array : public Variable
{
public:
   using Iterator = class ArrayIterator;

   // Constructors
   Array(Perl & perl, AV * av, bool increase_refcount);

   // Elements manipulation
   Scalar get(const SSize_t index) const;
   Scalar getFirst() const;
   Scalar getLast() const;
   Array & replace(const SSize_t index, const Scalar & scalar);
   Array & push(const Scalar & scalar);
   Array & push(const Array & array);
   Array & push(const std::vector<Scalar> & scalars);
   Array & push(const std::list<Scalar> & scalars);
   Array & unshift(const Scalar & scalar);
   Array & unshift(const Array & array);
   Array & unshift(const std::vector<Scalar> & scalars);
   Array & unshift(const std::list<Scalar> & scalars);
   Scalar pop();
   Scalar shift();
   Array & clear();

   // Common
   int getSize() const;
   bool isEmpty() const;
   bool isExists(SSize_t index) const;
   Array & reserve(SSize_t size);
   Array makeCopy() const;
   Scalar makeRef() const;
   std::string toString() const;
   std::vector<Scalar> toVector() const;
   std::list<Scalar> toList() const;
   Iterator begin() const;
   Iterator end() const;

   // Internals
   AV * getAV() const;

   // Operators
   Scalar operator[] (const SSize_t index) const noexcept;
   Array & operator+= (const Scalar & scalar);
   Array & operator<< (const Scalar & scalar);
   Array operator+ (const Array & other) const;
   operator bool() const;

protected:
   void _store(const int index, Scalar scalar);
   void _push(Scalar scalar);

   bool _checkIndex(const SSize_t index) const;

   SV ** _getFirstSV() const;
   SV ** _getLastSV() const;
};



} // namespace yoba

#endif // YOBAPERL_ARRAY_HPP
