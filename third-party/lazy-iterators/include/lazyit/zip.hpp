#pragma once
#ifndef INCLUDED_LAZYIT_ZIP
#define INCLUDED_LAZYIT_ZIP

/*
 * Chris Welshman 2014
 */

#include <utility>
#include <iterator>
#include <algorithm>

namespace lazyit {

template<typename It1,typename It2>
struct zip_iterator {
	typedef typename std::iterator_traits<It1>::value_type        value_type_1;
	typedef typename std::iterator_traits<It1>::reference         reference_1;
	typedef typename std::iterator_traits<It1>::difference_type   difference_type_1;
	typedef typename std::iterator_traits<It1>::iterator_category iterator_category_1;

	typedef typename std::iterator_traits<It2>::value_type        value_type_2;
	typedef typename std::iterator_traits<It2>::reference         reference_2;
	typedef typename std::iterator_traits<It2>::difference_type   difference_type_2;
	typedef typename std::iterator_traits<It2>::iterator_category iterator_category_2;

	typedef std::pair<value_type_1,value_type_2> value_type;
	typedef std::pair<reference_1,reference_2>   reference;
	typedef std::pair<It1,It2>                   pair_type;
	typedef const pair_type*                     pointer;

	typedef difference_type_1   difference_type;
	typedef iterator_category_1 iterator_category;

	zip_iterator() = default;

	explicit zip_iterator( const pair_type& pair ) : pair(pair) {}

	zip_iterator( const It1& it1, const It2& it2 ) : zip_iterator(pair_type(it1,it2)) {}

	reference operator*() const {
		return reference( *pair.first, *pair.second );
	}

	pointer operator->() const {
		return &pair;
	}

	reference_1 first() const {
		return reference_1( *pair.first );
	}

	reference_2 second() const {
		return reference_2( *pair.second );
	}

	zip_iterator<It1,It2>& operator++() {
		++pair.first;
		++pair.second;
		return *this;
	}

	zip_iterator<It1,It2> operator++(int) {
		zip_iterator<It1,It2> temp = *this;
		++(*this);
		return temp;
	}

	zip_iterator<It1,It2>& operator--() {
		--pair.first;
		--pair.second;
		return *this;
	}

	zip_iterator<It1,It2> operator--(int) {
		zip_iterator<It1,It2> temp = *this;
		--(*this);
		return temp;
	}

	zip_iterator<It1,It2>& operator+=( difference_type offset ) {
		pair.first += offset;
		pair.second += offset;
		return *this;
	}

	zip_iterator<It1,It2> operator+( difference_type offset ) const {
		zip_iterator<It1,It2> temp = *this;
		return temp += offset;
	}

	zip_iterator<It1,It2>& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	zip_iterator<It1,It2> operator-( difference_type offset ) const {
		zip_iterator<It1,It2> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const zip_iterator<It1,It2>& rhs ) const {
		return std::distance( rhs.pair.first, pair.first );
	}

	reference operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	bool operator==( const zip_iterator<It1,It2>& rhs ) const {
		return pair == rhs.pair;
	}

	bool operator!=( const zip_iterator<It1,It2>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const zip_iterator<It1,It2>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const zip_iterator<It1,It2>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const zip_iterator<It1,It2>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const zip_iterator<It1,It2>& rhs ) const {
		return !( *this < rhs );
	}

protected:
	pair_type pair;
};

template<typename It1,typename It2>
struct zip_range {
	typedef typename std::iterator_traits<It1>::value_type      value_type_1;
	typedef typename std::iterator_traits<It2>::value_type      value_type_2;
	typedef typename std::iterator_traits<It1>::difference_type difference_type_1;
	typedef typename std::iterator_traits<It2>::difference_type difference_type_2;
	typedef typename std::common_type<difference_type_1,difference_type_2>::type difference_type;
	typedef It1 iterator_1;
	typedef It2 iterator_2;
	typedef zip_iterator<iterator_1,iterator_2>  iterator;
	typedef std::reverse_iterator<iterator>      reverse_iterator;
	typedef std::pair<It1,It1>                   pair_type_1;
	typedef std::pair<It2,It2>                   pair_type_2;
	typedef std::pair<pair_type_1,pair_type_2>   range_type;
	typedef std::pair<value_type_1,value_type_2> value_type;

	explicit zip_range( const range_type& range ) : range(range) {}

	zip_range( const pair_type_1& range_1, const pair_type_2& range_2 ) : range(range_type(range_1,range_2)) {}

	difference_type size() const {
		difference_type N1 = std::distance( range.first.first, range.first.second );
		difference_type N2 = std::distance( range.second.first, range.second.second );
		return std::min( N1, N2 );
	}

	iterator begin() const {
		return iterator( range.first.first, range.second.first );
	}

	iterator end() const {
		difference_type N = size();
		return iterator( range.first.first + N, range.second.first + N );
	}

protected:
	range_type range;
};

template<typename It1,typename It2>
inline zip_range<It1,It2> zip( It1&& first_1, It1&& last_1, It2&& first_2, It2&& last_2 ) {
	return zip_range<It1,It2>(
		std::make_pair(
			std::forward<It1>(first_1),
			std::forward<It1>(last_1)
		),
		std::make_pair(
			std::forward<It2>(first_2),
			std::forward<It2>(last_2)
		)
	);
}

template<typename R1,typename R2>
inline auto zip( R1&& r1, R2&& r2 ) {
	return zip(
		begin( std::forward<R1>(r1) ),
		end( std::forward<R1>(r1) ),
		begin( std::forward<R2>(r2) ),
		end( std::forward<R2>(r2) )
	);
}

template<typename R1,typename R2>
inline auto czip( const R1& r1, const R2& r2 ) {
	return zip(
		cbegin( r1 ),
		cend( r1 ),
		cbegin( r2 ),
		cend( r2 )
	);
}

} // namespace lazyit

#endif // #ifndef INCLUDED_LAZYIT_ZIP
