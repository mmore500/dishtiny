#pragma once
#ifndef INCLUDED_LAZYIT_INTEGER_INTERVAL
#define INCLUDED_LAZYIT_INTEGER_INTERVAL

/*
 * Chris Welshman 2014
 */

#include <type_traits>
#include <utility>
#include <iterator>

namespace lazyit {

template<typename T>
struct integer_iterator {
	typedef T                                                         value_type;
	typedef typename std::make_signed_t<T>                            difference_type;
	typedef value_type                                                reference;
	typedef typename std::add_pointer_t<T>                            pointer;
	typedef typename std::iterator_traits<pointer>::iterator_category iterator_category;

	integer_iterator() = default;

	explicit integer_iterator( T value ) : value(value) {}

	value_type operator*() const {
		return value;
	}

	integer_iterator<T>& operator++() {
		++value;
		return *this;
	}

	integer_iterator<T> operator++(int) {
		integer_iterator<T> temp = *this;
		++(*this);
		return temp;
	}

	integer_iterator<T>& operator--() {
		--value;
		return *this;
	}

	integer_iterator<T> operator--(int) {
		integer_iterator<T> temp = *this;
		--(*this);
		return temp;
	}

	integer_iterator<T>& operator+=( difference_type offset ) {
		value += offset;
		return *this;
	}

	integer_iterator<T> operator+( difference_type offset ) const {
		integer_iterator<T> temp = *this;
		return temp += offset;
	}

	integer_iterator<T>& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	integer_iterator<T> operator-( difference_type offset ) const {
		integer_iterator<T> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const integer_iterator<T>& rhs ) const {
		return difference_type(value) - difference_type(rhs.value);
	}

	value_type operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	bool operator==( const integer_iterator<T>& rhs ) const {
		return value == rhs.value;
	}

	bool operator!=( const integer_iterator<T>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const integer_iterator<T>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const integer_iterator<T>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const integer_iterator<T>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const integer_iterator<T>& rhs ) const {
		return !( *this < rhs );
	}

protected:
	T value;
};

template<typename T>
struct integer_interval_range {
	typedef T                               value_type;
	typedef typename std::make_signed_t<T>  difference_type;
	typedef integer_iterator<T>             iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::pair<T,T>                  range_type;

	explicit integer_interval_range( range_type range ) : range(range) {}

	integer_interval_range( T lower, T upper ) : range(lower,upper) {}

	difference_type size() const {
		return difference_type(T(1) + range.second) - difference_type(range.first);
	}

	iterator begin() const {
		return iterator( range.first );
	}

	iterator end() const {
		return iterator( range.second + T(1) );
	}

	value_type lower() const {
		return range.first;
	}

	value_type upper() const {
		return range.second;
	}

protected:
	range_type range;
};

template<typename T>
inline auto integer_interval( T a, T b ) {
	return integer_interval_range<T>( a, b );
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_INTEGER_INTERVAL
